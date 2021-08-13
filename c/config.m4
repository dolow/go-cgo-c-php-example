PHP_ARG_ENABLE(my_module, Whether to enable the MyModule extension, [ --enable-my-module Enable MyModule])

if test "$MY_MODULE" != "no"; then
    PHP_NEW_EXTENSION(my_module, main.c, $ext_shared)
fi
