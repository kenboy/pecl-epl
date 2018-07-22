#include "function/array/drop.h"

#include <zend_operators.h>
#include "zend_closures.h"
#include "zend_interfaces.h"
#include "zend_exceptions.h"
#include "ext/standard/info.h"

/* Class entry pointers */
extern zend_class_entry *epl_collect_ptr;

static void internal_drop(zval *return_value, zend_long n)
{
	zend_long num_key;
	zend_string *string_key;
	int pos = 0;

	zend_hash_internal_pointer_reset(Z_ARRVAL_P(return_value));
	while (FAILURE != zend_hash_has_more_elements(Z_ARRVAL_P(return_value))) {
		if (++pos > n) {
			break;
		}

		switch(zend_hash_get_current_key(Z_ARRVAL_P(return_value), &string_key, &num_key)) {
			case HASH_KEY_IS_STRING:
				zend_hash_del(Z_ARRVAL_P(return_value), string_key);
				break;

			case HASH_KEY_IS_LONG:
				zend_hash_index_del(Z_ARRVAL_P(return_value), num_key);
				break;
		}

		//zend_hash_move_forward(Z_ARRVAL_P(return_value));
	}
}

/* {{{ collect::drop([$n = 1])
 */
const zend_internal_arg_info arginfo_collect_method_drop[] = {
	{ (const char*)(zend_uintptr_t)(-1), 0, ZEND_RETURN_VALUE, 0 },
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
};

PHPAPI ZEND_NAMED_FUNCTION(epl_collect_method_drop)
{
	zval *array, rv;
	zend_long n = 1;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END_EX(RETURN_ZVAL(getThis(), 1, 0));

	array = zend_read_property(epl_collect_ptr, getThis(), "value", sizeof("value")-1, 1, &rv);
	internal_drop(array, n);

	RETURN_ZVAL(getThis(), 1, 0);
}
/* }}} */

/* {{{ array drop(array $array, $n = 1)
 */
const zend_internal_arg_info arginfo_epl_function_drop[] = {
	{ (const char*)(zend_uintptr_t)(-1), ZEND_TYPE_ENCODE(IS_ARRAY, 0), ZEND_RETURN_VALUE, 0 },
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
};

PHPAPI ZEND_NAMED_FUNCTION(epl_function_drop)
{
	zval *array;
	zend_long n = 1;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ARRAY_EX(array, 0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();

	internal_drop(array, n);
	RETURN_ZVAL(array, 1, 0);
}
/* }}} */
