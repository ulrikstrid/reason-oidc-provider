// ***********************************************
// This example commands.js shows you how to
// create various custom commands and overwrite
// existing commands.
//
// For more comprehensive examples of custom
// commands please read more here:
// https://on.cypress.io/custom-commands
// ***********************************************
//
//
// -- This is a parent command --
// Cypress.Commands.add("login", (email, password) => { ... })
//
//
// -- This is a child command --
// Cypress.Commands.add("drag", { prevSubject: 'element'}, (subject, options) => { ... })
//
//
// -- This is a dual command --
// Cypress.Commands.add("dismiss", { prevSubject: 'optional'}, (subject, options) => { ... })
//
//
// -- This is will overwrite an existing command --
// Cypress.Commands.overwrite("visit", (originalFn, url, options) => { ... })

Cypress.Commands.add("selectByUrl", url => cy.get(`a[href="${url}"]:first`));

Cypress.Commands.add("login", (email, password) => {
  cy.get('[name="username"]').type(email);
  cy.get('[name="password"]').type(password);
  cy.get('[type="submit"]').click();
});

Cypress.Commands.add("buttonByRow", row =>
  cy.get(
    `body > div > div:nth-child(4) > div.panel-body > table > tbody > tr:nth-child(${row}) > td:nth-child(1) > a > .btn`
  )
);
