let make =
    (~respond_with_string, ~create_response, ~headers_of_list, ~json, reqd) => {
  let content_length = json |> String.length |> string_of_int;
  respond_with_string(
    reqd,
    create_response(
      ~headers=
        headers_of_list([
          ("content-type", "application/json"),
          ("content-length", content_length),
        ]),
      `OK,
    ),
    json,
  );
};