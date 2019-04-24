type t = {
  set: (~kind: string, ~key: string, string) => Lwt.t(unit),
  get: (~kind: string, string) => Lwt.t(string),
};

let make: unit => Lwt.t(t);