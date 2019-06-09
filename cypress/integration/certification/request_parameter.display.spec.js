describe.skip("Request Parameter - display", function() {
  const baseUrl =
    process.env.BASE_URL || "https://op.certification.openid.net:61740/";
  it("Request with display=page", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(23).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(23).should("have.class", "btn-success");
  });

  it("Request with display=popup", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(24).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(24).should("have.class", "btn-success");
  });
});
