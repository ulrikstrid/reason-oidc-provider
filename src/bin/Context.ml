type t = {
  host: string;
  session_store: SessionStorage.t Lwt.t;
  jwk: Jose.Jwk.t;
  rsa_priv: Nocrypto.Rsa.priv;
  clients: Oidc.Client.t list;
  find_user: string -> Oidc.User.t option;
}


let make ~host ~rsa_priv ~clients ~users () = 
  let find_user = Oidc.User.find_user ~users in
  {
    host;
    session_store = SessionStorage.make ();
    jwk = rsa_priv |> Nocrypto.Rsa.pub_of_priv |> Jose.Jwk.make |> CCResult.get_exn;
    rsa_priv;
    clients;
    find_user;
  }