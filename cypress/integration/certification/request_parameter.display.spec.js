describe.skip("Request Parameter - display", function() {
  const baseUrl =
    process.env.BASE_URL || "https://op.certification.openid.net:61740/";
  it("Request with display=page", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(37).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(37).should("have.class", "btn-success");
  });

  it("Request with display=popup", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(38).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(38).should("have.class", "btn-success");
  });
});
