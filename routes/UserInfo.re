let make = (~httpImpl, ~find_access_token, reqd) => {
  open Http.HttpImpl;
  let get_access_token_from_body = body => {
    Http.UrlencodedForm.parse(body)
    |> Http.UrlencodedForm.get_param("access_token")
    |> CCOpt.get_or(~default="invalid");
  };
  Lwt.Infix.(
    httpImpl.get_header("Authorization")
    |> CCOpt.flat_map(header =>
         header |> CCString.split(~by=" ") |> CCList.get_at_idx(1)
       )
    |> CCOpt.map_or(
         ~default=httpImpl.read_body(reqd) >|= get_access_token_from_body,
         Lwt.return,
       )
    >>= find_access_token
    >|= (
      data =>
        switch (data) {
        | Some(access_token_data) =>
          let user =
            Yojson.Basic.from_string(access_token_data)
            |> Yojson.Basic.Util.member("user")
            |> Oidc.User.from_json;

          Logs.info(m => m("access_token_data: %s", access_token_data));

          let claims =
            Oidc.Claims.(
              access_token_data
              |> Yojson.Basic.from_string
              |> Yojson.Basic.Util.member("claims")
              |> Yojson.Basic.Util.to_option(Oidc.Claims.from_json)
              |> CCOpt.map_or(~default=[], claims =>
                   claims.userinfo
                   |> CCList.map(claim =>
                        switch (claim) {
                        | Essential(c) => c
                        | NonEssential(c) => c
                        }
                      )
                   |> CCList.map(key =>
                        Oidc.User.get_value_by_key(user, key)
                        |> CCOpt.map(value => (key, `String(value)))
                      )
                   |> CCList.keep_some
                 )
            )
            |> CCList.append([("sub", `String(user.email))]);

          let json = `Assoc(claims) |> Yojson.Basic.to_string;

          Http.Response.Json.make(~httpImpl, ~json, reqd);
        | None =>
          Http.Response.Unauthorized.make(
            ~httpImpl,
            {|error="invalid_token"|},
            reqd,
          )
        }
    )
  );
};