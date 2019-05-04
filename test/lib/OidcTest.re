open TestFramework;

describe("Jwk", u => {
  Nocrypto_entropy_unix.initialize();
  let priv_key = Nocrypto.Rsa.generate(~g=Nocrypto.Rng.generator^, 4048);
  let pub_key = priv_key |> Nocrypto.Rsa.pub_of_priv;

  // TODO: Make a smaller test
  u.test("Jwk.make", ({expect}) => {
    let jwk = pub_key |> Oidc.Jwk.make;
    let jwt_header: Jwt.header = Oidc.Jwk.make_jwt_header(priv_key, jwk);

    let int_string_of_float = f => f |> int_of_float |> string_of_int;

    let jwt =
      Jwt.(
        empty_payload
        |> add_claim(iss, "http://localhost:8080")
        |> add_claim(sub, "u@s.dev")
        |> add_claim(aud, "3c9fe13f-0e1f-4e0f-9be8-534ea8a32175")
        |> add_claim(iat, Unix.time() |> int_string_of_float)
        |> add_claim(exp, Unix.time() +. 3600. |> int_string_of_float)
        |> t_of_header_and_payload(jwt_header)
      );

    expect.bool(
      Jwt.verify(~alg="RS256", ~jwks=`List([Oidc.Jwk.to_json(jwk)]), jwt),
    ).
      toBe(
      true,
    );
  });
});

describe("Parameters", u => {
  let validQuery = "http://localhost:3000/authorize?response_type=code&client_id=s6BhdRkqt3&redirect_uri=https%3A%2F%2Fclient.example.org%2Fcb&scope=openid%20profile&state=af0ifjsldkj&nonce=n-0S6_WzA2Mj";
  let validParameters =
    Oidc.Parameters.parseQuery(Uri.of_string(validQuery));

  u.test("parseQuery", ({expect}) => {
    expect.string(validParameters.client_id).toEqual("s6BhdRkqt3");
    expect.string(validParameters.redirect_uri).toEqual(
      "https://client.example.org/cb",
    );
    expect.string(validParameters.nonce).toEqual("n-0S6_WzA2Mj");
  });

  u.test("get_client", ({expect}) => {
    let client: Oidc.Client.t = {
      id: "s6BhdRkqt3",
      redirect_uri: "https://client.example.org/cb",
    };
    let clients = [client];

    expect.option(Oidc.Parameters.get_client(~clients, validParameters)).toBe(
      Some(client),
    );
  });
});