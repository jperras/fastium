/* -*- Mode: C; tab-width: 4 -*- */


#include "php_inflector.h"

/* {{{ inflector_functions[] */
static const function_entry inflector_functions[] = {
	PHP_FE(underscore, NULL)
	{ NULL, NULL, NULL }
};
/* }}} */

/* Declare module entry */
zend_module_entry inflector_module_entry = {
	STANDARD_MODULE_HEADER,
	PHP_INFLECTOR_EXTNAME,
	inflector_functions,     /* Functions  */
	NULL,     /* MINIT */
	NULL,     /* MSHUTDOWN */
	NULL,     /* RINIT */
	NULL,     /* RSHUTDOWN */
	NULL,     /* MINFO */
	PHP_INFLECTOR_EXTVER,
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_INFLECTOR
ZEND_GET_MODULE(inflector)
#endif


PHP_FUNCTION(underscore)
{
	php_printf("underscore.\n");
}


/* vim: set tabstop=4 expandtab: */