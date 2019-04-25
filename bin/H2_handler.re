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

let route_handler: (Context.t, Unix.sockaddr, Reqd.t) => unit =
  (context, _client_address, request_descriptor) => {
    Lwt.async(() => {
      open H2;

      let {Request.target, meth, headers} as request =
        Reqd.request(request_descriptor);

      let content_length =
        Headers.get(headers, "content-length")
        |> CCOpt.map_or(~default=128, int_of_string);

      let read_body =
        Http.Body.read(
          ~content_length,
          ~get_request_body=Reqd.request_body,
          ~schedule_read=Body.schedule_read,
        );

      let create_response = (~headers, status) =>
        Response.create(~headers, status);

      OidcRoutes.makeCallback(
        ~target,
        ~method=meth,
        ~getHeader=Headers.get(headers),
        ~create_response,
        ~headers_of_list=Headers.of_list,
        ~respond_with_string=Reqd.respond_with_string,
        ~read_body,
        ~headers,
        ~context,
        request_descriptor,
      );
    });
  };