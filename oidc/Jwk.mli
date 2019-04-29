type t = {
  alg: string;
  kty: string;
  use: string;
  n: string;
  e: string;
  kid: string;
  x5t: string;
}

val make : Nocrypto.Rsa.pub -> t 

val to_json : t -> Yojson.Basic.t