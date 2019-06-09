describe("Request Parameter - Misc", function() {
  const baseUrl =
    process.env.BASE_URL || "https://op.certification.openid.net:61740/";
  it("Request with extra query component", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(41).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(41).should("have.class", "btn-success");
  });

  it.skip("Providing acr_values", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(42).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(42).should("have.class", "btn-success");
  });

  it.skip("Providing claims_locales", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(43).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(43).should("have.class", "btn-success");
  });

  it.skip("Using prompt=none with user hint through id_token_hint", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(44).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(44).should("have.class", "btn-success");
  });

  it.skip("Providing login_hint", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(45).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(45).should("have.class", "btn-success");
  });

  it.skip("Requesting ID Token with max_age=1 seconds restriction", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(46).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(46).should("have.class", "btn-success");
  });

  it.skip("Requesting ID Token with max_age=10000 seconds restriction", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(47).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(47).should("have.class", "btn-success");
  });

  it.skip("Providing ui_locales", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(48).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(48).should("have.class", "btn-success");
  });
});
