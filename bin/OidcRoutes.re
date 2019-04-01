open H2;

type ctx = unit;

// let makeAuthCallback = AuthCallbackRoute.make;

let makeCallback = (reqd, mk_context) => {
  let {H2.Request.target, meth, headers} as request = Reqd.request(reqd);
  let req_uri = target |> Uri.of_string;
  let req_path = Uri.path(req_uri);
  Format.printf("Req: %a\n%!", H2.Request.pp_hum, request);
  let path_parts = Str.(split(regexp("/"), req_path));

  let _ctx = mk_context(reqd);

  switch (meth, path_parts) {
  | (`GET, ["authorize"]) =>
    Routes.Authorize.makeRoute(target, headers, reqd)
  | (`GET, ["interaction"]) => Routes.LoginForm.makeRoute(reqd)
  | (`POST, ["interaction"]) => Routes.ValidateAuth.makeRoute(reqd)
  | _ =>
    Reqd.respond_with_string(reqd, Response.create(`Not_found), "");
    Lwt.return_unit;
  };
};