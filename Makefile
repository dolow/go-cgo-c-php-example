.PHONY: cgo
cgo:
	cd ./cgo && go build -o libmymodule.so -buildmode=c-shared  ./main.go

.PHONY: c
c:
	cd ./c && EXTRA_CFLAGS="-I`pwd`/../cgo -L`pwd`/../cgo -lmymodule" make
	cp ./c/.libs/my_module.so /usr/local/lib/php/extensions/no-debug-non-zts-20200930/

.PHONY: test
test:
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`/cgo php ./php/main.php
