let make =
    (~respond_with_string, ~create_response, ~headers_of_list, ~host, reqd) => {
  let json =
    Printf.sprintf(
      {|{
  "issuer": "%s",
  "authorization_endpoint": "%s/authorize",
  "token_endpoint": "%s/token",
  "userinfo_endpoint": "%s/userinfo",
  "jwks_uri": "%s/.well-known/jwks.json",
  "scopes_supported": [
    "user"
  ],
  "response_types_supported": [
    "code"
  ],
  "token_endpoint_auth_methods_supported": [
    "client_secret_basic"
  ]
}|},
      host,
      host,
      host,
      host,
      host,
    );

  Http.Response.Json.make(
    ~respond_with_string,
    ~create_response,
    ~headers_of_list,
    ~json,
    reqd,
  );
  Lwt.return_unit;
};