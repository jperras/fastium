/* -*- Mode: C; tab-width: 4 -*- */

#include "php_inflector.h"
#include "ext/standard/php_string.h"
#include "ext/pcre/php_pcre.h"

/* {{{ inflector_functions[] */
static const function_entry inflector_functions[] = {
	PHP_FE(underscore, NULL)
	PHP_FE(humanize, NULL)
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

/* {{{ proto string humanize(string)
       Takes an under_scored version of a word and turns it into an human-readable form
	   by replacing underscores with a space, and by upper casing the initial character. */
PHP_FUNCTION(humanize)
{
	char *word = NULL;
	int word_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &word, &word_len) == FAILURE ){
		RETURN_FALSE;
	}

	if (!word_len) {
		RETURN_EMPTY_STRING();
	}

	char *result;
	int result_len;
	register char *r, *r_end;

	result = php_str_to_str(word, word_len, "_", 1, " ", 1, &result_len);
	r = result;

    *r = toupper((unsigned char) *r);
    for (r_end = r + strlen(result) - 1; r < r_end; ) {
        if (isspace((int) *(unsigned char *)r++)) {
            *r = toupper((unsigned char) *r);
        }
    }

	RETVAL_STRING(result, 1);
	efree(result);
	return;

}
/* }}} */

/* vim: set tabstop=4 softtabstop=4 shiftwidth=4 textwidth=100 smarttab noexpandtab smartindent: */