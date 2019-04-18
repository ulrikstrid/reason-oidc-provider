open Lwt.Infix;

let json_err =
  fun
  | Ok(_) as ok => ok
  | Error(err) => Error(`String(err));

let h2_handler =
    (
      ~request_handler=H2_handler.request_handler,
      ~error_handler=H2_handler.error_handler,
      tls_server,
    ) =>
  H2_lwt_unix.Server.TLS.create_connection_handler(
    ~certfile=?None,
    ~keyfile=?None,
    ~config=?None,
    ~server=tls_server,
    ~request_handler,
    ~error_handler,
  );

let http1_handler =
    (
      ~request_handler=Http1_handler.request_handler,
      ~error_handler=Http1_handler.error_handler,
      tls_server,
    ) =>
  Httpaf_lwt.Server.TLS.create_connection_handler(
    ~certfile=?None,
    ~keyfile=?None,
    ~config=?None,
    ~server=tls_server,
    ~request_handler,
    ~error_handler,
  );

/*
 let mk_connection_handler = (~tls_server, mk_context: 'a => OidcRoutes.ctx) => {
   open H2;
   let connection_handler /*: (Unix.sockaddr, Lwt_unix.file_descr) => Lwt.t(unit) */ = {
     let request_handler: (Unix.sockaddr, Reqd.t) => unit =
       (_client_address, request_descriptor) =>
         Lwt.async(() =>
           Http1Routes.makeCallback(request_descriptor, mk_context)
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

     h2_handler(~request_handler, ~error_handler, tls_server);
   };
   connection_handler;
 };
 */

let startHttpServer = (~port=8080, ~mk_context, ()) => {
  open Lwt.Infix;

  let listen_address = Unix.(ADDR_INET(inet_addr_loopback, port));

  Lwt.async(() => {
    let handler =
      Httpaf_lwt.Server.create_connection_handler(
        ~config=?None,
        ~request_handler=Http1_handler.route_handler(mk_context),
        ~error_handler=Http1_handler.error_handler,
      );

    Lwt_io.establish_server_with_client_socket(listen_address, handler)
    >>= (_server => Lwt_io.printlf("HTTP server started on port: %d", port));
  });

  let (forever, _) = Lwt.wait();
  forever;
};

let startHttpsServer = (~port=9443, ~cert, ~priv_key, ~mk_context, ()) => {
  open Lwt.Infix;

  let listen_address = Unix.(ADDR_INET(inet_addr_loopback, port));

  Lwt.async(() => {
    let handler = (client_addr, fd) => {
      Lwt.catch(
        () =>
          X509_lwt.private_of_pems(~cert, ~priv_key)
          >>= (
            certificate => {
              let config =
                Tls.Config.server(
                  ~alpn_protocols=["h2", "http/1.1"],
                  ~certificates=`Single(certificate),
                  ~ciphers=
                    List.filter(
                      Tls.Ciphersuite.ciphersuite_tls12_only,
                      Tls.Config.Ciphers.supported,
                    ),
                  (),
                );
              Tls_lwt.Unix.server_of_fd(config, fd)
              >>= (
                tls_server => {
                  switch (Tls_lwt.Unix.epoch(tls_server)) {
                  | `Error =>
                    Lwt_io.eprintf(
                      "Unable to fetch session data. Did the handshake fail?",
                    )
                  | `Ok({alpn_protocol, _}) =>
                    switch (alpn_protocol) {
                    | None => Lwt.return_unit // Unable to nogitiate a protocol
                    | Some("http/1.1") =>
                      http1_handler(
                        ~request_handler=
                          Http1_handler.route_handler(mk_context),
                        tls_server,
                        client_addr,
                        fd,
                      )
                    | Some("h2") =>
                      h2_handler(
                        ~request_handler=H2_handler.route_handler(mk_context),
                        tls_server,
                        client_addr,
                        fd,
                      )
                    | _ => assert(false) // This should never happen
                    }
                  };
                }
              );
            }
          ),
        exn => Lwt_io.eprintf("EXN: %s\n", Printexc.to_string(exn)),
      );
    };

    Lwt_io.establish_server_with_client_socket(listen_address, handler)
    >>= (_server => Lwt_io.printlf("HTTPS server started on port: %d", port));
  });

  let (forever, _) = Lwt.wait();
  forever;
};

let start = (~port=8080, ~ctx: 'a => OidcRoutes.ctx, ~cert, ~priv_key, ()) => {
  Sys.(set_signal(sigpipe, Signal_ignore));
  Lwt_main.run(
    Lwt.join([
      startHttpServer(~port, ~mk_context=ctx, ()),
      startHttpsServer(~cert, ~priv_key, ~mk_context=ctx, ()),
    ]),
  );
};