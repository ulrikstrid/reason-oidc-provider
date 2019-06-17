describe("Userinfo Endpoint", function() {
  const baseUrl =
    process.env.BASE_URL || "https://op.certification.openid.net:61740/";
  it("UserInfo Endpoint access with POST and bearer body", function() {
    cy.clearCookies();

    cy.visit(baseUrl);

    cy.buttonByRow(21).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(21).should("have.class", "btn-success");
  });

  it("UserInfo Endpoint access with GET and bearer header", function() {
    cy.visit(baseUrl);

    cy.buttonByRow(22).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(22).should("have.class", "btn-success");

    cy.clearCookies();
  });

  it("UserInfo Endpoint access with POST and bearer header", function() {
    cy.visit(baseUrl);

    cy.buttonByRow(23).click();

    cy.login("ulrik.strid@outlook.com", "strid");

    cy.buttonByRow(23).should("have.class", "btn-success");

    cy.clearCookies();
  });
});
