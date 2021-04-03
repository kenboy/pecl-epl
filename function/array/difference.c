#include "function/array/difference.h"

#include <zend_operators.h>
#include "zend_closures.h"
#include "zend_interfaces.h"
#include "zend_exceptions.h"
#include "ext/standard/info.h"

/* Class entry pointers */
extern zend_class_entry *epl_collect_ptr;

static void internal_difference(zval *return_value, zval *args, int argc) 
{
	zval *value, dummy;
	int num = 0, i;
	HashTable exclude;
	zend_string *str, *string_key;
	zend_ulong num_key;
	zend_bool exists;

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

			str = zval_get_string(value);
			zend_hash_add(&exclude, str, &dummy);
			zend_string_release(str);

			zend_hash_move_forward(Z_ARRVAL(args[i]));	
		}
	}

	zend_hash_internal_pointer_reset(Z_ARRVAL_P(return_value));
	while (FAILURE != zend_hash_has_more_elements(Z_ARRVAL_P(return_value))) {

		value = zend_hash_get_current_data(Z_ARRVAL_P(return_value));

		str = zval_get_string(value);
		exists = zend_hash_exists(&exclude, str);
		zend_string_release(str);

		if (!exists) {
			zend_hash_move_forward(Z_ARRVAL_P(return_value));
			continue;	
		}

		switch(zend_hash_get_current_key(Z_ARRVAL_P(return_value), &string_key, &num_key)) {
			case HASH_KEY_IS_STRING:
				zend_hash_del(Z_ARRVAL_P(return_value), string_key);
				break;

			case HASH_KEY_IS_LONG:
				zend_hash_index_del(Z_ARRVAL_P(return_value), num_key);
				break;
		}
	}

	zend_hash_destroy(&exclude);
}

/* {{{ collect::difference(array $array [, array $... ])
 */
const zend_internal_arg_info arginfo_collect_method_difference[] = {
	{ (const char*)(zend_uintptr_t)(-1), 0, ZEND_RETURN_VALUE, 0 },
	ZEND_ARG_VARIADIC_TYPE_INFO(0, values, IS_ARRAY, 0)
};

PHPAPI ZEND_NAMED_FUNCTION(epl_collect_method_difference)
{
	zval *array, *args, rv;
    int argc;

	ZEND_PARSE_PARAMETERS_START(1, -1)
		Z_PARAM_VARIADIC('+', args, argc)
	ZEND_PARSE_PARAMETERS_END_EX(RETURN_ZVAL(getThis(), 1, 0));

	array = zend_read_property(epl_collect_ptr, getThis(), "value", sizeof("value")-1, 1, &rv);
	internal_difference(array, args, argc);

	RETURN_ZVAL(getThis(), 1, 0);
}
/* }}} */

/* {{{ array difference(array $array, array $... [, array $... ])
 */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_epl_function_difference, ZEND_RETURN_VALUE, -1, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_VARIADIC_TYPE_INFO(0, values, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

PHPAPI ZEND_NAMED_FUNCTION(epl_function_difference)
{
	zval *array, *args;
    int argc;

	ZEND_PARSE_PARAMETERS_START(2, -1)
		Z_PARAM_ARRAY_EX(array, 0, 1)
		Z_PARAM_VARIADIC('+', args, argc)
	ZEND_PARSE_PARAMETERS_END();

	internal_difference(array, args, argc);
	RETURN_ZVAL(array, 1, 0);
}
/* }}} */
