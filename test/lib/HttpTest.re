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

describe("url-encoded", u => {
  let form_post_data = "grant_type=authorization_code&redirect_uri=https%3A%2F%2Fop-test%3A60004%2Fauthz_cb&code=A3wZWT2UZppKo2WGqqt";
  let form_data = UrlencodedForm.parse(form_post_data);
  let get_code = UrlencodedForm.get_param("code");

  u.test("Uri.get_query_param", ({expect}) =>
    expect.option(get_code(form_data)).toBe(Some("A3wZWT2UZppKo2WGqqt"))
  );
});