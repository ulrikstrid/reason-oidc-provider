let makeRoute =
    (
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~read_body,
      ~parameters: Oidc.Parameters.t,
      ~hash_key,
      reqd,
    ) => {
  read_body(reqd)
  |> Lwt.map(_body => {
       let code =
         Printf.sprintf({|{"nonce": "%s"}|}, parameters.nonce)
         |> Cstruct.of_string
         |> Nocrypto.Rsa.encrypt(~key=hash_key |> Nocrypto.Rsa.pub_of_priv)
         |> Cstruct.to_string
         |> Base64.encode_exn(~pad=false, ~alphabet=Base64.uri_safe_alphabet);

       Logs.info(m => m("Code: %s", code));

       // Validate credentials
       // switch on OK credentials
       Http.Response.Redirect.make(
         ~respond_with_string,
         ~create_response,
         ~headers_of_list,
         ~targetPath=
           Printf.sprintf(
             "%s?code=%s%s",
             parameters.redirect_uri,
             code,
             CCOpt.map_or(
               ~default="",
               state => "&state=" ++ state,
               parameters.state,
             ),
           ),
         reqd,
       );
     });
};