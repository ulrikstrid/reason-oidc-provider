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

  context.session_store 
  >>= session_store =>
      switch (method, path_parts) {
      | (`GET, ["authorize"]) =>
        Routes.Authorize.makeRoute(
          ~set_session=session_store.set(~kind="session"),
          ~respond_with_string,
          ~create_response,
          ~headers_of_list,
          ~get_header,
          ~redirectPath=context.host ++ "/interaction",
          ~clients=context.clients,
          target,
          reqd,
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
          ~get_header,
          ~read_body,
          ~hash_key=context.rsa_priv,
          ~get_session=session_store.get(~kind="session"),
          ~set_code=session_store.set(~kind="code"),
          ~clients=context.clients,
          reqd,
        )
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
          ~get_code=session_store.find(~kind="code"),
          ~jwk=context.jwk,
          reqd,
        )
      | _ =>
        Routes.FourOhFour.make(
          ~respond_with_string,
          ~create_response,
          ~headers_of_list,
          ~req_path,
          reqd,
        )
      };
  };