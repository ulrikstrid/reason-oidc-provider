type t = {
  set: (~kind: string, ~key: string, string) => Lwt.t(unit),
  get: (~kind: string, string) => Lwt.t(string),
  find: (~kind: string, string) => Lwt.t(option(string)),
  remove: (~kind: string, string) => Lwt.t(unit),
};

let make: unit => Lwt.t(t);