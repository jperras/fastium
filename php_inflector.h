#ifndef PHP_INFLECTOR_H
#define PHP_INFLECTOR_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* Headers */
#include <php.h>

/* Entry point */
extern zend_module_entry inflector_module_entry;
#define phpext_inflector_ptr &inflector_module_entry;

#ifndef ZTS
#include "TSRM.h"
#endif

PHP_FUNCTION(underscore);
PHP_FUNCTION(humanize);
PHP_FUNCTION(camelize);
PHP_FUNCTION(enclose);
PHP_MINIT_FUNCTION(inflector);
PHP_MINFO_FUNCTION(inflector);


/* Properties */
#define PHP_INFLECTOR_EXTNAME "fastium"
#define PHP_INFLECTOR_EXTVER  "0.1"

#endif /* PHP_INFLECTOR_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */