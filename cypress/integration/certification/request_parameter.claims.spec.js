describe("Request Parameter - claims", function() {
  const baseUrl =
    process.env.BASE_URL || "https://op.certification.openid.net:61740/";

  it.skip("Supports combining claims requested with scope and claims request parameter [Extra]", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(25).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(25).should("have.class", "btn-success");
  });

  it("Requesting ID Token with email claim", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(26).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(26).should("have.class", "btn-success");
  });

  it("Supports returning different claims in ID Token and at UserInfo Endpoint", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(27).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(27).should("have.class", "btn-success");
  });

  it.skip("Requesting ID Token with essential acr claim [Extra]", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(28).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(28).should("have.class", "btn-success");
  });

  it.skip("Requesting ID Token with voluntary acr claim [Extra]", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(29).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(29).should("have.class", "btn-success");
  });

  it.skip("Requesting ID Token with essential specific acr claim [Extra]", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(30).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(30).should("have.class", "btn-success");
  });

  it("Requesting ID Token with essential auth_time claim [Extra]", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(31).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(31).should("have.class", "btn-success");
  });

  it("Claims request with essential name claim", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(32).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(32).should("have.class", "btn-success");
  });

  it.skip("Claims request with essential name and voluntary email and picture claims [Extra]", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(33).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(33).should("have.class", "btn-success");
  });

  it.skip("Support claims request specifying sub value", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(34).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(34).should("have.class", "btn-success");
  });

  it.skip("Claims request with voluntary email and picture claims", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(35).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(35).should("have.class", "btn-success");
  });
});
