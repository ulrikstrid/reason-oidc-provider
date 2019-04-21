type ctx = unit;

let makeCallback =
    (
      ~target,
      ~method,
      ~getHeader,
      ~create_response,
      ~respond_with_string,
      ~headers_of_list,
      ~headers,
      reqd,
      mk_context,
    ) => {
  let req_uri = target |> Uri.of_string;
  let req_path = Uri.path(req_uri);
  let path_parts = Str.(split(regexp("/"), req_path));

  let _ctx = mk_context();

  switch (method, path_parts) {
  | (`GET, ["authorize"]) =>
    Routes.Authorize.makeRoute(
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~getHeader,
      target,
      reqd,
    )
  | (`GET, ["interaction"]) =>
    Routes.LoginForm.makeRoute(
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      reqd,
    )
  | (`POST, ["interaction"]) =>
    Routes.ValidateAuth.makeRoute(
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      reqd,
    )
  | _ =>
    Routes.OkResponse.make(
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      reqd,
    );
    Lwt.return_unit;
  };
};