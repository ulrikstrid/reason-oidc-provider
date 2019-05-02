let makeRoute =
    (
      ~set_session,
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~get_header,
      ~redirectPath,
      target,
      reqd,
    ) => {
  open Lwt.Infix;
  open Http;

  let parameters = Uri.of_string(target) |> Oidc.Parameters.parseQuery;

  Console.log(parameters.response_type);
  Console.log(parameters);

  Oidc.Parameters.(
    switch (parameters) {
    | {
        response_type: ["code"],
        client_id: "3c9fe13f-0e1f-4e0f-9be8-534ea8a32175",
        redirect_uri: "http://localhost:5500/auth/cb",
        scope: ["openid", ...rest],
        state,
        nonce,
      } =>
      let cookie_key =
        Uuidm.v4_gen(Random.State.make_self_init(), ()) |> Uuidm.to_string;
      let cookie_name = "session";
      let cookie_value =
        String.concat("", [cookie_name, "=", cookie_key, " ;Max-Age=300"]);

      set_session(~kind=cookie_name, ~key=cookie_key, target)
      >|= (
        () =>
          Http.Response.Redirect.make(
            ~respond_with_string,
            ~create_response,
            ~headers_of_list,
            ~extra_headers=[("set-cookie", cookie_value)],
            ~targetPath=redirectPath,
            reqd,
          )
      );
    | _ =>
      Http.Response.Unauthorized.make(
        ~respond_with_string,
        ~create_response,
        ~headers_of_list,
        reqd,
        "No Authorization header",
      );

      Lwt.return_unit;
    }
  );
};