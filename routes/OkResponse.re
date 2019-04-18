let make = (~respond_with_string, ~create_response, ~headers_of_list, reqd) => {
  respond_with_string(
    reqd,
    create_response(
      ~headers=headers_of_list([("content-length", "2")]),
      `OK,
    ),
    "ok",
  );
};