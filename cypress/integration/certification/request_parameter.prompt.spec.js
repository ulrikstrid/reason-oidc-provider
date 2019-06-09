describe.skip("Request Parameter - prompt", function() {
  const baseUrl =
    process.env.BASE_URL || "https://op.certification.openid.net:61740/";
  it("Login no nonce, code flow [Basic]", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(29).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(29).should("have.class", "btn-success");
  });

  it("Request with prompt=none when logged in [Basic, Implicit, Hybrid]", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(30).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(30).should("have.class", "btn-success");
  });

  it("Request with prompt=none when not logged in", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(31).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(31).should("have.class", "btn-success");
  });
});
