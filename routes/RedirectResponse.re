open H2;



let make = (~code=303, ~targetPath, reqd) => {
  let content_length = targetPath |> String.length |> string_of_int;

  Reqd.respond_with_string(
    reqd,
    Response.create(
      Status.of_code(code),
      ~headers=Headers.of_list([("content-length", content_length)]),
    ),
    targetPath,
  );
};