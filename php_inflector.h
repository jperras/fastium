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

/* Properties */
#define PHP_INFLECTOR_EXTNAME "inflector"
#define PHP_INFLECTOR_EXTVER  "0.1"

#endif /* PHP_INFLECTOR_H */