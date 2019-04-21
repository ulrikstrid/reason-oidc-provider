let make =
    (
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~code=303,
      ~targetPath,
      reqd,
    ) => {
  let content_length = targetPath |> String.length |> string_of_int;

  respond_with_string(
    reqd,
    create_response(
      ~headers=
        headers_of_list([
          ("content-length", content_length),
          ("location", targetPath),
        ]),
      `Code(code),
    ),
    targetPath,
  );
};