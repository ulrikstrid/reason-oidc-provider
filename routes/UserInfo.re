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
        | Some(user_string) =>
          let user = Oidc.User.from_string(user_string);
          Http.Response.Json.make(
            ~httpImpl,
            ~json=Printf.sprintf({|{"sub": "%s"}|}, user.email),
            reqd,
          );
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
