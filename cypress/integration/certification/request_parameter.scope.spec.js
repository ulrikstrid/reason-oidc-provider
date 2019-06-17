describe("Request Parameter - scope", function() {
  const baseUrl =
    process.env.BASE_URL || "https://op.certification.openid.net:61740/";

  it.skip("Scope requesting all claims", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(52).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(52).should("have.class", "btn-success");
  });

  it.skip("Scope requesting address claims", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(53).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(53).should("have.class", "btn-success");
  });

  it("Scope requesting email claims", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(54).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(54).should("have.class", "btn-success");
  });

  it.skip("Scope requesting phone claims", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(55).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(55).should("have.class", "btn-success");
  });

  it.skip("Scope requesting profile claims", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(56).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(56).should("have.class", "btn-success");
  });
});
