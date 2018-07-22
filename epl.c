/* epl extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "zend_operators.h"
#include "zend_closures.h"
#include "zend_interfaces.h"
#include "zend_exceptions.h"
#include "ext/standard/info.h"
#include "php_epl.h"
#include "function/array/chunk.h"
#include "function/array/compact.h"
#include "function/array/difference.h"
#include "function/array/differenceBy.h"
#include "function/array/differenceWith.h"
#include "function/array/drop.h"
#include "function/array/dropWhile.h"
#include "function/array/dropRight.h"
#include "function/array/dropRightWhile.h"

/* Class entry pointers */
PHPAPI zend_class_entry *epl_collect_ptr;

static ZEND_NAMED_FUNCTION(epl_before_call_magic) /* {{{ */ 
{
	zval *args;
    int argc;

	ZEND_PARSE_PARAMETERS_START(1, -1)
		Z_PARAM_VARIADIC_EX('+', args, argc, 1)
	ZEND_PARSE_PARAMETERS_END();

	RETURN_TRUE
}
/* }}} */

/* {{{ array before(int $n, callable $func)
 */
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO(arginfo_epl_function_before, Closure, 0)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
	ZEND_ARG_CALLABLE_INFO(0, func, 0)
ZEND_END_ARG_INFO()

PHPAPI ZEND_NAMED_FUNCTION(epl_function_before)
{
	zval instance;
	zend_long n;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache;
	zend_function *mptr;
	zend_function call;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(n)
		Z_PARAM_FUNC(fci, fci_cache)
	ZEND_PARSE_PARAMETERS_END();

	mptr = fci_cache.function_handler;
	mptr->type = ZEND_INTERNAL_FUNCTION;
	mptr->internal_function.handler = epl_before_call_magic;

	zend_create_fake_closure(return_value, mptr, NULL, NULL, NULL);
}
/* }}} */

/* {{{ \epl\collect::__construct(array $array)
 */
ZEND_BEGIN_ARG_INFO(arginfo_collect_method___construct, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

static ZEND_NAMED_FUNCTION(epl_collect_method___construct)
{
	zval *array = NULL, property;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_ARRAY_EX(array, 0 , 1)
	ZEND_PARSE_PARAMETERS_END();

	if (NULL == array) {
		array_init(&property);
		array = &property;
	}

	zend_update_property(epl_collect_ptr, getThis(), "value", sizeof("value")-1, array);
}
/* }}} */

/* {{{ \epl\collect::all()
 */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO(arginfo_collect_method_all, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

static ZEND_NAMED_FUNCTION(epl_collect_method_all)
{
	zval *array, rv;

	array = zend_read_property(epl_collect_ptr, getThis(), "value", sizeof("value")-1, 1, &rv);
	RETURN_ZVAL(array, 1, 0)
}
/* }}} */

/* {{{ \epl\collect::next()
 */
ZEND_BEGIN_ARG_INFO(arginfo_collect_method_current, 0)
ZEND_END_ARG_INFO()

static ZEND_NAMED_FUNCTION(epl_collect_method_current)
{
	zval *array, rv;

	array = zend_read_property(epl_collect_ptr, getThis(), "value", sizeof("value")-1, 1, &rv);
	RETURN_ZVAL(zend_hash_get_current_data(Z_ARRVAL_P(array)), 1, 0)
}
/* }}} */

/* {{{ \epl\collect::next()
 */
ZEND_BEGIN_ARG_INFO(arginfo_collect_method_key, 0)
ZEND_END_ARG_INFO()

static ZEND_NAMED_FUNCTION(epl_collect_method_key)
{
	zval *array, rv, key;

	array = zend_read_property(epl_collect_ptr, getThis(), "value", sizeof("value")-1, 1, &rv);
	zend_hash_get_current_key_zval(Z_ARRVAL_P(array), &key);
	RETURN_ZVAL(&key, 1, 0)
}
/* }}} */

/* {{{ \epl\collect::next()
 */
ZEND_BEGIN_ARG_INFO(arginfo_collect_method_next, 0)
ZEND_END_ARG_INFO()

static ZEND_NAMED_FUNCTION(epl_collect_method_next)
{
	zval *array, rv;

	array = zend_read_property(epl_collect_ptr, getThis(), "value", sizeof("value")-1, 1, &rv);
	zend_hash_move_forward(Z_ARRVAL_P(array));
}
/* }}} */

/* {{{ \epl\collect::rewind()
 */
ZEND_BEGIN_ARG_INFO(arginfo_collect_method_rewind, 0)
ZEND_END_ARG_INFO()

static ZEND_NAMED_FUNCTION(epl_collect_method_rewind)
{
	zval *array, rv;

	array = zend_read_property(epl_collect_ptr, getThis(), "value", sizeof("value")-1, 1, &rv);
	zend_hash_internal_pointer_reset(Z_ARRVAL_P(array));
}
/* }}} */

/* {{{ \epl\collect::valid()
 */
ZEND_BEGIN_ARG_INFO(arginfo_collect_method_valid, 0)
ZEND_END_ARG_INFO()

static ZEND_NAMED_FUNCTION(epl_collect_method_valid)
{
	zval *array, rv;

	array = zend_read_property(epl_collect_ptr, getThis(), "value", sizeof("value")-1, 1, &rv);
	RETURN_BOOL(SUCCESS == zend_hash_has_more_elements(Z_ARRVAL_P(array)))
}
/* }}} */

/* {{{ \epl\collect::count()
 */
ZEND_BEGIN_ARG_INFO(arginfo_collect_method_count, 0)
ZEND_END_ARG_INFO()

static ZEND_NAMED_FUNCTION(epl_collect_method_count)
{
	zval *array, rv;

	array = zend_read_property(epl_collect_ptr, getThis(), "value", sizeof("value")-1, 1, &rv);
	RETURN_LONG(zend_hash_num_elements(Z_ARRVAL_P(array)))
}
/* }}} */

/* {{{ \epl\collect::offsetExists($index)
 */
ZEND_BEGIN_ARG_INFO_EX(arginfo_collect_method_offset_exists, 0, 0, 1)
    ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

static ZEND_NAMED_FUNCTION(epl_collect_method_offset_exists)
{
	zval *index, *array, rv;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(index)
	ZEND_PARSE_PARAMETERS_END();

	array = zend_read_property(epl_collect_ptr, getThis(), "value", sizeof("value")-1, 1, &rv);

	switch (Z_TYPE_P(index)) {
		case IS_STRING:
			RETURN_BOOL(zend_symtable_exists_ind(Z_ARRVAL_P(array), Z_STR_P(index)))

		case IS_LONG:
			RETURN_BOOL(zend_hash_index_exists(Z_ARRVAL_P(array), Z_LVAL_P(index)))

		case IS_NULL:
			RETURN_BOOL(zend_hash_exists_ind(Z_ARRVAL_P(array), ZSTR_EMPTY_ALLOC()))

		case IS_RESOURCE:
			zend_error(E_NOTICE, "Resource ID#%d used as offset, casting to integer (%d)", Z_RES_HANDLE_P(index), Z_RES_HANDLE_P(index));
			RETURN_BOOL(zend_hash_index_find(Z_ARRVAL_P(array), Z_RES_HANDLE_P(index)))
			break;

		case IS_FALSE:
			RETURN_BOOL(zend_hash_index_find(Z_ARRVAL_P(array), 0))
			break;

		case IS_TRUE:
			RETURN_BOOL(zend_hash_index_find(Z_ARRVAL_P(array), 1))
			break;

		case IS_DOUBLE:
			RETURN_BOOL(zend_hash_index_find(Z_ARRVAL_P(array), zend_dval_to_lval(Z_DVAL_P(index))))
			break;

		default:
			zend_throw_exception(zend_ce_exception, "Illegal offset type", 0);
			return;
	}
}
/* }}} */

/* {{{ \epl\collect::offsetGet($index)
 */
ZEND_BEGIN_ARG_INFO_EX(arginfo_collect_method_offset_get, 0, 0, 1)
    ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

static ZEND_NAMED_FUNCTION(epl_collect_method_offset_get)
{
	zval *index, *array, *value, rv;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(index)
	ZEND_PARSE_PARAMETERS_END();

	array = zend_read_property(epl_collect_ptr, getThis(), "value", sizeof("value")-1, 1, &rv);

	switch (Z_TYPE_P(index)) {
		case IS_STRING:
			if (value = zend_hash_find(Z_ARRVAL_P(array), Z_STR_P(index))) {
				RETURN_ZVAL(value, 1, 0);
			}
			zend_throw_exception_ex(zend_ce_exception, 0, "Undefined index: %s", Z_STR_P(index));
			return;

		case IS_LONG:
			if (value = zend_hash_index_find(Z_ARRVAL_P(array), Z_LVAL_P(index))) {
				RETURN_ZVAL(value, 1, 0);
			}
			zend_throw_exception_ex(zend_ce_exception, 0, "Undefined index: %d", Z_LVAL_P(index));
			return;

		case IS_NULL:
			if (value = zend_hash_find(Z_ARRVAL_P(array), ZSTR_EMPTY_ALLOC())) {
				RETURN_ZVAL(value, 1, 0);
			}
			zend_throw_exception_ex(zend_ce_exception, 0, "Undefined index: %s", "");
			return;

		case IS_RESOURCE:
			zend_error(E_NOTICE, "Resource ID#%d used as offset, casting to integer (%d)", Z_RES_HANDLE_P(index), Z_RES_HANDLE_P(index));
			if (value = zend_hash_index_find(Z_ARRVAL_P(array), Z_RES_HANDLE_P(index))) {
				RETURN_ZVAL(value, 1, 0);
			}
			zend_throw_exception_ex(zend_ce_exception, 0, "Undefined index: %d", Z_RES_HANDLE_P(index));
			break;

		case IS_FALSE:
			if (value = zend_hash_index_find(Z_ARRVAL_P(array), 0)) {
				RETURN_ZVAL(value, 1, 0);
			}
			zend_throw_exception_ex(zend_ce_exception, 0, "Undefined index: %d", 0);
			break;

		case IS_TRUE:
			if (value = zend_hash_index_find(Z_ARRVAL_P(array), 1)) {
				RETURN_ZVAL(value, 1, 0);
			}
			zend_throw_exception_ex(zend_ce_exception, 0, "Undefined index: %d", 1);
			break;

		case IS_DOUBLE:
			if (value = zend_hash_index_find(Z_ARRVAL_P(array), zend_dval_to_lval(Z_DVAL_P(index)))) {
				RETURN_ZVAL(value, 1, 0);
			}
			zend_throw_exception_ex(zend_ce_exception, 0, "Undefined index: %d", zend_dval_to_lval(Z_DVAL_P(index)));
			break;

		default:
			zend_throw_exception(zend_ce_exception, "Illegal offset type", 0);
			return;
	}
}
/* }}} */

/* {{{ \epl\collect::offsetSet($index, $newval)
 */
ZEND_BEGIN_ARG_INFO_EX(arginfo_collect_method_offset_set, 0, 0, 2)
        ZEND_ARG_INFO(0, index)
        ZEND_ARG_INFO(0, newval)
ZEND_END_ARG_INFO()

static ZEND_NAMED_FUNCTION(epl_collect_method_offset_set)
{
	zval *index, *newval, *array, rv, *result;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(index)
		Z_PARAM_ZVAL(newval)
	ZEND_PARSE_PARAMETERS_END();

	array = zend_read_property(epl_collect_ptr, getThis(), "value", sizeof("value")-1, 1, &rv);

	switch (Z_TYPE_P(index)) {
		case IS_STRING:
			zend_hash_add(Z_ARRVAL_P(array), Z_STR_P(index), newval);
			break;

		case IS_LONG:
			zend_hash_index_add(Z_ARRVAL_P(array), Z_LVAL_P(index), newval);
			break;

		case IS_NULL:
			zend_hash_next_index_insert(Z_ARRVAL_P(array), newval);
			break;

		case IS_RESOURCE:
			zend_error(E_NOTICE, "Resource ID#%d used as offset, casting to integer (%d)", Z_RES_HANDLE_P(index), Z_RES_HANDLE_P(index));
			zend_hash_index_update(Z_ARRVAL_P(array), Z_RES_HANDLE_P(index), newval);
			break;

		case IS_FALSE:
			zend_hash_index_update(Z_ARRVAL_P(array), 0, newval);
			break;

		case IS_TRUE:
			zend_hash_index_update(Z_ARRVAL_P(array), 1, newval);
			break;

		case IS_DOUBLE:
			zend_hash_index_update(Z_ARRVAL_P(array), zend_dval_to_lval(Z_DVAL_P(index)), newval);
			break;

		default:
			zend_throw_exception(zend_ce_exception, "Illegal offset type", 0);
			return;
	}
}
/* }}} */

/* {{{ \epl\collect::offsetUnset($index)
 */
ZEND_BEGIN_ARG_INFO_EX(arginfo_collect_method_offset_unset, 0, 0, 1)
    ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

static ZEND_NAMED_FUNCTION(epl_collect_method_offset_unset)
{
	zval *index, *array, rv;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(index)
	ZEND_PARSE_PARAMETERS_END();

	array = zend_read_property(epl_collect_ptr, getThis(), "value", sizeof("value")-1, 1, &rv);

	switch (Z_TYPE_P(index)) {
		case IS_STRING:
			zend_hash_del(Z_ARRVAL_P(array), Z_STR_P(index));
			break;

		case IS_LONG:
			zend_hash_index_del(Z_ARRVAL_P(array), Z_LVAL_P(index));
			break;

		case IS_NULL:
			zend_hash_del(Z_ARRVAL_P(array), ZSTR_EMPTY_ALLOC());
			break;

		case IS_RESOURCE:
			zend_error(E_NOTICE, "Resource ID#%d used as offset, casting to integer (%d)", Z_RES_HANDLE_P(index), Z_RES_HANDLE_P(index));
			zend_hash_index_del(Z_ARRVAL_P(array), Z_RES_HANDLE_P(index));
			break;

		case IS_FALSE:
			zend_hash_index_del(Z_ARRVAL_P(array), 0);
			break;

		case IS_TRUE:
			zend_hash_index_del(Z_ARRVAL_P(array), 1);
			break;

		case IS_DOUBLE:
			zend_hash_index_del(Z_ARRVAL_P(array), zend_dval_to_lval(Z_DVAL_P(index)));
			break;

		default:
			zend_throw_exception(zend_ce_exception, "Illegal offset type", 0);
			return;
	}
}
/* }}} */

/* {{{ \epl\collect::__invoke(array $array)
 */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO(arginfo_epl_function_collect_create, IS_OBJECT, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

PHPAPI ZEND_NAMED_FUNCTION(epl_function_collect_create)
{
	zval *array = NULL, property;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_ARRAY_EX(array, 0 , 1)
	ZEND_PARSE_PARAMETERS_END();

	if (NULL == array) {
		array_init(&property);
		array = &property;
	}

	object_init_ex(return_value, epl_collect_ptr);
	zend_update_property(epl_collect_ptr, return_value, "value", sizeof("value")-1, array);
}
/* }}} */

static const zend_function_entry epl_collect_method_functions[] = {
	ZEND_FENTRY(__construct, epl_collect_method___construct, arginfo_collect_method___construct, ZEND_ACC_PUBLIC)
	ZEND_FENTRY(current, epl_collect_method_current, arginfo_collect_method_current, ZEND_ACC_PUBLIC)
	ZEND_FENTRY(key, epl_collect_method_key, arginfo_collect_method_key, ZEND_ACC_PUBLIC)
	ZEND_FENTRY(next, epl_collect_method_next, arginfo_collect_method_next, ZEND_ACC_PUBLIC)
	ZEND_FENTRY(rewind, epl_collect_method_rewind, arginfo_collect_method_rewind, ZEND_ACC_PUBLIC)
	ZEND_FENTRY(valid, epl_collect_method_valid, arginfo_collect_method_valid, ZEND_ACC_PUBLIC)
	ZEND_FENTRY(count, epl_collect_method_count, arginfo_collect_method_count, ZEND_ACC_PUBLIC)
	ZEND_FENTRY(offsetExists, epl_collect_method_offset_exists, arginfo_collect_method_offset_exists, ZEND_ACC_PUBLIC)
	ZEND_FENTRY(offsetGet, epl_collect_method_offset_get, arginfo_collect_method_offset_get, ZEND_ACC_PUBLIC)
	ZEND_FENTRY(offsetSet, epl_collect_method_offset_set, arginfo_collect_method_offset_set, ZEND_ACC_PUBLIC)
	ZEND_FENTRY(offsetUnset, epl_collect_method_offset_unset, arginfo_collect_method_offset_unset, ZEND_ACC_PUBLIC)
	ZEND_FENTRY(all, epl_collect_method_all, arginfo_collect_method_all, ZEND_ACC_PUBLIC)
	{ "chunk", epl_collect_method_chunk, arginfo_collect_method_chunk, (uint32_t) 1, ZEND_ACC_PUBLIC },
	{ "compact", epl_collect_method_compact, arginfo_collect_method_compact, (uint32_t) 0, ZEND_ACC_PUBLIC },
	{ "difference", epl_collect_method_difference, arginfo_collect_method_difference, (uint32_t) 1, ZEND_ACC_PUBLIC },
	{ "differenceBy", epl_collect_method_difference_by, arginfo_collect_method_difference_by, (uint32_t) 2, ZEND_ACC_PUBLIC },
	{ "differenceWith", epl_collect_method_difference_with, arginfo_collect_method_difference_with, (uint32_t) 2, ZEND_ACC_PUBLIC },
	{ "drop", epl_collect_method_drop, arginfo_collect_method_drop, (uint32_t) 1, ZEND_ACC_PUBLIC },
	{ "dropWhile", epl_collect_method_drop_while, arginfo_collect_method_drop_while, (uint32_t) 1, ZEND_ACC_PUBLIC },
	{ "dropRight", epl_collect_method_drop_right, arginfo_collect_method_drop_right, (uint32_t) 1, ZEND_ACC_PUBLIC },
	{ "dropRightWhile", epl_collect_method_drop_right_while, arginfo_collect_method_drop_right_while, (uint32_t) 1, ZEND_ACC_PUBLIC },
	PHP_FE_END
};

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(epl)
{
	zend_class_entry _collect_entry;

	INIT_NS_CLASS_ENTRY(_collect_entry, "epl", "collect", epl_collect_method_functions);
	epl_collect_ptr = zend_register_internal_class(&_collect_entry);
	zend_class_implements(epl_collect_ptr, 3, zend_ce_iterator, zend_ce_countable, zend_ce_arrayaccess);
	zend_declare_property_null(epl_collect_ptr, "value", sizeof("value")-1, ZEND_ACC_PROTECTED);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(epl)
{
#if defined(ZTS) && defined(COMPILE_DL_EPL)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(epl)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "epl support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ epl_functions[]
 */
static const zend_function_entry epl_functions[] = {
	ZEND_NS_NAMED_FE("epl", collect, epl_function_collect_create, arginfo_epl_function_collect_create)
	{ "epl\\chunk", epl_function_chunk, arginfo_epl_function_chunk, (uint32_t) 2, 0 },
	{ "epl\\compact", epl_function_compact, arginfo_epl_function_compact, (uint32_t) 1, 0 },
	{ "epl\\difference", epl_function_difference, arginfo_epl_function_difference, (uint32_t) 2, 0 },
	{ "epl\\differenceBy", epl_function_difference_by, arginfo_epl_function_difference_by, (uint32_t) 3, 0 },
	{ "epl\\differenceWith", epl_function_difference_with, arginfo_epl_function_difference_with, (uint32_t) 3, 0 },
	{ "epl\\drop", epl_function_drop, arginfo_epl_function_drop, (uint32_t) 2, 0 },
	{ "epl\\dropWhile", epl_function_drop_while, arginfo_epl_function_drop_while, (uint32_t) 2, 0 },
	{ "epl\\dropRight", epl_function_drop_right, arginfo_epl_function_drop_right, (uint32_t) 2, 0 },
	{ "epl\\dropRightWhile", epl_function_drop_right_while, arginfo_epl_function_drop_right_while, (uint32_t) 2, 0 },
	ZEND_NS_NAMED_FE("epl", before, epl_function_before, arginfo_epl_function_before)
	PHP_FE_END
};
/* }}} */

/* {{{ epl_module_entry
 */
zend_module_entry epl_module_entry = {
	STANDARD_MODULE_HEADER,
	"epl",					/* Extension name */
	epl_functions,			/* zend_function_entry */
	PHP_MINIT(epl),			/* PHP_MINIT - Module initialization */
	NULL,					/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(epl),			/* PHP_RINIT - Request initialization */
	NULL,					/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(epl),			/* PHP_MINFO - Module info */
	PHP_EPL_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_EPL
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(epl)
#endif
