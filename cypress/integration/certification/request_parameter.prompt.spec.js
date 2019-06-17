describe.skip("Request Parameter - prompt", function() {
  const baseUrl =
    process.env.BASE_URL || "https://op.certification.openid.net:61740/";
  it("Login no nonce, code flow [Basic]", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(43).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(43).should("have.class", "btn-success");
  });

  it("Request with prompt=none when logged in [Basic, Implicit, Hybrid]", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(44).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(44).should("have.class", "btn-success");
  });

  it("Request with prompt=none when not logged in", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(45).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(45).should("have.class", "btn-success");
  });
});
