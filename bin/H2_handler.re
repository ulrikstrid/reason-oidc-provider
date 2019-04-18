open H2;

let request_handler: (Unix.sockaddr, Reqd.t) => unit =
  (_client_address, request_descriptior) => {
    let request = Reqd.request(request_descriptior);
    let response_content_type =
      switch (Headers.get(request.headers, "Content-Type")) {
      | Some(request_content_type) => request_content_type
      | None => "application/octet-stream"
      };
    let response_body = "";
    let response =
      Response.create(
        ~headers=Headers.of_list([("content-type", response_content_type)]),
        `OK,
      );
    Reqd.respond_with_string(request_descriptior, response, response_body);
  };

let error_handler = (_client_address, ~request=?, _error, start_response) => {
  let response_body = start_response(Headers.empty);
  Body.close_writer(response_body);
};

let route_handler: ('a => OidcRoutes.ctx, Unix.sockaddr, Reqd.t) => unit =
  (mk_context, _client_address, request_descriptor) => {
    Lwt.async(() => {
      open H2;

      let {Request.target, meth, headers} as request =
        Reqd.request(request_descriptor);

      let req_uri = target |> Uri.of_string;
      let req_path = Uri.path(req_uri);
      let path_parts = Str.(split(regexp("/"), req_path));

      let create_response = (~headers, status) =>
        Response.create(~headers, status);

      OidcRoutes.makeCallback(
        ~target,
        ~method=meth,
        ~getHeader=Headers.get(headers),
        ~create_response,
        ~headers_of_list=Headers.of_list,
        ~respond_with_string=Reqd.respond_with_string,
        ~headers,
        request_descriptor,
        mk_context,
      );
    });
  };