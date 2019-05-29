type t = {
  response_type: list(string),
  client: Client.t,
  redirect_uri: string,
  scope: list(string),
  state: option(string),
  nonce: string,
};

let parse_query:
  (~clients: list(Client.t), Uri.t) =>
  result(t, list([ | `Msg(string) | `Client(Client.t)]));