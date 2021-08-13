# go-cgo-c-php-example

This repository shows the simple example of go-php binding using PHP extension.

Those code example shows not only long travel between go and php, but also complecated type conversions between languages.

## NOTE

This example is based on PHP 8.0.2.

PHP changes its interface for extension frequently.

# Motivation

If you have a nice module written in Go, you may don't want to rewrite the same features in PHP.

After trying reusing Go code, you'll realize that need to separate the language binding interface and core logic of Go code eventually.

What's more, you may find out that there is lack of information about how to develop the PHP extension.

Here is the repository for the people in such challenging case.

# Structure

```
/
 |- c/    # PHP binding
 |- cgo/  # cgo that has interface for C
 |- go/   # pure Go code
 |- php/  # pure php code that uses go program as PHP extension
```

# Usage

## Prerequisites

- docker

## Prepare

On your host machine.

```
% docker compose up
% docker exec -it go-cgo-php-example /bin/bash
```

On the container.

```
$ cd ./cgo;
$ go mod tidy
```

## Build

```
$ make cgo
$ make c
```

## Run

```
$ make test
```
