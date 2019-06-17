describe("Client Authentication", function() {
  const baseUrl =
    process.env.BASE_URL || "https://op.certification.openid.net:61740/";

  it.skip("To refresh an Access Token", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(19).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(19).should("have.class", "btn-success");
  });
});
