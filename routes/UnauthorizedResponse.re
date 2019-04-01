open H2;

let make = (reqd, message) => {
  Reqd.respond_with_string(
    reqd,
    Response.create(
      `Code(401),
      ~headers=
        Headers.of_list([
          ("content-length", String.length(message) |> string_of_int),
        ]),
    ),
    message,
  );
};