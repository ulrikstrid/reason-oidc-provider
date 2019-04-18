type ctx = unit;
/*
 module type HttpImpl = {
   module Status: {type t;};

   module Headers: {type t;};
   module Request: {type t;};
   //module Version: {type t;};

   module Response: {
     type t;
     let create: (~reason: string=?, ~headers: Headers.t=?, Status.t) => t;
   };

   module Reqd: {
     type t;
     let respond_with_string: (t, Response.t, string) => unit;
     let request: t => Request.t;
   };
 };
 */
let makeCallback =
    (
      ~target,
      ~method,
      ~getHeader,
      ~create_response,
      ~respond_with_string,
      ~headers_of_list,
      ~headers,
      reqd,
      mk_context,
    ) => {
  let req_uri = target |> Uri.of_string;
  let req_path = Uri.path(req_uri);
  let path_parts = Str.(split(regexp("/"), req_path));

  let _ctx = mk_context();

  switch (method, path_parts) {
  | (`GET, ["authorize"]) =>
    Routes.Authorize.makeRoute(
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      ~getHeader,
      target,
      reqd,
    )
  // | (`GET, ["interaction"]) => Routes.LoginForm.makeRoute(reqd)
  // | (`POST, ["interaction"]) => Routes.ValidateAuth.makeRoute(create_response, reqd)
  | _ =>
    Routes.OkResponse.make(
      ~respond_with_string,
      ~create_response,
      ~headers_of_list,
      reqd,
    );
    Lwt.return_unit;
  };
};