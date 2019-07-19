open TestFramework;

describe("JOSE - Jwk", u => {
  Nocrypto_entropy_unix.initialize();
  let priv_key = Nocrypto.Rsa.generate(~g=Nocrypto.Rng.generator^, 2048);
  let pub_key = priv_key |> Nocrypto.Rsa.pub_of_priv;

  u.test("to_json", ({expect}) => {
    let jwk = pub_key |> Jose.Jwk.make |> CCResult.get_exn;

    expect.fn(() => Jose.Jwk.to_json(jwk)).not.toThrow();
  });

  u.test("make_jwt_header", ({expect}) => {
    let jwk = pub_key |> Jose.Jwk.make |> CCResult.get_exn;

    expect.fn(() => Jose.Jwt.make_header(jwk)).not.toThrow();
  });

  /* TODO: Make a smaller test */
  u.test("Jwk.make", ({expect}) => {
    let jwk = pub_key |> Jose.Jwk.make |> CCResult.get_exn;
    let jwt_header: Jose.Jwt.header = Jose.Jwt.make_header(jwk);

    let jwt =
      Jose.Jwt.(
        empty_payload
        |> add_claim("iss", `String("http://localhost:8080"))
        |> add_claim("sub", `String("u@s.dev"))
        |> add_claim("aud", `String("3c9fe13f-0e1f-4e0f-9be8-534ea8a32175"))
        |> add_claim("iat", `Int(Unix.time() |> int_of_float))
        |> add_claim("exp", `Int(Unix.time() +. 3600. |> int_of_float))
        |> sign(jwt_header, priv_key)
      );

    let jwks = [jwk];

    expect.result(CCResult.map(Jose.Jwt.verify(~jwks), jwt)).toBeOk();
  });
});

/*
describe("JOSE - JWT", ({test, _}) => {
  test("Decode plaintext", ({expect}) => {
    let token = "eyJhbGciOiJub25lIn0.eyJoZWxsbyI6ICJ3b3JsZCJ9.";

    let t = Jose.Jwt.from_string(token);

    expect.result(t).toBeOk();
  });

  test("Encode plaintext", ({expect}) => {
    let token = "eyJhbGciOiJub25lIn0.eyJoZWxsbyI6ICJ3b3JsZCJ9.";

    `Assoc([("hello", `String("world"))])
    |> Jose.Jwt.

    let t = Jose.Jwt.from_string(token);

    expect.result(t).toBeOk();
  });
});
*/
