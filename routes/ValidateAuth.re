let makeRoute =
    (
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~read_body,
      ~get_header,
      ~find_user,
      ~get_session,
      ~set_code,
      ~hash_key,
      ~clients,
      reqd,
    ) => {
  open Lwt.Infix;

  let session_id =
    get_header("Cookie")
    |> CCOpt.get_or(~default="")
    |> Http.Cookie.get_cookie(~key="session");

  switch (session_id) {
  | None =>
    Http.Response.Unauthorized.make(
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      reqd,
      "No session found",
    )
    |> Lwt.return
  | Some(session) =>
    let cookie_value = session.value;

    get_session(cookie_value)
    >|= (
      session_string =>
        session_string
        |> Uri.of_string
        |> Oidc.Parameters.parse_query(~clients)
    )
    >>= (
      fun
      | Valid(parameters) =>
        read_body(reqd)
        >|= (
          body => {
            Logs.info(m => m("body: %s", body));

            let parsed_body = Http.UrlencodedForm.parse(body);

            let username_opt =
              Http.UrlencodedForm.get_param("username", parsed_body);
            let password_opt =
              Http.UrlencodedForm.get_param("password", parsed_body);

            let user_opt = CCOpt.flat_map(find_user, username_opt);

            CCOpt.map2(
              (password, user) => (password, user),
              password_opt,
              user_opt,
            )
            |> CCOpt.flat_map(((password, user: Oidc.User.t)) =>
                 if (user.password == password) {
                   Some(user);
                 } else {
                   None;
                 }
               )
            |> CCOpt.map(user => {
                 let bodyValue =
                   `Assoc([
                     ("nonce", `String(parameters.nonce)),
                     ("user", Oidc.User.to_json(user)),
                   ])
                   |> Yojson.to_string;

                 let code = Uuidm.create(`V4) |> Uuidm.to_string;
                 (code, bodyValue);
               });
          }
        )
        >>= (
          v => {
            switch (v) {
            | Some((code, bodyValue)) =>
              set_code(~key=code, bodyValue)
              >|= (
                () =>
                  Http.Response.Redirect.make(
                    ~respond_with_string,
                    ~create_response,
                    ~headers_of_list,
                    ~targetPath=
                      Printf.sprintf(
                        "%s?code=%s%s",
                        parameters.redirect_uri,
                        code,
                        CCOpt.map_or(
                          ~default="",
                          state => "&state=" ++ state,
                          parameters.state,
                        ),
                      ),
                    reqd,
                  )
              )
            | None =>
              Http.Response.Unauthorized.make(
                ~respond_with_string,
                ~create_response,
                ~headers_of_list,
                reqd,
                "Wrong username or password",
              )
              |> Lwt.return
            };
          }
        )
      | _ =>
        Http.Response.Unauthorized.make(
          ~respond_with_string,
          ~create_response,
          ~headers_of_list,
          reqd,
          "Invalid session found",
        )
        |> Lwt.return
    );
  };
};
