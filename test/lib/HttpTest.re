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
  u.test("Uri.get_query_param", ({expect}) => {
    let form_post_data = "grant_type=authorization_code&redirect_uri=https%3A%2F%2Fop-test%3A60004%2Fauthz_cb&code=A3wZWT2UZppKo2WGqqt";
    let form_data = UrlencodedForm.parse(form_post_data);
    let get_code = UrlencodedForm.get_param("code");

    expect.option(get_code(form_data)).toBe(Some("A3wZWT2UZppKo2WGqqt"));
  });

  u.test("decodes special chars correctly", ({expect}) => {
    let form_post_data = "username=ulrik.strid%40outlook.com&password=test";
    let form_data = UrlencodedForm.parse(form_post_data);
    let get_username = UrlencodedForm.get_param("username");

    expect.option(get_username(form_data)).toBe(
      Some("ulrik.strid@outlook.com"),
    );
  });
});
