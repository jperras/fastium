/* -*- Mode: C; tab-width: 4 -*- */

#include "php_inflector.h"
#include "ext/standard/php_string.h"

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


/* {{{ proto string underscore(string)
       Takes a CamelCased version of a word and turns it into an under_scored one. */
PHP_FUNCTION(underscore)
{
    char *word;
	int word_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &word, &word_len) == FAILURE) {
		RETVAL_NULL();
		return;
	}

	char *regexp   = "/(?<=\\w)([A-Z])/";
	int regexp_len = sizeof(regexp) - 1;

	zval *replace;
	char *result;
	int  result_len;

	MAKE_STD_ZVAL(replace);
	ZVAL_STRING(replace, "_\\1", 0);

	php_strtolower(word, word_len);
	result = php_pcre_replace(regexp, regexp_len, word, word_len, replace, 0, &result_len, -1, NULL TSRMLS_CC);
	FREE_ZVAL(replace);

	if (result == NULL) {
		RETVAL_NULL();
		return;
	}

	RETVAL_STRING(result, result_len);
	return;
}
/* }}} */

/* vim: set tabstop=4 softtabstop=4 shiftwidth=4 textwidth=100 smarttab noexpandtab smartindent: */