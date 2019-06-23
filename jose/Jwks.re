type t = {keys: list(Jwk.t)};

let json_of_t = t => `Assoc(("keys", `List(t.keys)));
