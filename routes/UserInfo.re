let make =
    (
      ~meth,
      ~read_body,
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~get_header,
      ~find_access_token,
      reqd,
    ) => {
  let get_access_token_from_body = body => {
    Http.UrlencodedForm.parse(body)
    |> Http.UrlencodedForm.get_param("access_token")
    |> CCOpt.get_or(~default="invalid");
  };
  Lwt.Infix.(
    get_header("Authorization")
    |> CCOpt.flat_map(header =>
         header |> CCString.split(~by=" ") |> CCList.get_at_idx(1)
       )
    |> CCOpt.map_or(
         ~default=read_body(reqd) >|= get_access_token_from_body,
         Lwt.return,
       )
    >>= find_access_token
    >|= (
      data =>
        switch (data) {
        | Some(user_string) =>
          let user = Oidc.User.from_string(user_string);
          Http.Response.Json.make(
            ~respond_with_string,
            ~create_response,
            ~headers_of_list,
            ~json=Printf.sprintf({|{"sub": "%s"}|}, user.email),
            reqd,
          );
        | None =>
          Http.Response.Unauthorized.make(
            ~respond_with_string,
            ~create_response,
            ~headers_of_list,
            {|error="invalid_token"|},
            reqd,
          )
        }
    )
  );
};