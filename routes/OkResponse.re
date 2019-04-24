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