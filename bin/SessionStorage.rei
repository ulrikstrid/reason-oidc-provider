type t = {
  set: (~kind: string, ~key: string, string) => Lwt.t(unit),
  get: (~kind: string, string) => Lwt.t(string),
  find: (~kind: string, string) => Lwt.t(option(string)),
};

let make: unit => Lwt.t(t);