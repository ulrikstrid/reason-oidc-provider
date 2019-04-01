let markup = {|<html>
<head></head>
<body>
  <form target="/interaction">
    <input name="username" type="email">
    <input name="password" type="password">
    <input type="submit" value="Login">
  </form>
</body>
</html>|};

let makeRoute = reqd => {
  HtmlResponse.make(~markup, reqd);

  Lwt.return_unit;
};