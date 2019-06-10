module Ok = {
  let make =
      (
        ~respond_with_string,
        ~create_response,
        ~headers_of_list,
        ~extra_headers=?,
        reqd,
      ) => {
    let headers =
      (
        switch (extra_headers) {
        | Some(h) => [("content-length", "2"), ...h]
        | None => [("content-length", "2")]
        }
      )
      |> headers_of_list;
    respond_with_string(reqd, create_response(~headers, `OK), "ok");
    ();
  };
};

module Json = {
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
};

module Html = {
  let make =
      (
        ~respond_with_string,
        ~create_response,
        ~headers_of_list,
        ~markup,
        reqd,
      ) => {
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
};

module Redirect = {
  let make =
      (
        ~respond_with_string: ('a, 'b, string) => unit,
        ~create_response,
        ~headers_of_list,
        ~extra_headers=?,
        ~code=303,
        ~targetPath,
        reqd,
      ) => {
    let content_length = targetPath |> String.length |> string_of_int;

    let constantHeaders = [
      ("content-length", content_length),
      ("location", targetPath),
    ];

    let headers =
      (
        switch (extra_headers) {
        | Some(h) => CCList.concat([constantHeaders, h])
        | None => constantHeaders
        }
      )
      |> headers_of_list;

    respond_with_string(
      reqd,
      create_response(~headers, `Code(code)),
      targetPath,
    );
  };
};

module Unauthorized = {
  let make =
      (
        ~respond_with_string,
        ~create_response,
        ~headers_of_list,
        reqd,
        message,
      ) => {
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
    ();
  };
};

module NotFound = {
  let make =
      (
        ~respond_with_string,
        ~create_response,
        ~headers_of_list,
        ~message="Not found",
        reqd,
      ) => {
    respond_with_string(
      reqd,
      create_response(
        ~headers=
          headers_of_list([
            ("content-length", String.length(message) |> string_of_int),
          ]),
        `Code(404),
      ),
      message,
    );
    ();
  };
};
