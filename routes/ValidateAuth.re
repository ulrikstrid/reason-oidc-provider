let makeRoute =
    (
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~read_body,
      ~get_header,
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
      | Ok(parameters) =>
        read_body(reqd)
        >>= (
          body => {
            // Validate credentials
            // switch on OK credentials
            let bodyValue =
              Printf.sprintf(
                {|{"nonce": "%s", "body": %s}|},
                parameters.nonce,
                body,
              );

            let code =
              bodyValue
              |> Cstruct.of_string
              |> Nocrypto.Rsa.encrypt(
                   ~key=hash_key |> Nocrypto.Rsa.pub_of_priv,
                 )
              |> Cstruct.to_string
              |> Base64.encode_exn(
                   ~pad=false,
                   ~alphabet=Base64.uri_safe_alphabet,
                 );

            set_code(~key=code, bodyValue) >|= (() => code);
          }
        )
        >|= (
          code => {
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
            );
          }
        )
      | Error(_) =>
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