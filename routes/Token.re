let make =
    (
      ~read_body,
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~priv_key,
      ~host,
      ~jwk: Oidc.Jwk.t,
      reqd,
    ) => {
  open Lwt.Infix;
  let jwt_header = Oidc.Jwk.make_jwt_header(priv_key, jwk);

  let int_string_of_float = f => f |> int_of_float |> string_of_int;

  let jwt =
    Jwt.(
      empty_payload
      |> add_claim(iss, host)
      |> add_claim(sub, "u@s.dev")
      |> add_claim(aud, "3c9fe13f-0e1f-4e0f-9be8-534ea8a32175")
      |> add_claim(iat, Unix.time() |> int_string_of_float)
      |> add_claim(exp, Unix.time() +. 3600. |> int_string_of_float)
    );

  Lwt.Infix.(
    read_body(reqd)
    >|= (
      body => {
        Logs.info(m => m("Body for token is %s", body));

        let id_token =
          Uri.get_query_param(Uri.of_string(body), "code")
          |> (
            fun
            | Some(code) =>
              Cstruct.of_string(code)
              |> Nocrypto.Rsa.decrypt(~key=priv_key)
              |> Cstruct.to_string
              |> Yojson.Basic.from_string
              |> Yojson.Basic.Util.member("nonce")
              |> Yojson.Basic.Util.to_string
              |> (nonce => Jwt.add_claim(Jwt.nonce, nonce, jwt))
            | None => jwt
          )
          |> Jwt.t_of_header_and_payload(jwt_header)
          |> Jwt.token_of_t;

        Logs.info(m => m("JWT: %s", id_token));

        Http.Response.Json.make(
          ~respond_with_string,
          ~create_response,
          ~headers_of_list,
          ~json=
            Printf.sprintf(
              {|{
                "id_token": "%s",
                "access_token": "%s",
                "expires_in": 3600,
                "token_type": "Bearer"
              }|},
              id_token,
              "access_token"
              |> Base64.encode_exn(~alphabet=Base64.uri_safe_alphabet),
            ),
          reqd,
        );
      }
    )
  );
};