describe("Discovery", function() {
  const baseUrl =
    process.env.BASE_URL || "https://op.certification.openid.net:61740/";
  it("Publishes openid-configuration discovery information", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(6).click();

    cy.buttonByRow(6).should("have.class", "btn-success");
  });

  it("Keys in OP JWKs well formed", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(7).click();

    cy.buttonByRow(7).should("have.class", "btn-success");
  });

  it("Verify that claims_supported is published", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(8).click();

    cy.buttonByRow(8).should("have.class", "btn-success");
  });

  it("Verify that jwks_uri is published", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(9).click();

    cy.buttonByRow(9).should("have.class", "btn-success");
  });
});
