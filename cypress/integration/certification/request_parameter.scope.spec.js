describe.skip("Request Parameter - prompt", function() {
  const baseUrl =
    process.env.BASE_URL || "https://op.certification.openid.net:61740/";
  it("Login no nonce, code flow [Basic]", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(35).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(35).should("have.class", "btn-success");
  });

  it("Request with prompt=none when logged in [Basic, Implicit, Hybrid]", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(36).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(36).should("have.class", "btn-success");
  });

  it("Request with prompt=none when not logged in", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(37).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(37).should("have.class", "btn-success");
  });

  it("Request with prompt=none when logged in [Basic, Implicit, Hybrid]", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(38).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(38).should("have.class", "btn-success");
  });

  it("Request with prompt=none when not logged in", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(39).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(39).should("have.class", "btn-success");
  });
});
