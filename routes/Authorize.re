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
      let cookie_key = "abcdef";
      let cookie_name = "session";

      let cookie_value =
        String.concat("", [cookie_name, "=", cookie_key, " ;Max-Age=300"]);

      Console.debug(cookie_value);

      set_session(~kind=cookie_name, ~key=cookie_key, target)
      >|= (
        () =>
          OkResponse.make(
            ~respond_with_string,
            ~create_response,
            ~headers_of_list,
            ~extra_headers=[("set-cookie", cookie_value)],
            reqd,
          )
      );

    | _ =>
      UnauthorizedResponse.make(
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