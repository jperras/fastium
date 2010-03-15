#ifndef PHP_INFLECTOR_H
#define PHP_INFLECTOR_H
#endif

/* Properties */
#define PHP_INFLECTOR_EXTNAME "inflector"
#define PHP_INFLECTOR_EXTVER  "0.1"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* Include PHP header */
#include "php.h"

/* Entry point */
extern zend_module_entry inflector_module_entry;
#define phpext_inflector_ptr &inflector_module_entry;