type t = {
  clients: list(Oidc.Client.t),
  provider: Oidc.Provider.t,
};

let get_clients_from_env:
  unit => result(list(Oidc.Client.t), [ | `Msg(string)]) =
  () => {
    Oidc.Client.(
      switch (Sys.getenv_opt("CLIENT_ID"), Sys.getenv_opt("REDIRECT_URI")) {
      | (Some(client_id), Some(redirect_uri)) =>
        let clients = [{id: client_id, redirect_uri}];
        Ok(clients);
      | (None, Some(_)) => Error(`Msg("No client_id provided"))
      | (Some(_), None) => Error(`Msg("No redirect_uri provided"))
      | (None, None) => Error(`Msg("No client config provided"))
      }
    );
  };

let get_provider_from_env: unit => result(Oidc.Provider.t, [ | `Msg(string)]) =
  () => {
    Oidc.Provider.(
      switch (Sys.getenv_opt("HOST")) {
      | Some(host) => Ok({host: host})
      | None => Error(`Msg("No host provided"))
      }
    );
  };

let get_from_env: unit => result(t, [ | `Msg(string)]) =
  () => {
    let clients: result(list('a), 'b) = get_clients_from_env();

    let provider = get_provider_from_env();

    switch (clients, provider) {
    | (Ok(clients), Ok(provider)) => Ok({clients, provider})
    | (Error(e), Ok(_)) => Error(e)
    | (Ok(_), Error(e)) => Error(e)
    | (Error(`Msg(clients_msg)), Error(`Msg(provider_msg))) =>
      Error(`Msg(clients_msg ++ " " ++ provider_msg))
    };
  };
