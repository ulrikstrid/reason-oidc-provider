describe("OAuth behaviors", function() {
  const baseUrl =
    process.env.BASE_URL || "https://op.certification.openid.net:61740/";
  it("Trying to use authorization code twice should result in an error", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(67).click();

    cy.get(".container > :nth-child(3) > a").click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(67).should("have.class", "btn-success");
  });

  it("Trying to use authorization code twice with 30 seconds in between uses must result in an error", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(68).click();

    cy.get(".container > :nth-child(3) > a").click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(68).should("have.class", "btn-success");
  });

  it("Trying to use authorization code twice should result in revoking previously issued access tokens", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(69).click();

    cy.get(".container > :nth-child(3) > a").click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(69).should("have.class", "btn-success");
  });
});
