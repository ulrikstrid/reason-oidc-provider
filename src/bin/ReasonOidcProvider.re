/** Setup loggers */
Fmt_tty.setup_std_outputs();
Logs.set_level(Some(Logs.Info));
Logs.set_reporter(Logs_fmt.reporter());

Nocrypto_entropy_unix.initialize();

let users =
  Oidc.User.[
    {
      email: "ulrik.strid@outlook.com",
      name: "Ulrik Strid",
      password: "strid",
    },
  ];

let main = (settings: Settings.t) => {
  let clients = settings.clients;
  let rsa_priv = Nocrypto.Rsa.generate(2048);
  let context =
    Context.make(
      ~host=settings.provider.host,
      ~rsa_priv,
      ~clients,
      ~users,
      (),
    );

  H2_server.start(
    ~cert="./localhost.pem",
    ~priv_key="./localhost.key",
    ~context,
    (),
  );
};

Settings.get_from_env()
|> (
  fun
  | Ok(settings) => main(settings)
  | Error(`Msg(msg)) => print_endline(msg)
);
