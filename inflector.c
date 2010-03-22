/* -*- Mode: C; tab-width: 4 -*- */

#include "php_inflector.h"
#include "ext/standard/php_string.h"
#include "ext/pcre/php_pcre.h"
#include "Zend/zend_hash.h"

/* Declarations */
static zend_class_entry *inflector_ce = NULL;

ZEND_DECLARE_MODULE_GLOBALS(inflector);

/*
 * Static helper functions.
 * @todo Move to separate .h/.c files.
 */

/* {{{ _regex_enclose(char*, int)
   Enclose a string for preg matching. */
static char * _regex_enclose(char *str, int str_len)
{
	char *str1 = "(?:";
	char *str2 = ")";
	char *result;

	result = (char *) emalloc((strlen(str1) + strlen(str2) + str_len + 1) *sizeof(char));

	strcpy(result, str1);
	strcat(result, str);
	strcat(result, str2);

	return result;
}
/* }}} */

/* PHP METHODS */

/* {{{ proto string lithium\util\Inflector::underscore(string)
       Takes a CamelCased version of a word and turns it into an under_scored one. */
static PHP_METHOD(Inflector, underscore)
{
	char *word = NULL, *result = NULL;
	int word_len = 0, result_len = 0;
	zval *replace_val;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &word, &word_len) == FAILURE) {
		RETURN_FALSE;
	}

	if (!word_len) {
		RETURN_EMPTY_STRING();
	}

	if (zend_hash_exists(INFLECTOR_G(underscore_cache), word, word_len)) {
		zval **cache;
		if (zend_hash_find(INFLECTOR_G(underscore_cache), word, word_len, (void **) &cache) == SUCCESS) {
			// RETURN_STRINGL(Z_STRVAL_PP(cache), Z_STRLEN_PP(cache), 0);
			RETURN_STRING(cache, 1);
		}
	}

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
	zend_hash_add(INFLECTOR_G(underscore_cache), word, word_len, result, result_len, NULL);

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
	char *word = NULL, *separator = "_", *result = NULL;
	int word_len, separator_len = 1, result_len;
	zval *params[1], *fname;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s", &word, &word_len, &separator, &separator_len) == FAILURE) {
		RETURN_FALSE;
	}

	if (!word_len) {
		RETURN_EMPTY_STRING();
	}

	if (zend_hash_exists(INFLECTOR_G(humanize_cache), word, word_len)) {
		zval **cache;
		if (zend_hash_find(INFLECTOR_G(humanize_cache), word, word_len, (void **) &cache) == SUCCESS) {
			// RETURN_STRINGL(Z_STRVAL_PP(cache), Z_STRLEN_PP(cache), 1);
			RETURN_STRING(cache, 1);
		}
	}

	result = php_str_to_str(word, word_len, separator, separator_len, " ", 1, &result_len);

	MAKE_STD_ZVAL(fname);
	ZVAL_STRING(fname, "ucwords", 1);
	MAKE_STD_ZVAL(params[0]);
	ZVAL_STRINGL(params[0], result, result_len, 1);

	if (call_user_function(EG(function_table), NULL,
				fname, return_value, 1, params TSRMLS_CC) == FAILURE) {

		php_error_docref(NULL TSRMLS_CC, E_WARNING,
				"Unable to call ucwords(). This shouldn't happen.");
		RETVAL_FALSE;
		goto cleanup;
	}
	zend_hash_add(INFLECTOR_G(humanize_cache), word, word_len,
		Z_STRVAL_P(return_value), Z_STRLEN_P(return_value), NULL);

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
	register char *r;
	char *word, *result, *result2;
	int word_len, result_len, result2_len;
	zend_bool cased = 1;
	zval *params[1], *fname, *callresult;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|b", &word, &word_len, &cased) == FAILURE) {
		RETURN_FALSE;
	}

	if (!word_len) {
		RETURN_EMPTY_STRING();
	}

	if (cased == 1 && zend_hash_exists(INFLECTOR_G(camelize_cache), word, word_len)) {
		zval **cache;
		if (zend_hash_find(INFLECTOR_G(camelize_cache), word, word_len, (void **) &cache) == SUCCESS) {
			// RETURN_STRINGL(Z_STRVAL_PP(cache), Z_STRLEN_PP(cache), 0);
			RETURN_STRING(cache, 1);
		}
	}
	if (cased == 0 && zend_hash_exists(INFLECTOR_G(camelize_under_cache), word, word_len)) {
		zval **cache;
		if (zend_hash_find(INFLECTOR_G(camelize_under_cache), word, word_len, (void **) &cache) == SUCCESS) {
			// RETURN_STRINGL(Z_STRVAL_PP(cache), Z_STRLEN_PP(cache), 0);
			RETURN_STRING(cache, 1);
		}
	}

	result = php_str_to_str(word, word_len, "_", 1, " ", 1, &result_len);

	MAKE_STD_ZVAL(fname);
	MAKE_STD_ZVAL(callresult);
	MAKE_STD_ZVAL(params[0]);

	ZVAL_STRING(fname, "ucwords", 1);
	ZVAL_STRINGL(params[0], result, result_len, 1);

	if (call_user_function(EG(function_table), NULL,
		fname, callresult, 1, params TSRMLS_CC) == FAILURE) {

		php_error_docref(NULL TSRMLS_CC, E_WARNING,
			"Unable to call ucwords(). This shouldn't happen.");
		goto cleanup;
	}
	result2 = php_str_to_str(Z_STRVAL_P(callresult), result_len, " ", 1, "", 0, &result2_len);

	if (cased != 1) {
		r = result2;
		*r = tolower((unsigned char) *r);
		zend_hash_add(INFLECTOR_G(camelize_under_cache), word, word_len, result2, result2_len, NULL);
	} else {
		zend_hash_add(INFLECTOR_G(camelize_cache), word, word_len, result2, result2_len, NULL);
	}

	RETVAL_STRINGL(result2, result2_len, 1);

cleanup:
	efree(result);
	efree(result2);
	zval_ptr_dtor(&callresult);
	zval_ptr_dtor(&fname);
	zval_ptr_dtor(&params[0]);
	return;
}
/* }}} */

/* {{{ proto lithium\util\Inflector::enclose(string) */
static PHP_METHOD(Inflector, enclose)
{
	char *word = NULL;
	int word_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &word, &word_len) == FAILURE) {
		RETURN_FALSE;
	}
	RETURN_STRING(_regex_enclose(word, word_len), 1);
}
/* }}} */

/* EXTENSION DIRECTIVES */

/* {{{ inflector_functions[] */
static function_entry inflector_class_methods[] = {
	PHP_ME(Inflector, underscore, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Inflector, humanize,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Inflector, camelize,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Inflector, enclose,    NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
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
	INIT_NS_CLASS_ENTRY(ce, FASTIUM_UTIL_NS, "Inflector", inflector_class_methods);
	inflector_ce = zend_register_internal_class(&ce TSRMLS_CC);

	int flags;
	flags = (ZEND_ACC_STATIC | ZEND_ACC_PROTECTED);

	zend_declare_property_null(inflector_ce , ZEND_STRL("_transliteration"), flags TSRMLS_CC);
	zend_declare_property_null(inflector_ce , ZEND_STRL("_uninflected"), flags TSRMLS_CC);
	zend_declare_property_null(inflector_ce , ZEND_STRL("_singular"), flags TSRMLS_CC);
	zend_declare_property_null(inflector_ce , ZEND_STRL("_plural"), flags TSRMLS_CC);

	ALLOC_HASHTABLE(INFLECTOR_G(underscore_cache));
	ALLOC_HASHTABLE(INFLECTOR_G(humanize_cache));
	ALLOC_HASHTABLE(INFLECTOR_G(camelize_cache));
	ALLOC_HASHTABLE(INFLECTOR_G(camelize_under_cache));

	zend_hash_init(INFLECTOR_G(underscore_cache), 0, NULL, NULL, 1);
	zend_hash_init(INFLECTOR_G(humanize_cache), 0, NULL, NULL, 1);
	zend_hash_init(INFLECTOR_G(camelize_cache), 0, NULL, NULL, 1);
	zend_hash_init(INFLECTOR_G(camelize_under_cache), 0, NULL, NULL, 1);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION */
PHP_MSHUTDOWN_FUNCTION(inflector)
{
	zend_hash_destroy(INFLECTOR_G(underscore_cache));
	zend_hash_destroy(INFLECTOR_G(humanize_cache));
	zend_hash_destroy(INFLECTOR_G(camelize_cache));
	zend_hash_destroy(INFLECTOR_G(camelize_under_cache));

	FREE_HASHTABLE(INFLECTOR_G(underscore_cache));
	FREE_HASHTABLE(INFLECTOR_G(humanize_cache));
	FREE_HASHTABLE(INFLECTOR_G(camelize_cache));
	FREE_HASHTABLE(INFLECTOR_G(camelize_under_cache));

	INFLECTOR_G(underscore_cache) = NULL;
	INFLECTOR_G(humanize_cache) = NULL;
	INFLECTOR_G(camelize_cache) = NULL;
	INFLECTOR_G(camelize_under_cache) = NULL;

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(inflector)
{
	php_info_print_table_start();
	php_info_print_table_header(2, PHP_INFLECTOR_EXTNAME, "Zoooom!");
	php_info_print_table_row(2, "Speed", "enabled");
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