let make = (~httpImpl, ~jwk, reqd) => {
  let jwk_string = jwk |> Oidc.Jwk.to_json |> Yojson.Basic.to_string;
  let json = Printf.sprintf({j|{"keys": [%s]}|j}, jwk_string);

  Http.Response.Json.make(~httpImpl, ~json, reqd);

  Lwt.return_unit;
};
