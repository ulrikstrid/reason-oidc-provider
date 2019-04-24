type t = {
  host: string,
  session_store: Lwt.t(SessionStorage.t),
};

let make = (~host, ()): t => {
  {host, session_store: SessionStorage.make()};
};