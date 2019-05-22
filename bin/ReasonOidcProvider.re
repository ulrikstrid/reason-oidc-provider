/** Setup loggers */
Fmt_tty.setup_std_outputs();
Logs.set_level(Some(Logs.Debug));
Logs.set_reporter(Logs_fmt.reporter());

let () = print_endline("starting...");
Nocrypto_entropy_unix.initialize();

let main = () => {
  let clients =
    Oidc.Client.[
      {
        id: "3c9fe13f-0e1f-4e0f-9be8-534ea8a32175",
        redirect_uri: "https://op.certification.openid.net:60907/authz_cb" //"http://localhost:5500/auth/cb",
      },
    ];
  let rsa_priv = Nocrypto.Rsa.generate(4048);
  let context =
    Context.make(~host="http://localhost:8080", ~rsa_priv, ~clients, ());

  H2_server.start(
    ~cert="./localhost.pem",
    ~priv_key="./localhost.key",
    ~context,
    (),
  );
};

main();