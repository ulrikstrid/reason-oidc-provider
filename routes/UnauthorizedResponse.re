let make =
    (~respond_with_string, ~create_response, ~headers_of_list, reqd, message) => {
  respond_with_string(
    reqd,
    create_response(
      ~headers=
        headers_of_list([
          ("content-length", String.length(message) |> string_of_int),
        ]),
      `Code(401),
    ),
    message,
  );
};