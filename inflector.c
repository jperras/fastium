#include "php_inflector.h"

zend_module_entry inflector_module_entry = {
	STANDARD_MODULE_HEADER,
	PHP_INFLECTOR_EXTNAME,
	NULL, /* Functions  */
	NULL, /* MINIT */
	NULL, /* MSHUTDOWN */
	NULL, /* RINIT */
	NULL, /* RSHUTDOWN */
	NULL, /* MINFO */
	PHP_INFLECTOR_EXTVER,
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_INFLECTOR
ZEND_GET_MODULE(inflector)
#endif