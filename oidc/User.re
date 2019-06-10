type t = {
  name: string,
  email: string,
  password: string,
};

let to_json = t =>
  `Assoc([
    ("name", `String(t.name)),
    ("email", `String(t.email)),
    ("password", `String(t.password)),
  ]);

let to_string = t => to_json(t) |> Yojson.Basic.to_string;

let find_user = (~users, email) =>
  CCList.find_opt(t => t.email == email, users);
