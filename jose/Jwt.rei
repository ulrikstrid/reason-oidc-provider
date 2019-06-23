type header;

let make_header: Jwk.t => header;

type payload = Yojson.Basic.t;
type claim = (string, Yojson.Basic.t);

let empty_payload: payload;

type signature;

type t;

let add_claim: (string, Yojson.Basic.t, payload) => payload;

let sign: (header, Nocrypto.Rsa.priv, payload) => t;

let to_string: t => string;

let verify: (~jwks: list(Jwk.t), t) => result(t, [ | `Msg(string)]);
