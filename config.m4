PHP_ARG_ENABLE(fastium,
[Enable the "fastium" extension],
[  --enable-fastium        Enable "fastium" extension support])

if test $PHP_FASTIUM != "no"; then
	PHP_SUBST(FASTIUM_SHARED_LIBADD)
	PHP_NEW_EXTENSION(fastium, php_fastium_inflector.c, $ext_shared)
fi