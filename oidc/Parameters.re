type t = {
  response_type: list(string),
  client: Client.t,
  redirect_uri: string,
  scope: list(string), // Must include at least the openid scope
  state: option(string),
  nonce: string,
};

let get_client = (~clients, ~client_id, ()) => {
  switch (client_id) {
  | Some(client_id) =>
    CCList.find_opt((client: Client.t) => client.id == client_id, clients)
    |> (
      fun
      | Some(client) => Ok(client)
      | None => Error(`Msg("No client found"))
    )
  | None => Error(`Msg("No client_id provided"))
  };
};

let parse_query = (~clients, uri) => {
  let getQueryParam = Uri.get_query_param(uri);

  let response_type =
    getQueryParam("response_type")
    |> CCOpt.map(String.split_on_char(' '))
    |> CCOpt.map(response_type => Ok(response_type))
    |> CCOpt.get_or(~default=Error(`Msg("No response_type")));

  let redirect_uri =
    getQueryParam("redirect_uri")
    |> CCOpt.map(redirect_uri => Ok(redirect_uri))
    |> CCOpt.get_or(~default=Error(`Msg("No redirect_uri")));

  let client =
    get_client(~clients, ~client_id=getQueryParam("client_id"), ());

  switch (client, response_type, redirect_uri) {
  | (Ok(client), Ok(response_type), Ok(redirect_uri))
      when client.redirect_uri == redirect_uri =>
    Ok({
      response_type,
      client,
      redirect_uri,
      scope:
        getQueryParam("scope")
        |> CCOpt.get_or(~default="")
        |> String.split_on_char(' '),
      state: getQueryParam("state"),
      nonce: getQueryParam("nonce") |> CCOpt.get_or(~default="12345"),
    })
  | (Ok(client), Ok(_), Ok(_)) =>
    Error([`Client(client), `Msg("redirect_uri does not match")])
  | (
      Error(client_id_msg),
      Error(response_type_msg),
      Error(redirect_uri_msg),
    ) =>
    Error([client_id_msg, response_type_msg, redirect_uri_msg])
  | (Ok(client), Error(response_type_msg), Error(redirect_uri_msg)) =>
    Error([`Client(client), response_type_msg, redirect_uri_msg])
  | (Ok(client), Ok(_), Error(redirect_uri_msg)) =>
    Error([`Client(client), redirect_uri_msg])
  | (Ok(client), Error(response_type_msg), Ok(redirect_uri)) =>
    Error([`Client(client), response_type_msg, `RedirectUri(redirect_uri)])
  | (Error(client_id_msg), Ok(_), Error(redirect_uri_msg)) =>
    Error([client_id_msg, redirect_uri_msg])
  | (Error(client_id_msg), Ok(_), Ok(redirect_uri)) =>
    Error([client_id_msg, `RedirectUri(redirect_uri)])
  | (Error(client_id_msg), Error(response_type_msg), Ok(redirect_uri)) =>
    Error([client_id_msg, response_type_msg, `RedirectUri(redirect_uri)])
  };
};