#ifndef PHP_INFLECTOR_H
#define PHP_INFLECTOR_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>

/* {{{ Defines */
#define PHP_INFLECTOR_EXTNAME "fastium"
#define PHP_INFLECTOR_EXTVER  "0.1"
#define FASTIUM_NS "lithium\\"
#define FASTIUM_UTIL_NS FASTIUM_NS "util"
/* }}} */

extern zend_module_entry inflector_module_entry;
#define phpext_inflector_ptr &inflector_module_entry;

/* {{{ ZTS */
#ifndef ZTS
#include "TSRM.h"
#endif
/* }}} */

ZEND_BEGIN_MODULE_GLOBALS(inflector)
HashTable *underscore_cache;
HashTable *humanize_cache;
HashTable *camelize_cache;
HashTable *camelize_under_cache;
ZEND_END_MODULE_GLOBALS(inflector)

#ifdef ZTS
#define INFLECTOR_G(v) TSRMG(inflector_globals_id, zend_inflector_globals *, v)
#else
#define INFLECTOR_G(v) (inflector_globals.v)
#endif


/* {{{ Forward declarations */

/* Static helper functions */
static char * _regex_enclose(char*, int);

/* PHP METHODS */
PHP_FUNCTION(underscore);
PHP_FUNCTION(humanize);
PHP_FUNCTION(camelize);
PHP_FUNCTION(enclose);
PHP_MINIT_FUNCTION(inflector);
PHP_RINIT_FUNCTION(inflector);
PHP_MSHUTDOWN_FUNCTION(inflector);
PHP_MINFO_FUNCTION(inflector);
/* }}} */

#endif /* PHP_INFLECTOR_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */