#include "function/array/chunk.h"

#include <zend_operators.h>
#include "zend_closures.h"
#include "zend_interfaces.h"
#include "zend_exceptions.h"
#include "ext/standard/info.h"

/* Class entry pointers */
extern zend_class_entry *epl_collect_ptr;

static void internal_chunk(zval *return_value, zend_long size)
{
	zval array, *value, chunk;
	zend_long count, current = 0;
	zend_string *string_key;
	zend_ulong num_key;

	if (size < 1) {
		zend_throw_exception(zend_ce_exception, "Size parameter expected to be greater than 0", 0);
		return;
	}

	count = zend_hash_num_elements(Z_ARRVAL_P(return_value));
	if (size > count) {
		zend_throw_exception(zend_ce_exception, "Size parameter expected to be greater than 0", 0);
		return;
	}

	array_init_size(&array, (uint32_t)(((count - 1) / size) + 1));
	ZVAL_UNDEF(&chunk);

	zend_hash_internal_pointer_reset(Z_ARRVAL_P(return_value));
	while (FAILURE != zend_hash_has_more_elements(Z_ARRVAL_P(return_value))) {
		if (Z_TYPE(chunk) == IS_UNDEF) {
			array_init_size(&chunk, (uint32_t)size);
		}

		value = zend_hash_get_current_data(Z_ARRVAL_P(return_value));

		switch(zend_hash_get_current_key(Z_ARRVAL_P(return_value), &string_key, &num_key)) {
			case HASH_KEY_IS_STRING:
				zend_hash_add(Z_ARRVAL(chunk), string_key, value);
				break;

			case HASH_KEY_IS_LONG:
				zend_hash_index_add(Z_ARRVAL(chunk), num_key, value);
				break;
		}

		zval_add_ref(value);

		if (!(++current % size)) {
			add_next_index_zval(&array, &chunk);
			ZVAL_UNDEF(&chunk);
		}

		zend_hash_move_forward(Z_ARRVAL_P(return_value));
	}

	if (Z_TYPE(chunk) != IS_UNDEF) {
		add_next_index_zval(&array, &chunk);
	}

	ZVAL_ZVAL(return_value, &array, 1, 0);
}

/* {{{ array chunk(array $array [, $size = 1])
 */
const zend_internal_arg_info arginfo_epl_function_chunk[] = {
	{ (const char*)(zend_uintptr_t)(-1), ZEND_TYPE_ENCODE(IS_ARRAY, 0), ZEND_RETURN_VALUE, 0 },
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, size, IS_LONG, 0)
};

PHPAPI ZEND_NAMED_FUNCTION(epl_function_chunk)
{
	zval *array;
	zend_long size = 1;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ARRAY(array)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(size)
	ZEND_PARSE_PARAMETERS_END();

	internal_chunk(array, size);
	RETURN_ZVAL(array, 1, 0);
}
/* }}} */

/* {{{ collect::chunk([$size = 1])
 */
PHPAPI const zend_internal_arg_info arginfo_collect_method_chunk[] = {
    { (const char*)(zend_uintptr_t)(-1), 0, ZEND_RETURN_VALUE, 0 },
	ZEND_ARG_TYPE_INFO(0, size, IS_LONG, 0)
};

PHPAPI ZEND_NAMED_FUNCTION(epl_collect_method_chunk)
{
	zval *array, rv;
	zend_long size = 1;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(size)
	ZEND_PARSE_PARAMETERS_END();

	array = zend_read_property(epl_collect_ptr, getThis(), "value", sizeof("value")-1, 1, &rv);
	internal_chunk(array, size);

	RETURN_ZVAL(getThis(), 1, 0);
}
/* }}} */