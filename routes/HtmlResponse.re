let make =
    (~respond_with_string, ~create_response, ~headers_of_list, ~markup, reqd) => {
  let content_length = markup |> String.length |> string_of_int;
  respond_with_string(
    reqd,
    create_response(
      ~headers=
        headers_of_list([
          ("content-type", "text/html"),
          ("content-length", content_length),
        ]),
      `OK,
    ),
    markup,
  );
};