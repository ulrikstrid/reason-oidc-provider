describe("Request Parameter - claims", function() {
  const baseUrl =
    process.env.BASE_URL || "https://op.certification.openid.net:61740/";
  it("Claims request with essential name claim", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(21).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(21).should("have.class", "btn-success");
  });
});
