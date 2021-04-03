#include "function/array/dropRightWhile.h"

#include <zend_operators.h>
#include "zend_closures.h"
#include "zend_interfaces.h"
#include "zend_exceptions.h"
#include "ext/standard/info.h"

/* Class entry pointers */
extern zend_class_entry *epl_collect_ptr;

static void internal_drop_right_while(
	zval *return_value,
	zend_fcall_info *fci,
	zend_fcall_info_cache *fci_cache
) {
	zval *entry, retval, key;
	zend_long num_key;
	zend_string *string_key;
	uint32_t pos;

	fci->retval = &retval;

	zend_hash_internal_pointer_end(Z_ARRVAL_P(return_value));
	while (FAILURE != zend_hash_has_more_elements(Z_ARRVAL_P(return_value))) {

		ZVAL_UNDEF(&retval);

		entry = zend_hash_get_current_data(Z_ARRVAL_P(return_value));
		zend_hash_get_current_key_zval(Z_ARRVAL_P(return_value), &key);
		zend_fcall_info_argn(fci, 3, entry, &key, return_value);

		if (zend_call_function(fci, fci_cache) != SUCCESS) {
			break;
		}

		if (!zend_is_true(&retval)) {
			break;
		}

		zend_hash_move_backwards(Z_ARRVAL_P(return_value));
	}

	pos = Z_ARRVAL_P(return_value)->nInternalPointer;
	while (FAILURE != zend_hash_has_more_elements(Z_ARRVAL_P(return_value))) {
		switch(zend_hash_get_current_key(Z_ARRVAL_P(return_value), &string_key, &num_key)) {
			case HASH_KEY_IS_STRING:
				zend_hash_del(Z_ARRVAL_P(return_value), string_key);
				break;

			case HASH_KEY_IS_LONG:
				zend_hash_index_del(Z_ARRVAL_P(return_value), num_key);
				break;
		}

		Z_ARRVAL_P(return_value)->nInternalPointer = pos;
		zend_hash_move_backwards(Z_ARRVAL_P(return_value));
	}
}

/* {{{ collect::dropRightWhile(callable $predicate)
 */
const zend_internal_arg_info arginfo_collect_method_drop_right_while[] = {
	{ (const char*)(zend_uintptr_t)(-1), 0, ZEND_RETURN_VALUE, 0 },
	ZEND_ARG_CALLABLE_INFO(0, predicate, 0)
};

PHPAPI ZEND_NAMED_FUNCTION(epl_collect_method_drop_right_while)
{
	zval *array, rv;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_FUNC(fci, fci_cache)
	ZEND_PARSE_PARAMETERS_END_EX(RETURN_ZVAL(getThis(), 1, 0));

	array = zend_read_property(epl_collect_ptr, getThis(), "value", sizeof("value")-1, 1, &rv);
	internal_drop_right_while(array, &fci, &fci_cache);

	RETURN_ZVAL(getThis(), 1, 0);
}
/* }}} */

/* {{{ array dropRightWhile(array $array, callable $predicate)
 */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_epl_function_drop_right_while, ZEND_RETURN_VALUE, -1, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_CALLABLE_INFO(0, predicate, 0)
ZEND_END_ARG_INFO()

PHPAPI ZEND_NAMED_FUNCTION(epl_function_drop_right_while)
{
	zval *array;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ARRAY_EX(array, 0, 1)
		Z_PARAM_FUNC(fci, fci_cache)
	ZEND_PARSE_PARAMETERS_END();

	internal_drop_right_while(array, &fci, &fci_cache);
	RETURN_ZVAL(array, 1, 0);
}
/* }}} */
