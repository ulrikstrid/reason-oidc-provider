let makeCallback =
    (
      ~target,
      ~method,
      ~get_header,
      ~create_response,
      ~respond_with_string: ('a, 'b, string) => unit,
      ~headers_of_list,
      ~read_body,
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
          ~get_header,
          ~redirectPath=context.host ++ "/interaction",
          ~clients=context.clients,
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
      get_header("Cookie")
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
            (p: option(Oidc.Parameters.t)) => {
              switch (p) {
              | Some(parameters) =>
                Routes.ValidateAuth.makeRoute(
                  ~parameters,
                  ~respond_with_string,
                  ~create_response,
                  ~headers_of_list,
                  ~read_body,
                  reqd,
                )
              | None =>
                Http.Response.Unauthorized.make(
                  ~respond_with_string,
                  ~create_response,
                  ~headers_of_list,
                  reqd,
                  "invalid query",
                )
                |> Lwt.return
              };
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
    Routes.Jwks.make(
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~jwk=context.jwk,
      reqd,
    )
  | (`GET, [".well-known", "openid-configuration"]) =>
    Routes.Discovery.make(
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~host=context.host,
      reqd,
    )
  | (`POST, ["token"]) =>
    Routes.Token.make(
      ~read_body,
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~priv_key=context.rsa_priv,
      ~host=context.host,
      ~jwk=context.jwk,
      reqd,
    )
  | _ =>
    Http.Response.NotFound.make(
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~message=req_path ++ " not found",
      reqd,
    )
    |> Lwt.return
  };
};