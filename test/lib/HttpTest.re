open TestFramework;
open Http;

describe("Cookie", u => {
  let cookie_header = "connect.sid=abc; session=def";

  u.test("Jwk.make", ({expect}) => {
    let cookie =
      Cookie.get_cookie(~key="session", cookie_header) |> CCOpt.get_exn;

    expect.string(cookie.value).toEqual("def");
  });
});