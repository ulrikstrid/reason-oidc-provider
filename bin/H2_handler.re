open H2;

let error_handler = (_client_address, ~request as _=?, _error, start_response) => {
  let response_body = start_response(Headers.empty);
  Body.close_writer(response_body);
};

let route_handler: (Context.t, Unix.sockaddr, Reqd.t) => unit =
  (context, _client_address, request_descriptor) => {
    let start = Unix.gettimeofday();
    Lwt.async(() => {
      let {Request.target, meth, headers, scheme: _} =
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
        ~get_header=Headers.get(headers),
        ~create_response,
        ~respond_with_string=Reqd.respond_with_string,
        ~headers_of_list=Headers.of_list,
        ~read_body,
        ~context,
        request_descriptor,
      )
      |> Lwt.map(() => {
           let stop = Unix.gettimeofday();
           Logs.info(m =>
             m(
               "H2: %s request to %s finished in %fms",
               Http.Method.to_string(meth),
               target,
               (stop -. start) *. 1000.,
             )
           );
         });
    });
  };