describe("Request Parameter - nonce", function() {
  const baseUrl =
    process.env.BASE_URL || "https://op.certification.openid.net:61740/";
  it("Login no nonce, code flow [Basic]", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(40).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(40).should("have.class", "btn-success");
  });

  it("ID Token has nonce when requested for code flow", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(41).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(41).should("have.class", "btn-success");
  });
});
