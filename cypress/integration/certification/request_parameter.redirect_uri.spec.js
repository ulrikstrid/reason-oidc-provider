describe("Request Parameter - redirect_uri", function() {
  const baseUrl =
    process.env.BASE_URL || "https://op.certification.openid.net:61740/";

  it.skip("No redirect_uri in request with one registered [Extra]", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(47).click();

    cy.selectByUrl(
      baseUrl + "continue?path=OP-redirect_uri-NotReg&index=3"
    ).click();

    cy.buttonByRow(47).should("have.class", "btn-success");
    cy.buttonByRow(47).should("have.title", "QuestionMark");
  });

  it.skip("Sent redirect_uri does not match a registered redirect_uri", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(48).click();

    cy.selectByUrl(
      baseUrl + "continue?path=OP-redirect_uri-NotReg&index=3"
    ).click();

    cy.buttonByRow(48).should("have.class", "btn-success");
    cy.buttonByRow(48).should("have.title", "QuestionMark");
  });
});
