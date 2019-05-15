FROM node:lts-alpine as base

ENV TERM=dumb \
  LD_LIBRARY_PATH=/usr/local/lib:/usr/lib:/lib

WORKDIR /reason-oidc-provider

RUN apk add --no-cache \
  libev libev-dev jq \
  ca-certificates wget \
  bash curl perl-utils \
  git patch gcc g++ \
  make m4 util-linux zlib-dev

RUN wget -q -O /etc/apk/keys/sgerrand.rsa.pub https://alpine-pkgs.sgerrand.com/sgerrand.rsa.pub
RUN wget https://github.com/sgerrand/alpine-pkg-glibc/releases/download/2.28-r0/glibc-2.28-r0.apk
RUN apk add --no-cache glibc-2.28-r0.apk

RUN npm install -g esy@next --unsafe-perm

COPY esy.json /reason-oidc-provider/esy.json
COPY esy.lock /reason-oidc-provider/esy.lock

RUN esy install
RUN esy build

COPY . /reason-oidc-provider

RUN esy install
RUN esy dune build --profile=docker

RUN esy mv '#{self.target_dir}/default/bin/ReasonOidcProvider.exe' /reason-oidc-provider/main.exe

RUN strip main.exe

FROM scratch

WORKDIR /reason-oidc-provider

COPY --from=base /etc/ssl/certs/ca-certificates.crt /etc/ssl/certs/
COPY --from=base /reason-oidc-provider/main.exe main.exe

EXPOSE 8080 9443

ENTRYPOINT ["/reason-oidc-provider/main.exe"]
