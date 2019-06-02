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

  let uri = Uri.of_string(target);

  let parameters = Oidc.Parameters.parse_query(~clients, uri);

  let error_message = msgs =>
    CCList.map(
      fun
      | `Msg(s) => s
      | _ => "",
      msgs,
    )
    |> CCString.concat(" ");

  let state_query_string =
    Uri.get_query_param(uri, "state")
    |> CCOpt.map_or(~default="", state => "&state=" ++ state);

  Oidc.Parameters.(
    switch (parameters) {
    | Ok(parameters) =>
      let cookie_key =
        Uuidm.v4_gen(Random.State.make_self_init(), ()) |> Uuidm.to_string;
      let cookie_value =
        String.concat("", ["session=", cookie_key, " ;Max-Age=300"]);

      let has_code_return_type =
        CCList.exists(rt => rt == "code", parameters.response_type);
      let has_openid_scope =
        CCList.exists(s => s == "openid", parameters.scope);
      switch (has_code_return_type, has_openid_scope) {
      | (true, true) =>
        set_session(~key=cookie_key, target)
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
      | (false, true) =>
        Http.Response.Redirect.make(
          ~respond_with_string,
          ~create_response,
          ~headers_of_list,
          ~targetPath=
            parameters.client.redirect_uri
            ++ "?error=unauthorized_client"
            ++ state_query_string,
          reqd,
        )
        |> Lwt.return
      | (true, false) =>
        Http.Response.Redirect.make(
          ~respond_with_string,
          ~create_response,
          ~headers_of_list,
          ~targetPath=
            parameters.client.redirect_uri
            ++ "?error=invalid_scope"
            ++ state_query_string,
          reqd,
        )
        |> Lwt.return
      | (false, false) =>
        Http.Response.Redirect.make(
          ~respond_with_string,
          ~create_response,
          ~headers_of_list,
          ~targetPath=
            parameters.client.redirect_uri
            ++ "?error=invalid_request"
            ++ state_query_string,
          reqd,
        )
        |> Lwt.return
      };
    | Error([`Client(client), ...msgs]) =>
      Logs.warn(m => m("%s", error_message(msgs)));

      Http.Response.Redirect.make(
        ~respond_with_string,
        ~create_response,
        ~headers_of_list,
        ~targetPath=
          client.redirect_uri ++ "?error=invalid_request" ++ state_query_string,
        reqd,
      )
      |> Lwt.return;
    | Error(msgs) =>
      let error_string =
        CCList.filter(
          fun
          | `Msg(_) => true
          | _ => false,
          msgs,
        )
        |> error_message;

      Logs.warn(m => m("%s", error_string));

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
            ~targetPath=
              redirect_uri ++ "?error=invalid_request" ++ state_query_string,
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