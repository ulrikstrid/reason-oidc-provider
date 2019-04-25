let makeRoute =
    (
      ~set_session,
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~getHeader,
      target,
      reqd,
    ) => {
  open Lwt.Infix;
  open Http;

  let parameters = Uri.of_string(target) |> Oidc.Parameters.parseQuery;

  Oidc.Parameters.(
    switch (parameters) {
    | {
        response_type: ["code"],
        client_id: "client_id",
        redirect_uri: "http://localhost:5500/auth/cb",
        scope,
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
            ~targetPath="http://localhost:8080/interaction",
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