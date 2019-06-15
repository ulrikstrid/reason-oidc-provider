let make = (~httpImpl, ~host, reqd) => {
  let json =
    Printf.sprintf(
      {|{
  "issuer": "%s",
  "authorization_endpoint": "%s/authorize",
  "token_endpoint": "%s/token",
  "userinfo_endpoint": "%s/userinfo",
  "jwks_uri": "%s/.well-known/jwks.json",
  "scopes_supported": [
    "openid", "email"
  ],
  "response_types_supported": [
    "code"
  ],
  "id_token_signing_alg_values_supported": [
    "RS256"
  ],
  "token_endpoint_auth_methods_supported": [
    "client_secret_basic"
  ],
  "claims_supported": [
    "email",
    "name"
  ]
}|},
      host,
      host,
      host,
      host,
      host,
    );

  Http.Response.Json.make(~httpImpl, ~json, reqd);
  Lwt.return_unit;
};