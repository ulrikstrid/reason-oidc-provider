let markup = {|<html>
<head></head>
<body>
  <form action="/interaction" method="post">
    <input name="username" type="email">
    <input name="password" type="password">
    <input type="submit" value="Login">
  </form>
</body>
</html>|};

let makeRoute = (~httpImpl, reqd) => {
  Http.Response.Html.make(~httpImpl, ~markup, reqd);

  Lwt.return_unit;
};
