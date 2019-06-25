open Http.HttpImpl;

let make = (~httpImpl, ~message=httpImpl.target ++ " not found", reqd) =>
  Http.Response.NotFound.make(~httpImpl, ~message, reqd) |> Lwt.return;
