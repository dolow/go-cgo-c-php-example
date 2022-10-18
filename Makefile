CONFIGURE_ARGS :=

all: cgo c

.PHONY: clean
clean:
	/bin/rm -rfv \
		c/.libs \
		c/autom4te.cache \
		c/build \
		c/include \
		c/modules
	
	/bin/rm -fv \
		c/config.h \
		c/config.h.in \
		c/config.log \
		c/config.nice \
		c/config.status \
		c/configure \
		c/configure.ac \
		c/libtool \
		c/main.dep \
		c/main.lo \
		c/Makefile \
		c/Makefile.fragments \
		c/Makefile.objects \
		c/my_module.la \
		c/run-tests.php \
		cgo/libmymodule.h \
		cgo/libmymodule.so \

.PHONY: cgo
cgo:
	cd ./cgo && go build -o libmymodule.so -buildmode=c-shared  ./main.go

c/configure:
	cd ./c && phpize

c/Makefile: c/configure
	cd ./c && ./configure $(CONFIGURE_ARGS)

.PHONY: c
c: c/configure c/Makefile
	cd ./c && EXTRA_CFLAGS="-I`pwd`/../cgo -L`pwd`/../cgo -lmymodule" make

install: cgo c
	cd ./c && make install

.PHONY: test
test:
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`/cgo php ./php/main.php
