open H2;

let makeRoute = reqd => {
  // Read body
  // Validate credentials
  // switch on OK credentials
  RedirectResponse.make(
    ~targetPath="http://localhost:4000",
    reqd,
  );

  Lwt.return_unit;
};