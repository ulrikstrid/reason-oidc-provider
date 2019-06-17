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

let code_of_body = (~find_code, ~remove_code, ~remove_access_token, body) => {
  Logs.app(m => m("Body for token is %s", body));

  let key: string =
    Http.UrlencodedForm.parse(body)
    |> Http.UrlencodedForm.get_param("code")
    |> CCOpt.get_or(~default="");

  find_code(key)
  >>= (
    code_data => {
      remove_code(key)
      >>= (
        () =>
          switch (code_data) {
          | Some(value) => Lwt.return(Some((key, value)))
          | None =>
            remove_access_token(
              Base64.encode_exn(~alphabet=Base64.uri_safe_alphabet, key),
            )
            >|= (() => None)
          }
      );
    }
  );
};

let make =
    (
      ~httpImpl,
      ~priv_key,
      ~host,
      ~find_code,
      ~remove_code,
      ~set_access_token,
      ~remove_access_token,
      ~jwk: Oidc.Jwk.t,
      reqd,
    ) => {
  let unauthorized_response = reqd => {
    Http.Response.Unauthorized.make(
      ~httpImpl,
      {|{"error": "invalid_grant"}|},
      reqd,
    );
  };

  httpImpl.read_body(reqd)
  >>= code_of_body(~find_code, ~remove_code, ~remove_access_token)
  >>= CCOpt.map_or(
        ~default=Lwt.return(unauthorized_response),
        ((code, code_data)) => {
          open Jwt;

          let jwt_header = Oidc.Jwk.make_jwt_header(priv_key, jwk);

          Logs.app(m => m("code_data: %s", code_data));

          let auth_json = Yojson.Basic.from_string(code_data);

          let nonce_string =
            auth_json
            |> Yojson.Basic.Util.member("nonce")
            |> Yojson.Basic.Util.to_string;

          let user =
            auth_json
            |> Yojson.Basic.Util.member("user")
            |> Oidc.User.from_json;

          let scopes =
            auth_json
            |> Yojson.Basic.Util.member("scope")
            |> Yojson.Basic.Util.to_list
            |> CCList.map(Yojson.Basic.Util.to_string)
            |> CCList.filter(s => s != "openid");

          Logs.app(m => m("scopes: %s", scopes |> CCString.concat(", ")));

          let auth_time =
            auth_json
            |> Yojson.Basic.Util.member("auth_time")
            |> Yojson.Basic.Util.to_float
            |> int_of_float
            |> string_of_int;

          let claims =
            Oidc.Claims.(
              auth_json
              |> Yojson.Basic.Util.member("claims")
              |> Yojson.Basic.Util.to_option(Oidc.Claims.from_json)
              |> CCOpt.map(claims => claims.id_token)
              |> CCOpt.get_or(~default=[])
              |> CCList.append(
                   scopes
                   |> CCList.map(Oidc.Scopes.string_to_scope)
                   |> CCList.flat_map(Oidc.Scopes.scope_to_claims),
                 )
              |> CCList.map(claim =>
                   switch (claim) {
                   | Essential(c) => c
                   | NonEssential(c) => c
                   }
                 )
              |> CCList.map(Oidc.User.get_value_by_claim(user))
              |> CCList.keep_some
            );

          Logs.info(m =>
            m("id_token_claims: %s", Yojson.Basic.to_string(`Assoc(claims)))
          );

          let id_token =
            payload_of_json(`Assoc(claims))
            |> add_claim(claim("auth_time"), auth_time)
            |> add_claim(iss, host)
            |> add_claim(sub, user.email)
            |> add_claim(aud, "3c9fe13f-0e1f-4e0f-9be8-534ea8a32175")
            |> add_claim(nonce, nonce_string)
            |> add_claim(iat, Unix.time() |> int_string_of_float)
            |> add_claim(exp, Unix.time() +. 3600. |> int_string_of_float)
            |> t_of_header_and_payload(jwt_header)
            |> token_of_t;

          let access_token =
            code |> Base64.encode_exn(~alphabet=Base64.uri_safe_alphabet);

          let access_token_data =
            `Assoc([
              ("user", Oidc.User.to_json(user)),
              ("claims", auth_json |> Yojson.Basic.Util.member("claims")),
              ("scope", auth_json |> Yojson.Basic.Util.member("scope")),
            ])
            |> Yojson.Basic.to_string;

          set_access_token(~key=access_token, access_token_data)
          >|= (
            () =>
              Http.Response.Json.make(
                ~httpImpl,
                ~json=
                  Printf.sprintf(
                    {|{
                "id_token": "%s",
                "access_token": "%s",
                "expires_in": 3600,
                "token_type": "Bearer"
              }|},
                    id_token,
                    access_token,
                  ),
              )
          );
        },
      )
  >|= (response => response(reqd));
};