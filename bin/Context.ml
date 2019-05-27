type t = {
  host: string;
  session_store: SessionStorage.t Lwt.t;
  jwk: Oidc.Jwk.t;
  rsa_priv: Nocrypto.Rsa.priv;
  clients: Oidc.Client.t list;
}


let make ~host ~rsa_priv ~clients () = {
  host;
  session_store = (SessionStorage.make ());
  jwk = rsa_priv |> Nocrypto.Rsa.pub_of_priv |> Oidc.Jwk.make |> CCResult.get_exn;
  rsa_priv;
  clients;
}