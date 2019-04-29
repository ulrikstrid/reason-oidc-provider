type t = {
  alg: string;
  kty: string;
  use: string;
  n: string;
  e: string;
  kid: string;
  (*x5t: string;*)
}

let make (rsa_pub: Nocrypto.Rsa.pub): t  =
  let n = rsa_pub.n |> Z.to_string |> B64.encode in
  let e =  rsa_pub.e |> Z.to_string |> B64.encode in
  let public_key: X509.public_key = `RSA rsa_pub in
  let () = Logs.debug (fun m -> m "key_id: %s" (public_key |> X509.key_id |> Cstruct.to_string |> B64.encode)) in
  let () = Logs.debug (fun m -> m "key_fingerprint: %s" (public_key |> X509.key_fingerprint |> Cstruct.to_string |> B64.encode)) in
    {
      alg = "RS256";
      kty = "RSA";
      use = "sig";
      n =  n;
      e = e;
      kid = public_key |> X509.key_id |> Cstruct.to_string |> B64.encode;
      (* x5t = public_key |> X509.key_fingerprint |> Cstruct.to_string |> B64.encode; *)
    }

let to_json t = `Assoc [
  ("alg", `String t.alg);
  ("kty", `String t.kty);
  ("use", `String t.use);
  ("n", `String t.n);
  ("e", `String t.e);
  ("kid", `String t.kid);
  (*"x5t", `String t.x5t*)
]