open H2;

let makeRoute =
    (
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~read_body,
      reqd,
    ) => {
  open Lwt.Infix;
  // Validate credentials
  // switch on OK credentials
  RedirectResponse.make(
    ~respond_with_string,
    ~create_response,
    ~headers_of_list,
    ~targetPath="http://localhost:8080",
    reqd,
  );

  // Read body
  read_body(reqd) >|= print_endline;
};