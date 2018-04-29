/* underscore extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "zend_operators.h"
#include "ext/standard/info.h"
#include "php_underscore.h"

/* {{{ array chunk(array $array [, $size = 1])
 */
static PHP_FUNCTION(underscore_chunk)
{
	zval *array, *zval_value, chunk;
	zend_long size = 1, array_count, current = 0;
	zend_string *str_key;
	zend_ulong num_key;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ARRAY(array)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(size)
	ZEND_PARSE_PARAMETERS_END();

	if (size < 1) {
		php_error_docref(NULL, E_WARNING, "Size parameter expected to be greater than 0");
		return;
	}

	array_count = zend_hash_num_elements(Z_ARRVAL_P(array));
	if (size > array_count) {
		php_error_docref(NULL, E_WARNING, "Size parameter expected to be greater than 0");
		return;
	}

	array_init_size(return_value, (uint32_t)(((array_count - 1) / size) + 1));

	ZVAL_UNDEF(&chunk);
	ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(array), num_key, str_key, zval_value) {
		if (Z_TYPE(chunk) == IS_UNDEF) {
			array_init_size(&chunk, (uint32_t)size);
		}

		if (str_key) {
			zend_hash_add(Z_ARRVAL(chunk), str_key, zval_value);
		} else {
			zend_hash_index_add(Z_ARRVAL(chunk), num_key, zval_value);
		}
		zval_add_ref(zval_value);

		if (!(++current % size)) {
				add_next_index_zval(return_value, &chunk);
				ZVAL_UNDEF(&chunk);
		}
	} ZEND_HASH_FOREACH_END();

	/* Add the final chunk if there is one. */
	if (Z_TYPE(chunk) != IS_UNDEF) {
			add_next_index_zval(return_value, &chunk);
	}
}
/* }}} */

/* {{{ array compact(array $array)
 */
static PHP_FUNCTION(underscore_compact)
{
	zval *array, *value;
	zend_string *key;
	zend_ulong idx;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ARRAY(array)
	ZEND_PARSE_PARAMETERS_END();

	array_init(return_value);

	ZEND_HASH_FOREACH_KEY_VAL_IND(Z_ARRVAL_P(array), idx, key, value) {
		if (!zend_is_true(value)) {
			continue;
		}

		if (key) {
			zend_hash_add(Z_ARRVAL_P(return_value), key, value);
		} else {
			zend_hash_index_add(Z_ARRVAL_P(return_value), idx, value);
		}
		zval_add_ref(value);
	} ZEND_HASH_FOREACH_END();	
}
/* }}} */

/* {{{ array difference(array $array)
 */
static PHP_FUNCTION(underscore_difference)
{
	zval *array, *args, *value, dummy;
    int argc, num = 0, i;
	HashTable exclude;
	zend_string *str, *key;
	zend_bool exists;
	zend_ulong idx;

	ZEND_PARSE_PARAMETERS_START(1, -1)
		Z_PARAM_ARRAY(array)
		Z_PARAM_VARIADIC('+', args, argc)
	ZEND_PARSE_PARAMETERS_END();

	if (argc == 0) {
		php_error_docref(NULL, E_WARNING, "");
		return;
	}

	for (i = 0; i < argc; i++) {
		num += zend_hash_num_elements(Z_ARRVAL(args[i]));
	}

	if (num == 0) {
		ZVAL_COPY(return_value, array);
		return;
	}

	zend_hash_init(&exclude, num, NULL, NULL, 0);

	for (i = 0; i < argc; i++) {
		ZEND_HASH_FOREACH_VAL_IND(Z_ARRVAL(args[i]), value) {
			str = zval_get_string(value);
			zend_hash_add(&exclude, str, &dummy);
			zend_string_release(str);
		} ZEND_HASH_FOREACH_END();
	}

	array_init(return_value);

	ZEND_HASH_FOREACH_KEY_VAL_IND(Z_ARRVAL_P(array), idx, key, value) {
		str = zval_get_string(value);
		exists = zend_hash_exists(&exclude, str);
		zend_string_release(str);

		if (exists) {
			continue;
		}

		if (key) {
			zend_hash_add_new(Z_ARRVAL_P(return_value), key, value);
		} else {
			zend_hash_index_add_new(Z_ARRVAL_P(return_value), idx, value);
		}

		zval_add_ref(value);
	} ZEND_HASH_FOREACH_END();

	zend_hash_destroy(&exclude);
}
/* }}} */

/* {{{ array differenceBy(array $array)
 */
static PHP_FUNCTION(underscore_difference_by)
{
	zval *array, *args, *value, dummy, retval;
    int argc, i, num = 0;
	HashTable exclude;
	zend_string *str, *key;
	zend_bool exists;
	zend_ulong idx;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache;

	ZEND_PARSE_PARAMETERS_START(1, -1)
		Z_PARAM_ARRAY(array)
		Z_PARAM_VARIADIC_EX('+', args, argc, 1)
		Z_PARAM_FUNC(fci, fci_cache)
	ZEND_PARSE_PARAMETERS_END();

	fci.retval = &retval;

	if (argc == 0) {
		php_error_docref(NULL, E_WARNING, "");
		return;
	}

	for (i = 0; i < argc; i++) {
		num += zend_hash_num_elements(Z_ARRVAL(args[i]));
	}

	if (num == 0) {
		ZVAL_COPY(return_value, array);
		return;
	}

	zend_hash_init(&exclude, num, NULL, NULL, 0);

	for (i = 0; i < argc; i++) {
		ZEND_HASH_FOREACH_VAL_IND(Z_ARRVAL(args[i]), value) {

			ZVAL_UNDEF(&retval);
			zend_fcall_info_argn(&fci, 1, value);
			if (zend_call_function(&fci, &fci_cache) != SUCCESS || Z_TYPE(retval) == IS_UNDEF) {
				continue;
			}

			if (Z_ISREF(retval)) {
				zend_unwrap_reference(&retval);
			}

			str = zval_get_string(&retval);
			zend_hash_add(&exclude, str, &dummy);
			zend_string_release(str);
		} ZEND_HASH_FOREACH_END();
	}

	array_init(return_value);

	ZEND_HASH_FOREACH_KEY_VAL_IND(Z_ARRVAL_P(array), idx, key, value) {

		ZVAL_UNDEF(&retval);
		zend_fcall_info_argn(&fci, 1, value);
		if (zend_call_function(&fci, &fci_cache) != SUCCESS || Z_TYPE(retval) == IS_UNDEF) {
			continue;
		}

		if (Z_ISREF(retval)) {
			zend_unwrap_reference(&retval);
		}

		str = zval_get_string(&retval);
		exists = zend_hash_exists(&exclude, str);
		zend_string_release(str);

		//compare_function()
		if (exists) {
			continue;
		}

		if (key) {
			zend_hash_add_new(Z_ARRVAL_P(return_value), key, value);
		} else {
			zend_hash_index_add_new(Z_ARRVAL_P(return_value), idx, value);
		}

		zval_add_ref(value);
	} ZEND_HASH_FOREACH_END();

	zend_hash_destroy(&exclude);
}
/* }}} */

/* {{{ array differenceWith(array $array)
 */
static PHP_FUNCTION(underscore_difference_with)
{
	zval *array, *args, *arr_value, *oth_value, retval;
    int argc, i;
	HashTable exclude;
	zend_string *str, *key;
	zend_bool exists;
	zend_ulong idx;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache;

	ZEND_PARSE_PARAMETERS_START(1, -1)
		Z_PARAM_ARRAY(array)
		Z_PARAM_VARIADIC_EX('+', args, argc, 1)
		Z_PARAM_FUNC(fci, fci_cache)
	ZEND_PARSE_PARAMETERS_END();

	fci.retval = &retval;

	if (argc == 0) {
		php_error_docref(NULL, E_WARNING, "");
		return;
	}

	array_init(return_value);

	ZEND_HASH_FOREACH_KEY_VAL_IND(Z_ARRVAL_P(array), idx, key, arr_value) {

		exists = 0;
		for (i = 0; i < argc; i++) {
			ZEND_HASH_FOREACH_VAL_IND(Z_ARRVAL(args[i]), oth_value) {

				ZVAL_UNDEF(&retval);
				zend_fcall_info_argn(&fci, 2, arr_value, oth_value);
				if (zend_call_function(&fci, &fci_cache) != SUCCESS || Z_TYPE(retval) == IS_UNDEF) {
					continue;
				}

				if (Z_ISREF(retval)) {
					zend_unwrap_reference(&retval);
				}

				exists = zend_is_true(&retval);
				if (exists) {
					break;
				}
			} ZEND_HASH_FOREACH_END();
		}

		if (exists) {
			continue;
		}

		if (key) {
			zend_hash_add_new(Z_ARRVAL_P(return_value), key, arr_value);
		} else {
			zend_hash_index_add_new(Z_ARRVAL_P(return_value), idx, arr_value);
		}

		zval_add_ref(arr_value);
	} ZEND_HASH_FOREACH_END();
}
/* }}} */

/* {{{ array drop(array $array, $n = 1)
 */
static PHP_FUNCTION(underscore_drop)
{
	zval *array, *entry;
	zend_long n = 1, num_key;
	zend_string *string_key;
	int num_in, pos;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ARRAY(array)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();

	num_in = zend_hash_num_elements(Z_ARRVAL_P(array));

	if (n > num_in || n < 0) {
		array_init(return_value);
		return;
	}

	/* Initialize returned array */
	array_init_size(return_value, (uint32_t)(num_in - n));

	pos = 0;
	ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(array), num_key, string_key, entry) {
		if (++pos <= n) {
			continue;
		}

		if (string_key) {
			entry = zend_hash_add_new(Z_ARRVAL_P(return_value), string_key, entry);
		} else {
			entry = zend_hash_index_add_new(Z_ARRVAL_P(return_value), num_key, entry);
		}
	} ZEND_HASH_FOREACH_END();
}
/* }}} */

/* {{{ array dropWhile(array $array, callable $predicate)
 */
static PHP_FUNCTION(underscore_drop_while)
{
	zval *array, *entry, retval, key;
	zend_long num_key;
	zend_string *string_key;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ARRAY(array)
		Z_PARAM_FUNC(fci, fci_cache)
	ZEND_PARSE_PARAMETERS_END();

	fci.retval = &retval;

	/* Initialize returned array */
	array_init(return_value);

	zend_hash_internal_pointer_reset(Z_ARRVAL_P(array));
	while (FAILURE != zend_hash_has_more_elements(Z_ARRVAL_P(array))) {

		ZVAL_UNDEF(&retval);

		entry = zend_hash_get_current_data(Z_ARRVAL_P(array));
		zend_hash_get_current_key_zval(Z_ARRVAL_P(array), &key);
		zend_fcall_info_argn(&fci, 3, entry, &key, array);

		if (zend_call_function(&fci, &fci_cache) != SUCCESS || Z_TYPE(retval) == IS_UNDEF) {
			continue;
		}

		if (!zend_is_true(&retval)) {
			break;
		}

		switch(zend_hash_get_current_key(Z_ARRVAL_P(array), &string_key, &num_key)) {
			case HASH_KEY_IS_STRING:
				zend_hash_add_new(Z_ARRVAL_P(return_value), string_key, entry);
				break;

			case HASH_KEY_IS_LONG:
				zend_hash_index_add_new(Z_ARRVAL_P(return_value), num_key, entry);
				break;
		}

		zend_hash_move_forward(Z_ARRVAL_P(array));
	}
}
/* }}} */

/* {{{ array dropRight(array $array, $n = 1)
 */
static PHP_FUNCTION(underscore_drop_right)
{
	zval *array, *entry;
	zend_long n = 1, num_key;
	zend_string *string_key;
	uint32_t length;
	int num_in, pos;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ARRAY(array)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();

	num_in = zend_hash_num_elements(Z_ARRVAL_P(array));

	if (n > num_in || n < 0) {
		array_init(return_value);
		return;
	}

	length = (uint32_t)(num_in - n);

	/* Initialize returned array */
	array_init_size(return_value, length);

	pos = 0;
	ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(array), num_key, string_key, entry) {
		if (pos++ >= length) {
			continue;
		}

		if (string_key) {
			entry = zend_hash_add_new(Z_ARRVAL_P(return_value), string_key, entry);
		} else {
			entry = zend_hash_index_add_new(Z_ARRVAL_P(return_value), num_key, entry);
		}
	} ZEND_HASH_FOREACH_END();
}
/* }}} */

/* {{{ array dropRightWhile(array $array, callable $predicate)
 */
static PHP_FUNCTION(underscore_drop_right_while)
{
	zval *array, *entry, retval, key;
	zend_long num_key;
	zend_string *string_key;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ARRAY(array)
		Z_PARAM_FUNC(fci, fci_cache)
	ZEND_PARSE_PARAMETERS_END();

	fci.retval = &retval;

	/* Initialize returned array */
	array_init(return_value);

	zend_hash_internal_pointer_end(Z_ARRVAL_P(array));
	while (FAILURE != zend_hash_has_more_elements(Z_ARRVAL_P(array))) {

		ZVAL_UNDEF(&retval);

		entry = zend_hash_get_current_data(Z_ARRVAL_P(array));
		zend_hash_get_current_key_zval(Z_ARRVAL_P(array), &key);
		zend_fcall_info_argn(&fci, 3, entry, &key, array);

		if (zend_call_function(&fci, &fci_cache) != SUCCESS || Z_TYPE(retval) == IS_UNDEF) {
			continue;
		}

		if (!zend_is_true(&retval)) {
			break;
		}

		switch(zend_hash_get_current_key(Z_ARRVAL_P(array), &string_key, &num_key)) {
			case HASH_KEY_IS_STRING:
				zend_hash_add_new(Z_ARRVAL_P(return_value), string_key, entry);
				break;

			case HASH_KEY_IS_LONG:
				zend_hash_index_add_new(Z_ARRVAL_P(return_value), num_key, entry);
				break;
		}

		zend_hash_move_backwards(Z_ARRVAL_P(array));
	}
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

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO(arginfo_underscore_difference_by, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_VARIADIC_TYPE_INFO(0, values, IS_ARRAY, 0)
	ZEND_ARG_CALLABLE_INFO(0, iteratee, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO(arginfo_underscore_difference_with, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_VARIADIC_TYPE_INFO(0, values, IS_ARRAY, 0)
	ZEND_ARG_CALLABLE_INFO(0, iteratee, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO(arginfo_underscore_drop, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_VARIADIC_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO(arginfo_underscore_drop_while, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_CALLABLE_INFO(0, predicate, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO(arginfo_underscore_drop_right, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_VARIADIC_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO(arginfo_underscore_drop_right_while, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_CALLABLE_INFO(0, predicate, 0)
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
	ZEND_NS_NAMED_FE("_", drop, ZEND_FN(underscore_drop), arginfo_underscore_drop)
	ZEND_NS_NAMED_FE("_", dropWhile, ZEND_FN(underscore_drop_while), arginfo_underscore_drop_while)
	ZEND_NS_NAMED_FE("_", dropRight, ZEND_FN(underscore_drop_right), arginfo_underscore_drop_right)
	ZEND_NS_NAMED_FE("_", dropRightWhile, ZEND_FN(underscore_drop_right_while), arginfo_underscore_drop_right_while)
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
