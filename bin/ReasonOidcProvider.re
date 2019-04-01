/** Setup loggers */
Fmt_tty.setup_std_outputs();
Logs.set_level(Some(Logs.Debug));
Logs.set_reporter(Logs_fmt.reporter());

let main = () =>
  H2_server.start(
    ~port=3000,
    ~cert="./localhost.crt",
    ~priv_key="./localhost.key",
    ~ctx=_req => (),
    (),
  );

main();