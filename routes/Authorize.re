/*

 curl --http2-prior-knowledge http://localhost:3000/authorize?response_type=code&client_id=s6BhdRkqt3&redirect_uri=https%3A%2F%2Fclient.example.org%2Fcb&scope=openid%20profile&state=af0ifjsldkj&nonce=n-0S6_WzA2Mj

 http://localhost:3000/authorize?
   response_type=code
  &client_id=s6BhdRkqt3
  &redirect_uri=https%3A%2F%2Fclient.example.org%2Fcb
  &scope=openid%20profile
  &state=af0ifjsldkj
  &nonce=n-0S6_WzA2Mj
  */

type parameters = {
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
    parameters;
  } else {
    raise(Not_found);
  };
};

let makeRoute =
    (
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~getHeader,
      target,
      reqd,
    ) => {
  let _parameters = Uri.of_string(target) |> parseQuery |> Console.log;

  getHeader("Authorization")
  |> (
    authHeader =>
      switch (authHeader) {
      | Some(auth) =>
        OkResponse.make(
          ~respond_with_string,
          ~create_response,
          ~headers_of_list,
          reqd,
        )
      | None =>
        UnauthorizedResponse.make(
          ~respond_with_string,
          ~create_response,
          ~headers_of_list,
          reqd,
          "No Authorization header",
        )
      }
  );

  Lwt.return_unit;
};