let trim_leading_null = s =>
  Astring.String.trim(
    ~drop=
      fun
      | '\000' => true
      | _ => false,
    s,
  );

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
          Http.UrlencodedForm.parse(body)
          |> Http.UrlencodedForm.get_param("code")
          |> (
            fun
            | Some(code) => {
                let decoded_code =
                  Base64.decode(
                    ~pad=false,
                    ~alphabet=Base64.uri_safe_alphabet,
                    code,
                  )
                  |> CCResult.map(Cstruct.of_string)
                  |> CCResult.map(Nocrypto.Rsa.decrypt(~key=priv_key))
                  |> CCResult.map(Cstruct.to_string)
                  |> CCResult.map(trim_leading_null);

                switch (decoded_code) {
                | Ok(c) =>
                  Logs.info(m => m("Decoded code: %s", c));
                  c
                  |> Yojson.Basic.from_string
                  |> Yojson.Basic.Util.member("nonce")
                  |> Yojson.Basic.Util.to_string
                  |> (nonce => Jwt.add_claim(Jwt.nonce, nonce, jwt));
                | Error(`Msg(message)) =>
                  Logs.warn(m => m("%s", message));
                  jwt;
                };
              }
            | None => {
                Logs.info(m => m("No code in body"));
                jwt;
              }
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