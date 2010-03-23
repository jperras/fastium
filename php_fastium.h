#ifndef PHP_FASTIUM_H
#define PHP_FASTIUM_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>

/* {{{ Defines */
#define PHP_FASTIUM_EXTNAME "fastium"
#define PHP_FASTIUM_EXTVER  "0.1"
#define FASTIUM_NS "lithium\\"
#define FASTIUM_UTIL_NS FASTIUM_NS "util"
/* }}} */

extern zend_module_entry fastium_module_entry;
#define phpext_fastium_ptr &fastium_module_entry;

/* {{{ ZTS */
#ifndef ZTS
#include "TSRM.h"
#endif
/* }}} */

ZEND_BEGIN_MODULE_GLOBALS(fastium)
HashTable *underscore_cache;
HashTable *humanize_cache;
HashTable *camelize_cache;
HashTable *camelize_under_cache;
ZEND_END_MODULE_GLOBALS(fastium)

#ifdef ZTS
#define FASTIUM_G(v) TSRMG(fastium_globals_id, zend_fastium_globals *, v)
#else
#define FASTIUM_G(v) (fastium_globals.v)
#endif


/* {{{ Forward declarations */

/* Static helper functions */
static char * _regex_enclose(char*, int);
static char * _ucwords(char*, int);

/* PHP METHODS */
PHP_FUNCTION(underscore);
PHP_FUNCTION(humanize);
PHP_FUNCTION(camelize);
PHP_FUNCTION(enclose);
PHP_MINIT_FUNCTION(fastium);
PHP_RINIT_FUNCTION(fastium);
PHP_MSHUTDOWN_FUNCTION(fastium);
PHP_MINFO_FUNCTION(fastium);
/* }}} */

#endif /* PHP_FASTIUM_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */