type t = {
  alg: string;
  kty: string;
  use: string;
  n: string;
  e: string;
  kid: string;
  x5t: string;
}

val empty : t

val make : Nocrypto.Rsa.pub -> (t, [ `Msg of string]) result   

val to_json : t -> Yojson.Basic.t
