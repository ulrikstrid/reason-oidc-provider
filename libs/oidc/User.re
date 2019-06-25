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

let from_json = json =>
  Yojson.Basic.Util.{
    name: json |> member("name") |> to_string,
    email: json |> member("email") |> to_string,
    password: json |> member("password") |> to_string,
  };

let to_string = t => to_json(t) |> Yojson.Basic.to_string;

let from_string = s => Yojson.Basic.from_string(s) |> from_json;

let find_user = (~users, email) =>
  CCList.find_opt(t => t.email == email, users);

let get_value_by_key = (user, key) => {
  switch (key) {
  | "name" => Some(user.name)
  | "email" => Some(user.email)
  | "email_verified" => Some(user.email)
  | "password" => Some(user.password)
  | _ => None
  };
};

let get_value_by_claim = (user, claim) => {
  switch (claim) {
  | "name" => Some(("name", `String(user.name)))
  | "email" => Some(("email", `String(user.email)))
  | "email_verified" => Some(("email_verified", `Bool(true)))
  | _ => None
  };
};
