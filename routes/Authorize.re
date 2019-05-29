let makeRoute =
    (
      ~set_session,
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~get_header,
      ~redirectPath,
      ~clients,
      target,
      reqd,
    ) => {
  open Lwt.Infix;
  open Http;

  let parameters =
    Uri.of_string(target) |> Oidc.Parameters.parse_query(~clients);

  let error_message = msgs =>
    CCList.map(
      fun
      | `Msg(s) => s
      | _ => "",
      msgs,
    )
    |> CCString.concat("");

  Oidc.Parameters.(
    switch (parameters) {
    | Ok(parameters) =>
      let cookie_key =
        Uuidm.v4_gen(Random.State.make_self_init(), ()) |> Uuidm.to_string;
      let cookie_name = "session";
      let cookie_value =
        String.concat("", [cookie_name, "=", cookie_key, " ;Max-Age=300"]);

      let has_code_return_type =
        CCList.exists(rt => rt == "code", parameters.response_type);
      let has_openid_scope =
        CCList.exists(s => s == "openid", parameters.scope);
      switch (has_code_return_type, has_openid_scope) {
      | (true, true) =>
        set_session(~kind=cookie_name, ~key=cookie_key, target)
        >|= (
          () =>
            Http.Response.Redirect.make(
              ~respond_with_string,
              ~create_response,
              ~headers_of_list,
              ~extra_headers=[("set-cookie", cookie_value)],
              ~targetPath=redirectPath,
              reqd,
            )
        )
      | _ =>
        Http.Response.Redirect.make(
          ~respond_with_string,
          ~create_response,
          ~headers_of_list,
          ~targetPath=
            parameters.client.redirect_uri
            ++ "?error=only code response_type supported and scope must include openid",
          reqd,
        )
        |> Lwt.return
      };
    | Error([`Client(client), ...msgs]) =>
      Http.Response.Redirect.make(
        ~respond_with_string,
        ~create_response,
        ~headers_of_list,
        ~targetPath=client.redirect_uri ++ "?error=" ++ error_message(msgs),
        reqd,
      )
      |> Lwt.return
    | Error(msgs) =>
      let error_string =
        CCList.filter(
          fun
          | `Msg(_) => true
          | _ => false,
          msgs,
        )
        |> error_message;

      let redirect_uri_opt =
        CCList.map(
          fun
          | `RedirectUri(redirect_uri) => Some(redirect_uri)
          | _ => None,
          msgs,
        )
        |> CCList.keep_some
        |> CCList.get_at_idx(0);

      (
        switch (redirect_uri_opt) {
        | Some(redirect_uri) =>
          Http.Response.Redirect.make(
            ~respond_with_string,
            ~create_response,
            ~headers_of_list,
            ~targetPath=redirect_uri ++ "?error=" ++ error_string,
            reqd,
          )
        | None =>
          Http.Response.Unauthorized.make(
            ~respond_with_string,
            ~create_response,
            ~headers_of_list,
            reqd,
            error_string,
          )
        }
      )
      |> Lwt.return;
    }
  );
};