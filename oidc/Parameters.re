type t = {
  response_type: list(string),
  client: Client.t,
  redirect_uri: string,
  scope: list(string), // Must include at least the openid scope
  state: option(string),
  nonce: string,
};

let get_client = (~clients, ~client_id, ~redirect_uri, ()) => {
  switch (client_id, redirect_uri) {
  | (Some(client_id), Some(redirect_uri)) =>
    CCList.find_opt(
      (client: Client.t) =>
        client.id == client_id && client.redirect_uri == redirect_uri,
      clients,
    )
    |> (
      fun
      | Some(client) => Ok(client)
      | None => Error(`Msg("No client found"))
    )
  | (None, _) => Error(`Msg("No client_id provided"))
  | (_, None) => Error(`Msg("No redirect_uri provided"))
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
    |> CCOpt.get_or(~default=Error(`Msg("No response_type")));

  let client =
    get_client(
      ~clients,
      ~client_id=getQueryParam("client_id"),
      ~redirect_uri=getQueryParam("redirect_uri"),
      (),
    );

  switch (client, response_type, redirect_uri) {
  | (Ok(client), Ok(response_type), Ok(redirect_uri)) =>
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
  | (
      Error(client_id_msg),
      Error(response_type_msg),
      Error(redirect_uri_msg),
    ) =>
    Error([client_id_msg, response_type_msg, redirect_uri_msg])
  | (Ok(client), Error(response_type_msg), Error(redirect_uri_msg)) =>
    Error([`Client(client), response_type_msg, redirect_uri_msg])
  | (Ok(client), _, Error(redirect_uri_msg)) =>
    Error([`Client(client), redirect_uri_msg])
  | (Error(client_id_msg), _, Error(redirect_uri_msg)) =>
    Error([client_id_msg, redirect_uri_msg])
  | (Error(client_id_msg), Error(response_type_msg), _) =>
    Error([client_id_msg, response_type_msg])
  | (Error(client_id_msg), _, _) => Error([client_id_msg])
  | (_, Error(response_type_msg), _) => Error([response_type_msg])
  };
};