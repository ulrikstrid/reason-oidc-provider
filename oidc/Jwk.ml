type t = {
  alg: string;
  kty: string;
  use: string;
  n: string;
  e: string;
  kid: string;
  x5t: string;
}

let empty = {
  alg = "";
  kty = "";
  use = "";
  n = "";
  e = "";
  kid = "";
  x5t = "";
}

let trim_leading_null s =
  Astring.String.trim ~drop:(function '\000' -> true | _ -> false) s

let make (rsa_pub: Nocrypto.Rsa.pub): (t, [ `Msg of string]) result  =
  let n = (CCString.rev (Z.to_bits rsa_pub.n))
    |> trim_leading_null
    |> Base64.encode ~pad:false ~alphabet:Base64.uri_safe_alphabet in
  let e = (CCString.rev (Z.to_bits rsa_pub.e))
    |> trim_leading_null
    |> Base64.encode ~alphabet:Base64.uri_safe_alphabet in
  let public_key: X509.public_key = `RSA rsa_pub in
  let kid = public_key
    |> X509.key_id
    |> Cstruct.to_string
    |> Base64.encode ~alphabet:Base64.uri_safe_alphabet in
  let x5t = public_key
    |> X509.key_fingerprint
    |> Cstruct.to_string
    |> Base64.encode ~alphabet:Base64.uri_safe_alphabet in
    match (n, e, kid, x5t) with
    | (Ok n, Ok e, Ok kid, Ok x5t) ->
      Ok {
        alg = "RS256";
        kty = "RSA";
        use = "sig";
        n =  n;
        e = e;
        kid = kid;
        x5t = x5t;
      }
    | (Error (`Msg m), _, _, _) -> Error (`Msg ("n " ^ m))
    | (_, Error (`Msg m), _, _) -> Error (`Msg ("e " ^ m))
    | (_, _, Error (`Msg m), _) -> Error (`Msg ("kid " ^ m))
    | (_, _, _, Error (`Msg m)) -> Error (`Msg ("x5t " ^ m))

let to_json t = `Assoc [
  ("alg", `String t.alg);
  ("kty", `String t.kty);
  ("use", `String t.use);
  ("n", `String t.n);
  ("e", `String t.e);
  ("kid", `String t.kid);
  "x5t", `String t.x5t
]

let make_jwt_header priv_key t = Jwt.make_header 
  ~alg:(Jwt.RS256 (Some priv_key))
  ~typ:"JWT"
  ~kid:t.kid
  ()
    