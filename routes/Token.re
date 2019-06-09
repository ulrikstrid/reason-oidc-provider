open Lwt.Infix;

let trim_leading_null = s =>
  Astring.String.trim(
    ~drop=
      fun
      | '\000' => true
      | _ => false,
    s,
  );

let int_string_of_float = f => f |> int_of_float |> string_of_int;

let code_of_body = (~get_code, ~remove_code, body) => {
  Logs.info(m => m("Body for token is %s", body));

  let key: string =
    Http.UrlencodedForm.parse(body)
    |> Http.UrlencodedForm.get_param("code")
    |> CCOpt.get_or(~default="");

  get_code(key)
  >>= (
    code => {
      remove_code(key) >|= (() => code);
    }
  );
};

let make =
    (
      ~read_body,
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~priv_key,
      ~host,
      ~get_code,
      ~remove_code,
      ~jwk: Oidc.Jwk.t,
      reqd,
    ) =>
  read_body(reqd)
  >>= code_of_body(~get_code, ~remove_code)
  >|= CCOpt.map(code => {
        open Jwt;

        let jwt_header = Oidc.Jwk.make_jwt_header(priv_key, jwk);

        let nonce_string =
          code
          |> Yojson.Basic.from_string
          |> Yojson.Basic.Util.member("nonce")
          |> Yojson.Basic.Util.to_string;

        empty_payload
        |> add_claim(iss, host)
        |> add_claim(sub, "u@s.dev")
        |> add_claim(aud, "3c9fe13f-0e1f-4e0f-9be8-534ea8a32175")
        |> add_claim(nonce, nonce_string)
        |> add_claim(iat, Unix.time() |> int_string_of_float)
        |> add_claim(exp, Unix.time() +. 3600. |> int_string_of_float)
        |> t_of_header_and_payload(jwt_header)
        |> token_of_t;
      })
  >|= (
    jwt =>
      switch (jwt) {
      | Some(id_token) =>
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
        )
      | None =>
        Http.Response.Unauthorized.make(
          ~respond_with_string,
          ~create_response,
          ~headers_of_list,
          reqd,
          {|{"error": "invalid_grant"}|},
        )
      }
  );