let make =
    (~respond_with_string, ~create_response, ~headers_of_list, ~jwk, reqd) => {
  let jwk_string = jwk |> Oidc.Jwk.to_json |> Yojson.Basic.to_string;
  let json = Printf.sprintf({j|{"keys": [%s]}|j}, jwk_string);

  Http.Response.Json.make(
    ~respond_with_string,
    ~create_response,
    ~headers_of_list,
    ~json,
    reqd,
  );

  Lwt.return_unit;
};
