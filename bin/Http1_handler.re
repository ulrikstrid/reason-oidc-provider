open Httpaf;

let redirect_handler: (Unix.sockaddr, Reqd.t) => unit =
  (_client_address, request_descriptior) => {
    let response =
      Response.create(
        ~headers=
          Headers.of_list([
            ("Location", "https://localhost:9443"),
            ("Connection", "close"),
          ]),
        `Moved_permanently,
      );
    Reqd.respond_with_string(request_descriptior, response, "");
  };

let redirect_error_handler =
    (_client_address, ~request=?, _error, start_response) => {
  let response_body = start_response(Headers.empty);
  Body.close_writer(response_body);
};

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
        ~headers=
          Headers.of_list([
            ("content-type", response_content_type),
            (
              "content-length",
              String.length(response_body) |> string_of_int,
            ),
          ]),
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
      open Httpaf;

      let {Request.target, meth, headers} as request =
        Reqd.request(request_descriptor);

      let content_length =
        Headers.get(headers, "content-length")
        |> CCOpt.map_or(~default=128, int_of_string);

      let read_body =
        HttpBody.read(
          ~content_length,
          ~get_request_body=Reqd.request_body,
          ~schedule_read=Body.schedule_read,
        );

      let create_response = (~headers, status) =>
        Response.create(~headers, ~reason=?None, ~version=?None, status);

      OidcRoutes.makeCallback(
        ~target,
        ~method=meth,
        ~getHeader=Headers.get(headers),
        ~headers_of_list=Headers.of_list,
        ~create_response,
        ~headers,
        ~respond_with_string=Reqd.respond_with_string,
        ~read_body,
        ~context,
        request_descriptor,
      );
    });
  };