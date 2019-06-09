describe("Response Type", function() {
  const baseUrl =
    process.env.BASE_URL || "https://op.certification.openid.net:61740/";
  it("Authorization request missing the response_type parameter", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(3).click();

    cy.selectByUrl(
      baseUrl + "continue?path=OP-Response-Missing&index=3"
    ).click();

    cy.buttonByRow(3).should("have.class", "btn-success");
  });

  it("Request with response_type=code", function() {
    cy.visit(baseUrl);

    cy.buttonByRow(4).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(4).should("have.class", "btn-success");

    cy.clearCookies();
  });
});
