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

  let state_query_string =
    Uri.get_query_param(uri, "state")
    |> CCOpt.map_or(~default="", state => "&state=" ++ state);

  Oidc.Parameters.(
    switch (parameters) {
    | Valid(parameters) =>
      let cookie_key =
        Uuidm.v4_gen(Random.State.make_self_init(), ()) |> Uuidm.to_string;
      let cookie_value =
        String.concat("", ["session=", cookie_key, " ;Max-Age=300"]);

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
      );
    | UnauthorizedClient(client) =>
      Logs.warn(m => m("%s", "unauthorized_client"));
      Http.Response.Redirect.make(
        ~respond_with_string,
        ~create_response,
        ~headers_of_list,
        ~targetPath=
          client.redirect_uri
          ++ "?error=unauthorized_client"
          ++ state_query_string,
        reqd,
      )
      |> Lwt.return;
    | InvalidScope(client) =>
      Logs.warn(m => m("%s", "invalid_scope"));
      Http.Response.Redirect.make(
        ~respond_with_string,
        ~create_response,
        ~headers_of_list,
        ~targetPath=
          client.redirect_uri ++ "?error=invalid_scope" ++ state_query_string,
        reqd,
      )
      |> Lwt.return;
    | InvalidWithClient(client) =>
      Logs.warn(m => m("%s", "invalid_request"));
      Http.Response.Redirect.make(
        ~respond_with_string,
        ~create_response,
        ~headers_of_list,
        ~targetPath=
          client.redirect_uri ++ "?error=invalid_request" ++ state_query_string,
        reqd,
      )
      |> Lwt.return;
    | InvalidWithRedirectUri(redirect_uri) =>
      Logs.warn(m => m("%s", "invalid_request"));
      Http.Response.Redirect.make(
        ~respond_with_string,
        ~create_response,
        ~headers_of_list,
        ~targetPath=
          redirect_uri ++ "?error=invalid_request" ++ state_query_string,
        reqd,
      )
      |> Lwt.return;
    | Invalid(error_string) =>
      Logs.warn(m => m("%s", "Invalid request, showing error to end-user"));
      Http.Response.Unauthorized.make(
        ~respond_with_string,
        ~create_response,
        ~headers_of_list,
        reqd,
        error_string,
      )
      |> Lwt.return;
    }
  );
};