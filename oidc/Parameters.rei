type t = {
  response_type: list(string),
  client: Client.t,
  redirect_uri: string,
  scope: list(string),
  state: option(string),
  nonce: string,
};

type parse_state =
  | Invalid(string)
  | UnauthorizedClient(Client.t)
  | InvalidScope(Client.t)
  | InvalidWithClient(Client.t)
  | InvalidWithRedirectUri(string)
  | Valid(t);

let parse_query: (~clients: list(Client.t), Uri.t) => parse_state;
