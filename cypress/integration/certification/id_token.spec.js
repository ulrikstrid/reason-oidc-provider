describe("ID Token", function() {
  const baseUrl =
    process.env.BASE_URL || "https://op.certification.openid.net:61740/";
  it("Does the OP sign the ID Token and with what", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(11).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(11).should("have.class", "btn-success");
  });

  it("IDToken has kid [Basic, Implicit, Hybrid]", function() {
    cy.visit(baseUrl);

    cy.buttonByRow(12).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(12).should("have.class", "btn-success");

    cy.clearCookies();
  });
});
