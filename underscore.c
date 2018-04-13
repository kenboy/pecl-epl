/* underscore extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_underscore.h"

/* {{{ array chunk(array $array [, $size = 1])
 */
static PHP_FUNCTION(underscore_chunk)
{
	zval *array;
	zend_long size;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ARRAY(array)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(size)
	ZEND_PARSE_PARAMETERS_END();

	RETURN_ARR()
}
/* }}} */

/* {{{ array compact(array $array)
 */
static PHP_FUNCTION(underscore_compact)
{
	zval *array;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ARRAY(array)
	ZEND_PARSE_PARAMETERS_END();

	RETURN_ARR()
}
/* }}} */

/* {{{ array difference(array $array)
 */
static PHP_FUNCTION(underscore_difference)
{
	zval *array;
	zval *args;
    int argc;

	ZEND_PARSE_PARAMETERS_START(1, -1)
		Z_PARAM_ARRAY(array)
		Z_PARAM_VARIADIC('+', args, argc)
	ZEND_PARSE_PARAMETERS_END();

	RETURN_ARR()
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(underscore)
{
#if defined(ZTS) && defined(COMPILE_DL_UNDERSCORE)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(underscore)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "underscore support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ arginfo
 */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO(arginfo_underscore_chunk, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, size, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO(arginfo_underscore_compact, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO(arginfo_underscore_difference, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_VARIADIC_TYPE_INFO(0, values, IS_ARRAY, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ underscore_functions[]
 */
static const zend_function_entry underscore_functions[] = {
	ZEND_NS_NAMED_FE("_", chunk, ZEND_FN(underscore_chunk), arginfo_underscore_chunk)
	ZEND_NS_NAMED_FE("_", compact, ZEND_FN(underscore_compact), arginfo_underscore_compact)
	ZEND_NS_NAMED_FE("_", difference, ZEND_FN(underscore_difference), arginfo_underscore_difference)
	ZEND_NS_NAMED_FE("_", differenceBy, ZEND_FN(underscore_difference_by), arginfo_underscore_difference_by)
	ZEND_NS_NAMED_FE("_", differenceWith, ZEND_FN(underscore_difference_with), arginfo_underscore_difference_with)
	PHP_FE_END
};
/* }}} */

/* {{{ underscore_module_entry
 */
zend_module_entry underscore_module_entry = {
	STANDARD_MODULE_HEADER,
	"underscore",					/* Extension name */
	underscore_functions,			/* zend_function_entry */
	NULL,							/* PHP_MINIT - Module initialization */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(underscore),			/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(underscore),			/* PHP_MINFO - Module info */
	PHP_UNDERSCORE_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_UNDERSCORE
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(underscore)
#endif
