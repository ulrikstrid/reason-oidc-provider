let makeRoute =
    (
      ~set_session,
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~get_header,
      ~redirectPath,
      ~clients,
      target,
      reqd,
    ) => {
  open Lwt.Infix;
  open Http;

  let parameters = Uri.of_string(target) |> Oidc.Parameters.parseQuery;
  let client = Oidc.Parameters.get_client(~clients, parameters);

  Oidc.Parameters.(
    switch (parameters, client) {
    | (
        {
          response_type: ["code"],
          client_id: _,
          redirect_uri: _,
          scope: ["openid", ...rest],
          state,
          nonce,
        },
        Some(client),
      ) =>
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
        "Bad request",
      );

      Lwt.return_unit;
    }
  );
};