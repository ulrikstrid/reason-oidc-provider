open H2;
open Lwt.Infix;

let json_err =
  fun
  | Ok(_) as ok => ok
  | Error(err) => Error(`String(err));

let mk_connection_handler =
    (~cert, ~priv_key, mk_context: 'a => OidcRoutes.ctx) => {
  let connection_handler:
    (~server: 'b=?, Unix.sockaddr, Lwt_unix.file_descr) => Lwt.t(unit) = {
    let request_handler: (Unix.sockaddr, Reqd.t) => unit =
      (_client_address, request_descriptor) =>
        Lwt.async(() =>
          OidcRoutes.makeCallback(request_descriptor, mk_context)
        );

    let error_handler:
      (
        Unix.sockaddr,
        ~request: H2.Request.t=?,
        _,
        Headers.t => Body.t([ | `write])
      ) =>
      unit =
      (_client_address, ~request as _=?, error, start_response) => {
        let response_body = start_response(Headers.empty);

        switch (error) {
        | `Exn(exn) =>
          Body.write_string(response_body, Printexc.to_string(exn));
          Body.write_string(response_body, "\n");

        | #Status.standard as error =>
          Body.write_string(
            response_body,
            Status.default_reason_phrase(error),
          )
        };

        Body.close_writer(response_body);
      };

    H2_lwt_unix.Server.TLS.create_connection_handler(
      ~certfile=cert,
      ~keyfile=priv_key,
      ~config=?None,
      ~request_handler,
      ~error_handler,
    );
  };
  connection_handler;
};

let start = (~port=8080, ~ctx: 'a => OidcRoutes.ctx, ~cert, ~priv_key, ()) => {
  let listen_address = Unix.(ADDR_INET(inet_addr_loopback, port));

  print_endline(cert);
  print_endline(priv_key);

  Lwt.async(() =>
    Lwt_io.establish_server_with_client_socket(
      listen_address,
      mk_connection_handler(~cert, ~priv_key, ctx),
    )
    >>= (_server => Lwt_io.printlf("Server started on port: %d", port))
  );

  let (forever, _) = Lwt.wait();

  Lwt_main.run(forever);
};