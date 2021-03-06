module Storage = Irmin_mem.KV(Irmin.Contents.String);

type t = {
  set: (~kind: string, ~key: string, string) => Lwt.t(unit),
  get: (~kind: string, string) => Lwt.t(string),
  find: (~kind: string, string) => Lwt.t(option(string)),
  remove: (~kind: string, string) => Lwt.t(unit),
};

let make: unit => Lwt.t(t) =
  () => {
    Lwt.Infix.(
      Storage.Repo.v(Irmin_mem.config())
      >>= Storage.master
      >|= (
        t => {
          {
            set: (~kind, ~key, value) => {
              Storage.set(t, [kind, key], ~info=Irmin.Info.none, value);
            },
            get: (~kind, key) => {
              Storage.get(t, [kind, key]);
            },
            find: (~kind, key) => {
              Storage.find(t, [kind, key]);
            },
            remove: (~kind, key) => {
              Storage.remove(t, ~info=Irmin.Info.none, [kind, key]);
            },
          };
        }
      )
    );
  };
