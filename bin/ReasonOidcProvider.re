/** Setup loggers */
Fmt_tty.setup_std_outputs();
Logs.set_level(Some(Logs.Debug));
Logs.set_reporter(Logs_fmt.reporter());

let main = () => {
  let context = Context.make(~host="http://localhost:8080", ());

  H2_server.start(
    ~cert="./localhost.pem",
    ~priv_key="./localhost.key",
    ~context,
    (),
  );
};

main();