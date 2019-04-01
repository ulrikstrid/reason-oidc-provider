open H2;

let make = (~markup, reqd) => {
  let content_length = markup |> String.length |> string_of_int;
  Reqd.respond_with_string(
    reqd,
    Response.create(
      `OK,
      ~headers=
        Headers.of_list([
          ("content-type", "text/html"),
          ("content-length", content_length),
        ]),
    ),
    markup,
  );
};