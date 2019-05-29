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

  let parameters =
    Uri.of_string(target) |> Oidc.Parameters.parse_query(~clients);

  Oidc.Parameters.(
    switch (parameters) {
    | Ok({
        response_type: ["code"],
        client,
        redirect_uri: _,
        scope: ["openid", ...rest],
        state,
        nonce,
      }) =>
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
    | Error([`Client(client), ..._]) =>
      Http.Response.Redirect.make(
        ~respond_with_string,
        ~create_response,
        ~headers_of_list,
        ~targetPath=client.redirect_uri ++ "?error=bad_parameters",
        reqd,
      )
      |> Lwt.return
    | Error([`Msg(msg), ..._]) =>
      Http.Response.Unauthorized.make(
        ~respond_with_string,
        ~create_response,
        ~headers_of_list,
        reqd,
        msg,
      )
      |> Lwt.return
    | _ =>
      Http.Response.Unauthorized.make(
        ~respond_with_string,
        ~create_response,
        ~headers_of_list,
        reqd,
        "Invalid parameters",
      )
      |> Lwt.return
    }
  );
};