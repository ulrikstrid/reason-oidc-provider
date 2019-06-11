let makeCallback = (~httpImpl, ~context: Context.t, reqd) => {
  open Lwt.Infix;
  open Http.HttpImpl;

  let req_uri = httpImpl.target |> Uri.of_string;
  let req_path = Uri.path(req_uri);
  let path_parts = Str.(split(regexp("/"), req_path));

  context.session_store
  >>= (
    session_store =>
      switch (httpImpl.meth, path_parts) {
      | (`GET, ["authorize"]) =>
        Routes.Authorize.makeRoute(
          ~httpImpl,
          ~set_session=session_store.set(~kind="session"),
          ~redirectPath=context.host ++ "/interaction",
          ~clients=context.clients,
          reqd,
        )
      | (`GET, ["interaction"]) =>
        Routes.LoginForm.makeRoute(~httpImpl, reqd)
      | (`POST, ["interaction"]) =>
        Routes.ValidateAuth.makeRoute(
          ~httpImpl,
          ~find_user=context.find_user,
          ~hash_key=context.rsa_priv,
          ~get_session=session_store.get(~kind="session"),
          ~set_code=session_store.set(~kind="code"),
          ~clients=context.clients,
          reqd,
        )
      | (`GET, [".well-known", "jwks.json"]) =>
        Routes.Jwks.make(~httpImpl, ~jwk=context.jwk, reqd)
      | (`GET, [".well-known", "openid-configuration"]) =>
        Routes.Discovery.make(~httpImpl, ~host=context.host, reqd)
      | (`POST, ["token"]) =>
        Routes.Token.make(
          ~httpImpl,
          ~priv_key=context.rsa_priv,
          ~host=context.host,
          ~find_code=session_store.find(~kind="code"),
          ~remove_code=session_store.remove(~kind="code"),
          ~set_access_token=session_store.set(~kind="access_token"),
          ~remove_access_token=session_store.remove(~kind="access_token"),
          ~jwk=context.jwk,
          reqd,
        )
      | (`POST, ["userinfo"])
      | (`GET, ["userinfo"]) =>
        Routes.UserInfo.make(
          ~httpImpl,
          ~find_access_token=session_store.find(~kind="access_token"),
          reqd,
        )
      | _ => Routes.FourOhFour.make(~httpImpl, reqd)
      }
  );
};
