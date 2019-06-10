let make =
    (
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~req_path,
      ~message=req_path ++ " not found",
      reqd,
    ) =>
  Http.Response.NotFound.make(
    ~respond_with_string,
    ~create_response,
    ~headers_of_list,
    ~message,
    reqd,
  )
  |> Lwt.return;
