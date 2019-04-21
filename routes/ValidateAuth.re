open H2;

let makeRoute =
    (~respond_with_string, ~create_response, ~headers_of_list, reqd) => {
  // Read body
  // Validate credentials
  // switch on OK credentials
  RedirectResponse.make(
    ~respond_with_string,
    ~create_response,
    ~headers_of_list,
    ~targetPath="http://localhost:8080",
    reqd,
  );

  Lwt.return_unit;
};