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

let makeRoute =
    (~respond_with_string, ~create_response, ~headers_of_list, reqd) => {
  HtmlResponse.make(
    ~respond_with_string,
    ~create_response,
    ~headers_of_list,
    ~markup,
    reqd,
  );

  Lwt.return_unit;
};