let makeRoute =
    (
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~read_body,
      ~oidc_state,
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
              "http://localhost:5500/auth/cb?code=SplxlOBeZQQYbYS6WxSbIA&state="
              ++ oidc_state,
            reqd,
          );
        }
      )
    );
};