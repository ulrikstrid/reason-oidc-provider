describe("Client Authentication", function() {
  const baseUrl =
    process.env.BASE_URL || "https://op.certification.openid.net:61740/";

  it("Access token request with client_secret_basic authentication", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(14).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(14).should("have.class", "btn-success");
  });

  it.skip("Access token request with private_key_jwt authentication", function() {
    cy.visit(baseUrl);

    cy.buttonByRow(15).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(15).should("have.class", "btn-success");

    cy.clearCookies();
  });

  it.skip("Access token request with client_secret_jwt authentication", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(16).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(16).should("have.class", "btn-success");
  });

  it.skip("Access token request with client_secret_post authentication", function() {
    cy.visit(baseUrl);

    cy.buttonByRow(17).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(17).should("have.class", "btn-success");

    cy.clearCookies();
  });
});
