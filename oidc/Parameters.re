type t = {
  response_type: list(string),
  client_id: string,
  redirect_uri: string,
  scope: list(string), // Must include at least the openid scope
  state: option(string),
  nonce: string,
};

let parseQuery = uri => {
  let getQueryParam = Uri.get_query_param(uri);
  let parameters = {
    response_type:
      getQueryParam("response_type")
      |> CCOpt.get_or(~default="")
      |> String.split_on_char(' '),
    client_id: getQueryParam("client_id") |> CCOpt.get_or(~default="1234"),
    redirect_uri:
      getQueryParam("redirect_uri")
      |> CCOpt.get_or(~default="http://localhost:3000"),
    scope:
      getQueryParam("scope")
      |> CCOpt.get_or(~default="")
      |> String.split_on_char(' '),
    state: getQueryParam("state"),
    nonce: getQueryParam("nonce") |> CCOpt.get_or(~default="12345"),
  };

  if (parameters.response_type == ["code"]) {
    Some(parameters);
  } else {
    None;
  };
};

let get_client = (~clients, t) => {
  CCList.find_opt(
    (client: Client.t) =>
      client.id == t.client_id && client.redirect_uri == t.redirect_uri,
    clients,
  );
};