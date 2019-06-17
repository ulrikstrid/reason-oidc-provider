describe("Request Parameter - request", function() {
  const baseUrl =
    process.env.BASE_URL || "https://op.certification.openid.net:61740/";

  it.skip("Support request request parameter", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(50).click();

    cy.buttonByRow(50).should("have.class", "btn-success");
  });
});
