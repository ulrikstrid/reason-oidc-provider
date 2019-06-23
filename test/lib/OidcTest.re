open TestFramework;

describe("OIDC - Parameters", u => {
  let client: Oidc.Client.t = {
    id: "s6BhdRkqt3",
    redirect_uri: "https://client.example.org/cb",
  };
  let clients = [client];

  let validQuery = "http://localhost:3000/authorize?response_type=code&client_id=s6BhdRkqt3&redirect_uri=https%3A%2F%2Fclient.example.org%2Fcb&scope=openid%20profile&state=af0ifjsldkj&nonce=n-0S6_WzA2Mj";
  switch (Oidc.Parameters.parse_query(~clients, Uri.of_string(validQuery))) {
  | Valid(validParameters) =>
    u.test("parseQuery", ({expect}) => {
      expect.string(validParameters.client.id).toEqual("s6BhdRkqt3");
      expect.string(validParameters.redirect_uri).toEqual(
        "https://client.example.org/cb",
      );
      expect.string(validParameters.nonce).toEqual("n-0S6_WzA2Mj");
    })
  | _ => ()
  };
});

describe("OIDC - User", u => {
  open Oidc.User;

  let user = {
    email: "ulrik.strid@outlook.com",
    name: "Ulrik Strid",
    password: "strid",
  };

  let users = [user];

  u.test("get_user", ({expect}) => {
    let get_user = email =>
      CCList.find_opt((user: Oidc.User.t) => user.email == email, users);

    expect.option(get_user("ulrik.strid@outlook.com")).toBe(Some(user));
  });
});

describe("OIDC - Claims", u =>
  Oidc.Claims.(
    u.test("from_string", ({expect}) => {
      let claims_string = {|{
      "id_token" : {
          "email"          : null,
          "email_verified" : { "essential" : true }
      },
      "userinfo": {
          "email"          : null,
          "email_verified" : { "essential" : true },
          "name"           : null
      }
   }|};

      let claims = from_string(claims_string);

      expect.list(claims.id_token).toContainEqual(NonEssential("email"));
      expect.list(claims.id_token).toContainEqual(
        Essential("email_verified"),
      );
      expect.list(claims.userinfo).toContainEqual(NonEssential("email"));
      expect.list(claims.userinfo).toContainEqual(
        Essential("email_verified"),
      );
    })
  )
);
