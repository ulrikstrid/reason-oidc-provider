let makeRoute =
    (
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~read_body,
      ~parameters: Oidc.Parameters.t,
      reqd,
    ) => {
  Lwt.Infix.
    // Read body
    (
      read_body(reqd)
      >|= (
        body => {
          // Validate credentials
          // switch on OK credentials
          Http.Response.Redirect.make(
            ~respond_with_string,
            ~create_response,
            ~headers_of_list,
            ~targetPath=
              parameters.redirect_uri
              ++ "?code=SplxlOBeZQQYbYS6WxSbIA&state="
              ++ CCOpt.get_or(~default="", parameters.state),
            reqd,
          );
        }
      )
    );
};