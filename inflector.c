/* -*- Mode: C; tab-width: 4 -*- */

#include "php_inflector.h"
#include "ext/standard/php_string.h"
#include "ext/pcre/php_pcre.h"

/* Declarations */
static zend_class_entry *inflector_ce = NULL;

/* {{{ proto string lithium\util\Inflector::underscore(string)
       Takes a CamelCased version of a word and turns it into an under_scored one. */
static PHP_METHOD(Inflector, underscore)
{
	char *word = NULL;
	int word_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &word, &word_len) == FAILURE) {
		RETURN_FALSE;
	}

	if (!word_len) {
		RETURN_EMPTY_STRING();
	}

	char *result = NULL;
	int result_len = 0;

	zval *replace_val;
	MAKE_STD_ZVAL(replace_val);
	ZVAL_STRING(replace_val, "_\\1", 1);

	/* Perform regular expression replacement */
	result = php_pcre_replace(
		"/(?<=\\w)([A-Z])/",strlen("/(?<=\\w)([A-Z])/"),
		word, word_len,
		replace_val, 0, &result_len, -1, NULL TSRMLS_CC);

	/* Make everything lowercase */
	php_strtolower(result, result_len);

	RETVAL_STRINGL(result, result_len, 1);

	zval_ptr_dtor(&replace_val);
	efree(result);
	return;
}
/* }}} */

/* {{{ proto string lithium\util\Inflector::humanize(string)
       Takes an under_scored version of a word and turns it into an human-readable form
	   by replacing underscores with a space, and by upper casing the initial character. */
static PHP_METHOD(Inflector, humanize)
{
	char *word = NULL, *separator = "_";
	int word_len, separator_len = 1;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s", &word, &word_len, &separator, &separator_len) == FAILURE) {
		RETURN_FALSE;
	}

	if (!word_len) {
		RETURN_EMPTY_STRING();
	}

	char *result;
	int result_len;
	result = php_str_to_str(word, word_len, separator, separator_len, " ", 1, &result_len);

	zval *params[1], *fname;
	MAKE_STD_ZVAL(fname);
	ZVAL_STRING(fname, "ucwords", 1);

	MAKE_STD_ZVAL(params[0]);
	ZVAL_STRINGL(params[0], result, result_len, 1);

	if (call_user_function(EG(function_table), NULL,
				fname, return_value, 1, &params TSRMLS_CC) == FAILURE) {

		php_error_docref(NULL TSRMLS_CC, E_WARNING,
				"Unable to call ucwords(). This shouldn't happen.");
		RETVAL_FALSE;
		goto cleanup;
	}

cleanup:
	efree(result);
	zval_ptr_dtor(&fname);
	zval_ptr_dtor(&params[0]);
	return;
}
/* }}} */

/* {{{ proto string lithium\util\Inflector::camelize(string, string) */
static PHP_METHOD(Inflector, camelize)
{
	char *word = NULL;
	int word_len;
	zend_bool cased = 1;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|b", &word, &word_len, &cased) == FAILURE) {
		RETURN_FALSE;
	}

	if (!word_len) {
		RETURN_EMPTY_STRING();
	}

	char *result, *result2;
	int result_len, result2_len;
	result = php_str_to_str(word, word_len, "_", 1, " ", 1, &result_len);

	zval *params[1], *fname;
	zval *callresult;
	MAKE_STD_ZVAL(fname);
	ZVAL_STRING(fname, "ucwords", 1);

	MAKE_STD_ZVAL(callresult);
	MAKE_STD_ZVAL(params[0]);
	ZVAL_STRINGL(params[0], result, result_len, 1);

	if (call_user_function(EG(function_table), NULL,
				fname, callresult, 1, &params TSRMLS_CC) == FAILURE) {

		php_error_docref(NULL TSRMLS_CC, E_WARNING,
				"Unable to call ucwords(). This shouldn't happen.");
		RETVAL_FALSE;
		goto cleanup;
	}

	result2 = php_str_to_str(Z_STRVAL_P(callresult), result_len, " ", 1, "", 0, &result2_len);
	RETVAL_STRINGL(result2, result2_len, 1);

cleanup:
	efree(result);
	efree(result2);
	efree(callresult);
	zval_ptr_dtor(&fname);
	zval_ptr_dtor(&params[0]);
	return;
}
/* }}} */

/* {{{ inflector_functions[] */
static function_entry inflector_class_methods[] = {
	PHP_ME(Inflector, underscore, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Inflector, humanize,   NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Inflector, camelize,   NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	{ NULL, NULL, NULL }
};
/* }}} */

/* {{{ inflector_module_entry  */
zend_module_entry inflector_module_entry = {
	STANDARD_MODULE_HEADER,
	PHP_INFLECTOR_EXTNAME,
	NULL,     /* Functions  */
	PHP_MINIT(inflector),     /* MINIT */
	NULL,     /* MSHUTDOWN */
	NULL,     /* RINIT */
	NULL,     /* RSHUTDOWN */
	PHP_MINFO(inflector),     /* MINFO */
	PHP_INFLECTOR_EXTVER,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(inflector)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "lithium\\util", PHP_INFLECTOR_EXTNAME, inflector_class_methods);
    inflector_ce = zend_register_internal_class(&ce TSRMLS_CC);

    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(inflector)
{
	php_info_print_table_start();
	php_info_print_table_header(2, PHP_INFLECTOR_EXTNAME, "enabled");
    php_info_print_table_row(2, "Version", PHP_INFLECTOR_EXTVER);
	php_info_print_table_end();
}

#ifdef COMPILE_DL_INFLECTOR
ZEND_GET_MODULE(inflector)
#endif
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */