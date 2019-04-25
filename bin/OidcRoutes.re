let makeCallback = (
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
    Routes.ValidateAuth.makeRoute(
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~read_body,
      reqd,
    )

  | (`GET, [".well-known", "jwks.json"]) =>
    let json = {j|{}|j};
    Http.Response.Json.make(
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~json,
      reqd,
    );
    Lwt.return_unit;
  | (`GET, [".well-known", "openid-configuration"]) =>
    let json = Printf.sprintf({|{
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
}|},context.host, context.host, context.host, context.host, context.host);

    Http.Response.Json.make(
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~json,
      reqd,
    );
    Lwt.return_unit;
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