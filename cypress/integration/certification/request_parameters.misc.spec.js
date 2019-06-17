describe("Request Parameter - Misc", function() {
  const baseUrl =
    process.env.BASE_URL || "https://op.certification.openid.net:61740/";
  it("Request with extra query component", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(58).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(58).should("have.class", "btn-success");
  });

  it.skip("Providing acr_values", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(59).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(59).should("have.class", "btn-success");
  });

  it.skip("Providing claims_locales", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(60).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(60).should("have.class", "btn-success");
  });

  it.skip("Using prompt=none with user hint through id_token_hint", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(61).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(61).should("have.class", "btn-success");
  });

  it.skip("Providing login_hint", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(62).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(62).should("have.class", "btn-success");
  });

  it.skip("Requesting ID Token with max_age=1 seconds restriction", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(63).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(63).should("have.class", "btn-success");
  });

  it.skip("Requesting ID Token with max_age=10000 seconds restriction", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(64).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(64).should("have.class", "btn-success");
  });

  it.skip("Providing ui_locales", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(65).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(65).should("have.class", "btn-success");
  });
});
