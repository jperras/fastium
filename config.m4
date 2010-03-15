PHP_ARG_ENABLE(inflector,
[Enable the "inflector" extension],
[  --enable-inflector        Enable "inflector" extension support])

if test $PHP_INFLECTOR != "no"; then
	PHP_SUBST(INFLECTOR_SHARED_LIBADD)
	PHP_NEW_EXTENSION(inflector, inflector.c, $ext_shared)
fi