#include "function/array/differenceBy.h"

#include <zend_operators.h>
#include "zend_closures.h"
#include "zend_interfaces.h"
#include "zend_exceptions.h"
#include "ext/standard/info.h"

/* Class entry pointers */
extern zend_class_entry *epl_collect_ptr;

static void internal_difference_by(
	zval *return_value, 
	zval *args, 
	int argc, 
	zend_fcall_info *fci, 
	zend_fcall_info_cache *fci_cache
) {
	zval *value, dummy, retval;
    int i, num = 0;
	HashTable exclude;
	zend_string *str, *key;
	zend_bool exists;
	zend_ulong idx;

	fci->retval = &retval;

	if (argc == 0) {
		zend_throw_exception(zend_ce_exception, "", 0);
		return;
	}

	for (i = 0; i < argc; i++) {
		num += zend_hash_num_elements(Z_ARRVAL(args[i]));
	}

	if (num == 0) {
		return;
	}

	zend_hash_init(&exclude, num, NULL, NULL, 0);

	for (i = 0; i < argc; i++) {
		zend_hash_internal_pointer_reset(Z_ARRVAL(args[i]));
		while (FAILURE != zend_hash_has_more_elements(Z_ARRVAL(args[i]))) {
			value = zend_hash_get_current_data(Z_ARRVAL(args[i]));

			ZVAL_UNDEF(&retval);
			zend_fcall_info_argn(fci, 1, value);
			zend_call_function(fci, fci_cache);

			str = zval_get_string(&retval);
			zend_hash_add(&exclude, str, &dummy);

			zend_string_release(str);
			zend_hash_move_forward(Z_ARRVAL(args[i]));	
		}
	}

	zend_hash_internal_pointer_reset(Z_ARRVAL_P(return_value));
	while (FAILURE != zend_hash_has_more_elements(Z_ARRVAL_P(return_value))) {
		value = zend_hash_get_current_data(Z_ARRVAL_P(return_value));

		ZVAL_UNDEF(&retval);
		zend_fcall_info_argn(fci, 1, value);
		if (zend_call_function(fci, fci_cache) != SUCCESS) {
			break;
		}

		str = zval_get_string(&retval);
		exists = zend_hash_exists(&exclude, str);
		zend_string_release(str);

		if (!exists) {
			zend_hash_move_forward(Z_ARRVAL_P(return_value));
			continue;	
		}

		switch(zend_hash_get_current_key(Z_ARRVAL_P(return_value), &key, &idx)) {
			case HASH_KEY_IS_STRING:
				zend_hash_del(Z_ARRVAL_P(return_value), key);
				break;

			case HASH_KEY_IS_LONG:
				zend_hash_index_del(Z_ARRVAL_P(return_value), idx);
				break;
		}
	}

	zend_hash_destroy(&exclude);
}

/* {{{ collect::differenceBy(array $array [, array $... ])
 */
const zend_internal_arg_info arginfo_collect_method_difference_by[] = {
	{ (const char*)(zend_uintptr_t)(-1), 0, ZEND_RETURN_VALUE, 0 },
	ZEND_ARG_VARIADIC_TYPE_INFO(0, values, IS_ARRAY, 0)
	ZEND_ARG_CALLABLE_INFO(0, iteratee, 0)
};

PHPAPI ZEND_NAMED_FUNCTION(epl_collect_method_difference_by)
{
	zval *array, *args, rv;
    int argc;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache;

	ZEND_PARSE_PARAMETERS_START(2, -1)
		Z_PARAM_VARIADIC_EX('+', args, argc, 1)
		Z_PARAM_FUNC(fci, fci_cache)
	ZEND_PARSE_PARAMETERS_END_EX(RETURN_ZVAL(getThis(), 1, 0));

	array = zend_read_property(epl_collect_ptr, getThis(), "value", sizeof("value")-1, 1, &rv);
	internal_difference_by(array, args, argc, &fci, &fci_cache);

	RETURN_ZVAL(getThis(), 1, 0);
}
/* }}} */

/* {{{ array differenceBy(array $array)
 */
const zend_internal_arg_info arginfo_epl_function_difference_by[] = {
	{ (const char*)(zend_uintptr_t)(-1), ZEND_TYPE_ENCODE(IS_ARRAY, 0), ZEND_RETURN_VALUE, 0 },
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_VARIADIC_TYPE_INFO(0, values, IS_ARRAY, 0)
	ZEND_ARG_CALLABLE_INFO(0, iteratee, 0)
};

PHPAPI ZEND_NAMED_FUNCTION(epl_function_difference_by)
{
	zval *array, *args;
    int argc;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache;

	ZEND_PARSE_PARAMETERS_START(3, -1)
		Z_PARAM_ARRAY_EX(array, 0, 1)
		Z_PARAM_VARIADIC_EX('+', args, argc, 1)
		Z_PARAM_FUNC(fci, fci_cache)
	ZEND_PARSE_PARAMETERS_END();

	internal_difference_by(array, args, argc, &fci, &fci_cache);
	RETURN_ZVAL(array, 1, 0);
}
/* }}} */
