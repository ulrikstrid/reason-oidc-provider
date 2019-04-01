open H2;

let make = reqd => {
  Reqd.respond_with_string(
    reqd,
    Response.create(
      `OK,
      ~headers=
        Headers.of_list([
          ("content-type", "application/text"),
          ("content-length", "2"),
        ]),
    ),
    "ok",
  );
};