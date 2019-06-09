describe("OAuth behaviors", function() {
  const baseUrl =
    process.env.BASE_URL || "https://op.certification.openid.net:61740/";
  it("Trying to use authorization code twice should result in an error", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(50).click();

    cy.get(".container > :nth-child(3) > a").click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(50).should("have.class", "btn-success");
  });

  it("Trying to use authorization code twice with 30 seconds in between uses must result in an error", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(51).click();

    cy.get(".container > :nth-child(3) > a").click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.wait(5 * 1000);

    cy.buttonByRow(51).should("have.class", "btn-success");
  });

  it.skip("Trying to use authorization code twice should result in revoking previously issued access tokens", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(52).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(52).should("have.class", "btn-success");
  });
});
