FROM php:8.0.2-buster

LABEL description="go-cgo-c-php-example"

WORKDIR /usr/src/workspace

RUN curl -L https://golang.org/dl/go1.16.6.linux-amd64.tar.gz -o go.tar.gz
RUN tar -C /usr/local -xzf go.tar.gz
RUN echo 'export PATH=$PATH:/usr/local/go/bin' >> ~/.bashrc
