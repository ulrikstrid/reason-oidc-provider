module Storage = Irmin_mem.KV(Irmin.Contents.String);

type t = {
  set: (~kind: string, ~key: string, string) => Lwt.t(unit),
  get: (~kind: string, string) => Lwt.t(string),
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
          };
        }
      )
    );
  };