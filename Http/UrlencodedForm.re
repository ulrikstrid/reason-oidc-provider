type key = string;
type value = string;

type t = list((key, value));

let parse = form_data =>
  CCString.split_on_char('&', form_data)
  |> CCList.map(s => CCString.split_on_char('=', s))
  |> CCList.filter_map(p =>
       switch (p) {
       | [key, value] => Some((key, value))
       | _ => None
       }
     );

let get_param = (key: key, t: t) =>
  CCList.find_opt(((k, _)) => k == key, t)
  |> CCOpt.map(((_, value)) => value);