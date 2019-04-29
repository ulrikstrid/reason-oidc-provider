type t = {
  host: string;
  session_store: SessionStorage.t Lwt.t;
  jwk: Oidc.Jwk.t;
  rsa_priv: Nocrypto.Rsa.priv;
}


let make ~host ~rsa_priv () = {
  host;
  session_store = (SessionStorage.make ());
  jwk = rsa_priv |> Nocrypto.Rsa.pub_of_priv |> Oidc.Jwk.make;
  rsa_priv = rsa_priv;
}