let makeCallback =
    (
      ~target,
      ~method,
      ~getHeader,
      ~create_response,
      ~respond_with_string: ('a, 'b, string) => unit,
      ~headers_of_list,
      ~read_body,
      ~headers,
      ~context: Context.t,
      reqd,
    ) => {
  open Lwt.Infix;

  Logs.info(m =>
    m("%s request to %s", Http.Method.to_string(method), target)
  );

  let req_uri = target |> Uri.of_string;
  let req_path = Uri.path(req_uri);
  let path_parts = Str.(split(regexp("/"), req_path));

  switch (method, path_parts) {
  | (`GET, ["authorize"]) =>
    context.session_store
    >>= (
      (session_store: SessionStorage.t) =>
        Routes.Authorize.makeRoute(
          ~set_session=session_store.set,
          ~respond_with_string,
          ~create_response,
          ~headers_of_list,
          ~getHeader,
          ~redirectPath=context.host ++ "/interaction",
          target,
          reqd,
        )
    )
  | (`GET, ["interaction"]) =>
    Routes.LoginForm.makeRoute(
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      reqd,
    )
  | (`POST, ["interaction"]) =>
    let session_id =
      getHeader("Cookie")
      |> CCOpt.get_or(~default="")
      |> Http.Cookie.get_cookie(~key="session");

    switch (session_id) {
    | Some(session) =>
      Console.log(session);
      let cookie_value = session.value;
      context.session_store
      >>= (
        (session_store: SessionStorage.t) =>
          session_store.get(~kind="session", cookie_value)
          >|= (
            session_string =>
              session_string |> Uri.of_string |> Oidc.Parameters.parseQuery
          )
          >>= (
            (parameters: Oidc.Parameters.t) => {
              Routes.ValidateAuth.makeRoute(
                ~oidc_state=CCOpt.get_or(~default="", parameters.state),
                ~respond_with_string,
                ~create_response,
                ~headers_of_list,
                ~read_body,
                reqd,
              );
            }
          )
      );
    | None =>
      Http.Response.Unauthorized.make(
        ~respond_with_string,
        ~create_response,
        ~headers_of_list,
        reqd,
        "No session found",
      )
      |> Lwt.return
    };
  | (`GET, [".well-known", "jwks.json"]) =>
    let jwk_string = context.jwk |> Oidc.Jwk.to_json |> Yojson.to_string;
    let json = Printf.sprintf({j|{"keys": [%s]}|j}, jwk_string);

    Http.Response.Json.make(
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~json,
      reqd,
    );
    Lwt.return_unit;
  | (`GET, [".well-known", "openid-configuration"]) =>
    let json =
      Printf.sprintf(
        {|{
  "issuer": "%s",
  "authorization_endpoint": "%s/authorize",
  "token_endpoint": "%s/token",
  "userinfo_endpoint": "%s/userinfo",
  "jwks_uri": "%s/.well-known/jwks.json",
  "scopes_supported": [
    "user"
  ],
  "response_types_supported": [
    "code"
  ],
  "token_endpoint_auth_methods_supported": [
    "client_secret_basic"
  ]
}|},
        context.host,
        context.host,
        context.host,
        context.host,
        context.host,
      );

    Http.Response.Json.make(
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~json,
      reqd,
    );
    Lwt.return_unit;
  | (`POST, ["token"]) =>
    Routes.Token.make(
      ~read_body,
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~priv_key=context.rsa_priv,
      ~host=context.host,
      reqd,
    )
  | _ =>
    Http.Response.Ok.make(
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      reqd,
    );
    Lwt.return_unit;
  };
};