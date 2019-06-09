describe.skip("Userinfo Endpoint", function() {
  const baseUrl =
    process.env.BASE_URL || "https://op.certification.openid.net:61740/";
  it("UserInfo Endpoint access with POST and bearer body", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(17).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(17).should("have.class", "btn-success");
  });

  it("UserInfo Endpoint access with GET and bearer header", function() {
    cy.visit(baseUrl);

    cy.buttonByRow(18).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(18).should("have.class", "btn-success");

    cy.clearCookies();
  });

  it("UserInfo Endpoint access with POST and bearer header", function() {
    cy.visit(baseUrl);

    cy.buttonByRow(19).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(19).should("have.class", "btn-success");

    cy.clearCookies();
  });
});
