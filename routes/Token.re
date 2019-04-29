let make =
    (
      ~read_body,
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~priv_key,
      ~host,
      reqd,
    ) => {
  let jwt_header =
    Jwt.header_of_algorithm_and_typ(Jwt.RS256(Some(priv_key)), None);

  let int_string_of_float = f =>
    string_of_float(f) |> CCString.filter(c => c != '.');

  let id_token =
    Jwt.(
      empty_payload
      |> add_claim(iss, host)
      |> add_claim(sub, "u@s.dev")
      |> add_claim(aud, "3c9fe13f-0e1f-4e0f-9be8-534ea8a32175")
      |> add_claim(iat, Unix.time() |> int_string_of_float)
      |> add_claim(exp, Unix.time() +. 3600. |> int_string_of_float)
      |> t_of_header_and_payload(jwt_header)
      |> token_of_t
    );
  Logs.debug(m => m("JWT: %s", id_token));
  Lwt.Infix.(
    read_body(reqd)
    >|= (body => Logs.info(m => m("Body for token is %s", body)))
    >|= (
      () =>
        Http.Response.Json.make(
          ~respond_with_string,
          ~create_response,
          ~headers_of_list,
          ~json=
            Printf.sprintf(
              {|{
              "id_token": "%s",
              "expires_in": 3600,
              "token_type": "Bearer"
          }|},
              id_token,
            ),
          reqd,
        )
    )
  );
};