/** example:
 *  connect.sid=s%3Ao3LqlpC1rpeinZzpM4TUL5Vq_vTENwrz.SsXH54t50UaX7g7TnDBAH9D4lbgpD8mJgWxLq2N3Oso; session=a172f142-30da-46ae-9cd1-fdbda2d05c8c
 */

type t = {
  key: string,
  value: string,
};

let header_to_list = cookie_header =>
  CCString.split_on_char(';', cookie_header)
  |> CCList.map(c => String.trim(c) |> String.split_on_char('='))
  |> CCList.map(l =>
       switch (l) {
       | [key, value] => Some({key, value})
       | _ => None
       }
     );

let get_cookie = (~key, cookie_header) =>
  header_to_list(cookie_header)
  |> CCList.find_opt(x =>
       switch (x) {
       | Some(c) => c.key == key
       | None => false
       }
     )
  |> CCOpt.flatten;