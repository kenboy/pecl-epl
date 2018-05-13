/* epl extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "zend_operators.h"
#include "zend_closures.h"
#include "ext/standard/info.h"
#include "php_epl.h"

static void internal_chunk(zval *return_value, zend_long size)
{
	zval *value, chunk, tmp;
	zend_long array_count, current = 0;
	zend_string *string_key;
	zend_ulong num_key;

	if (size < 1) {
		php_error_docref(NULL, E_WARNING, "Size parameter expected to be greater than 0");
		return;
	}

	array_count = zend_hash_num_elements(Z_ARRVAL_P(return_value));
	if (size > array_count) {
		php_error_docref(NULL, E_WARNING, "Size parameter expected to be greater than 0");
		return;
	}

	ZVAL_ARR(&tmp, zend_array_dup(Z_ARRVAL_P(return_value)));
	ZVAL_UNDEF(return_value);

	array_init_size(return_value, (uint32_t)(((array_count - 1) / size) + 1));
	ZVAL_UNDEF(&chunk);

	zend_hash_internal_pointer_reset(Z_ARRVAL(tmp));
	while (FAILURE != zend_hash_has_more_elements(Z_ARRVAL(tmp))) {
		if (Z_TYPE(chunk) == IS_UNDEF) {
			array_init_size(&chunk, (uint32_t)size);
		}

		value = zend_hash_get_current_data(Z_ARRVAL(tmp));

		switch(zend_hash_get_current_key(Z_ARRVAL(tmp), &string_key, &num_key)) {
			case HASH_KEY_IS_STRING:
				zend_hash_add(Z_ARRVAL(chunk), string_key, value);
				break;

			case HASH_KEY_IS_LONG:
				zend_hash_index_add(Z_ARRVAL(chunk), num_key, value);
				break;
		}

		zval_add_ref(value);

		if (!(++current % size)) {
			add_next_index_zval(return_value, &chunk);
			ZVAL_UNDEF(&chunk);
		}

		zend_hash_move_forward(Z_ARRVAL(tmp));
	}

	if (Z_TYPE(chunk) != IS_UNDEF) {
		add_next_index_zval(return_value, &chunk);
	}
}

/* {{{ array chunk(array $array [, $size = 1])
 */
static PHP_FUNCTION(epl_chunk)
{
	zval *array, tmp;
	zend_long size = 1;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ARRAY(array)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(size)
	ZEND_PARSE_PARAMETERS_END();

	ZVAL_ARR(return_value, zend_array_dup(Z_ARRVAL_P(array)));
	internal_chunk(return_value, size);
}
/* }}} */

static void internal_compact(zval *return_value)
{
	zval *array, *value;
	zend_string *string_key;
	zend_ulong num_key;

	zend_hash_internal_pointer_reset(Z_ARRVAL_P(return_value));
	while (FAILURE != zend_hash_has_more_elements(Z_ARRVAL_P(return_value))) {

		value = zend_hash_get_current_data(Z_ARRVAL_P(return_value));

		if (!zend_is_true(value)) {
			switch(zend_hash_get_current_key(Z_ARRVAL_P(return_value), &string_key, &num_key)) {
				case HASH_KEY_IS_STRING:
					zend_hash_del(Z_ARRVAL_P(return_value), string_key);
					break;

				case HASH_KEY_IS_LONG:
					zend_hash_index_del(Z_ARRVAL_P(return_value), num_key);
					break;
			}
		}

		zend_hash_move_forward(Z_ARRVAL_P(return_value));		
	}
}

/* {{{ array compact(array $array)
 */
static PHP_FUNCTION(epl_compact)
{
	zval *array;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY(array)
	ZEND_PARSE_PARAMETERS_END();

	ZVAL_ARR(return_value, zend_array_dup(Z_ARRVAL_P(array)));
	internal_compact(return_value);
}
/* }}} */

static void internal_difference(zval *return_value, zval *args, int argc) 
{
	zval *value, dummy;
	int num = 0, i;
	HashTable exclude;
	zend_string *str, *string_key;
	zend_ulong num_key;

	if (argc == 0) {
		php_error_docref(NULL, E_WARNING, "");
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
			value = zend_hash_get_current_data(Z_ARRVAL_P(return_value));

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

		if (zend_hash_exists(&exclude, str)) {
			switch(zend_hash_get_current_key(Z_ARRVAL_P(return_value), &string_key, &num_key)) {
				case HASH_KEY_IS_STRING:
					zend_hash_del(Z_ARRVAL_P(return_value), string_key);
					break;

				case HASH_KEY_IS_LONG:
					zend_hash_index_del(Z_ARRVAL_P(return_value), num_key);
					break;
			}
		}

		zend_string_release(str);
		zend_hash_move_forward(Z_ARRVAL_P(return_value));		
	}

	zend_hash_destroy(&exclude);
}

/* {{{ array difference(array $array)
 */
static PHP_FUNCTION(epl_difference)
{
	zval *array, *args;
    int argc;

	ZEND_PARSE_PARAMETERS_START(1, -1)
		Z_PARAM_ARRAY(array)
		Z_PARAM_VARIADIC('+', args, argc)
	ZEND_PARSE_PARAMETERS_END();

	ZVAL_ARR(return_value, zend_array_dup(Z_ARRVAL_P(array)));
	internal_difference(return_value, args, argc);
}
/* }}} */

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
		php_error_docref(NULL, E_WARNING, "");
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

			ZVAL_UNDEF(&retval);
			zend_fcall_info_argn(fci, 1, zend_hash_get_current_data(Z_ARRVAL_P(return_value)));
			zend_call_function(fci, fci_cache);

			str = zval_get_string(&retval);
			zend_hash_add(&exclude, str, &dummy);

			zend_string_release(str);
			zend_hash_move_forward(Z_ARRVAL(args[i]));	
		}
	}

	zend_hash_internal_pointer_reset(Z_ARRVAL_P(return_value));
	while (FAILURE != zend_hash_has_more_elements(Z_ARRVAL_P(return_value))) {

		ZVAL_UNDEF(&retval);
		zend_fcall_info_argn(fci, 1, zend_hash_get_current_data(Z_ARRVAL_P(return_value)));
		if (zend_call_function(fci, fci_cache) != SUCCESS) {
			break;
		}

		str = zval_get_string(&retval);

		if (zend_hash_exists(&exclude, str)) {
			switch(zend_hash_get_current_key(Z_ARRVAL_P(return_value), &key, &idx)) {
				case HASH_KEY_IS_STRING:
					zend_hash_del(Z_ARRVAL_P(return_value), key);
					break;

				case HASH_KEY_IS_LONG:
					zend_hash_index_del(Z_ARRVAL_P(return_value), idx);
					break;
			}
		}

		zend_string_release(str);
		zend_hash_move_forward(Z_ARRVAL_P(return_value));		
	}

	zend_hash_destroy(&exclude);
}

/* {{{ array differenceBy(array $array)
 */
static PHP_FUNCTION(epl_difference_by)
{
	zval *array, *args;
    int argc;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache;

	ZEND_PARSE_PARAMETERS_START(1, -1)
		Z_PARAM_ARRAY(array)
		Z_PARAM_VARIADIC_EX('+', args, argc, 1)
		Z_PARAM_FUNC(fci, fci_cache)
	ZEND_PARSE_PARAMETERS_END();

	ZVAL_ARR(return_value, zend_array_dup(Z_ARRVAL_P(array)));
	internal_difference_by(return_value, args, argc, &fci, &fci_cache);
}
/* }}} */

static void internal_difference_with(
	zval *return_value, 
	zval *args, 
	int argc, 
	zend_fcall_info *fci, 
	zend_fcall_info_cache *fci_cache
) {
	zval retval;
    int i;
	zend_string *key;
	zend_ulong idx;
	zend_bool exists;

	fci->retval = &retval;

	if (argc == 0) {
		php_error_docref(NULL, E_WARNING, "");
		return;
	}

	zend_hash_internal_pointer_reset(Z_ARRVAL_P(return_value));
	while (FAILURE != zend_hash_has_more_elements(Z_ARRVAL_P(return_value))) {

		exists = 0;
		for (i = 0; i < argc; i++) {
			zend_hash_internal_pointer_reset(Z_ARRVAL(args[i]));
			while (FAILURE != zend_hash_has_more_elements(Z_ARRVAL(args[i]))) {

				ZVAL_UNDEF(&retval);
				zend_fcall_info_argn(
					fci, 
					2, 
					zend_hash_get_current_data(Z_ARRVAL_P(return_value)),
					zend_hash_get_current_data(Z_ARRVAL(args[i]))
				);

				if (zend_call_function(fci, fci_cache) != SUCCESS) {
					break;
				}

				exists = zend_is_true(&retval);
				if (exists) {
					break;
				}

				zend_hash_move_forward(Z_ARRVAL(args[i]));	
			}

			if (exists) {
				break;
			}
		}

		if (exists) {
			switch(zend_hash_get_current_key(Z_ARRVAL_P(return_value), &key, &idx)) {
				case HASH_KEY_IS_STRING:
					zend_hash_del(Z_ARRVAL_P(return_value), key);
					break;

				case HASH_KEY_IS_LONG:
					zend_hash_index_del(Z_ARRVAL_P(return_value), idx);
					break;
			}
		}

		zend_hash_move_forward(Z_ARRVAL_P(return_value));
	}
}

/* {{{ array differenceWith(array $array)
 */
static PHP_FUNCTION(epl_difference_with)
{
	zval *array, *args;
    int argc;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache;

	ZEND_PARSE_PARAMETERS_START(1, -1)
		Z_PARAM_ARRAY(array)
		Z_PARAM_VARIADIC_EX('+', args, argc, 1)
		Z_PARAM_FUNC(fci, fci_cache)
	ZEND_PARSE_PARAMETERS_END();

	ZVAL_ARR(return_value, zend_array_dup(Z_ARRVAL_P(array)));
	internal_difference_with(return_value, args, argc, &fci, &fci_cache);
}
/* }}} */

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

/* {{{ array drop(array $array, $n = 1)
 */
static PHP_FUNCTION(epl_drop)
{
	zval *array;
	zend_long n = 1;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ARRAY(array)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();

	ZVAL_ARR(return_value, zend_array_dup(Z_ARRVAL_P(array)));
	internal_drop(return_value, n);
}
/* }}} */

static void internal_drop_while(
	zval *return_value,
	zend_fcall_info *fci,
	zend_fcall_info_cache *fci_cache
) {
	zval *entry, retval, key;
	zend_long num_key;
	zend_string *string_key;

	fci->retval = &retval;

	zend_hash_internal_pointer_reset(Z_ARRVAL_P(return_value));
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

		zend_hash_move_forward(Z_ARRVAL_P(return_value));
	}

	while (FAILURE != zend_hash_has_more_elements(Z_ARRVAL_P(return_value))) {
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

/* {{{ array dropWhile(array $array, callable $predicate)
 */
static PHP_FUNCTION(epl_drop_while)
{
	zval *array;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ARRAY(array)
		Z_PARAM_FUNC(fci, fci_cache)
	ZEND_PARSE_PARAMETERS_END();

	ZVAL_ARR(return_value, zend_array_dup(Z_ARRVAL_P(array)));
	internal_drop_while(return_value, &fci, &fci_cache);
}
/* }}} */

static void internal_drop_right(
	zval *return_value,
	zend_long n
) {
	zend_long num_key;
	zend_string *string_key;
	int pos = 0;

	zend_hash_internal_pointer_end(Z_ARRVAL_P(return_value));
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

		zend_hash_internal_pointer_end(Z_ARRVAL_P(return_value));
	}
}

/* {{{ array dropRight(array $array, $n = 1)
 */
static PHP_FUNCTION(epl_drop_right)
{
	zval *array;
	zend_long n = 1;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ARRAY(array)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();

	ZVAL_ARR(return_value, zend_array_dup(Z_ARRVAL_P(array)));
	internal_drop_right(return_value, n);
}
/* }}} */

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

/* {{{ array dropRightWhile(array $array, callable $predicate)
 */
static PHP_FUNCTION(epl_drop_right_while)
{
	zval *array;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ARRAY(array)
		Z_PARAM_FUNC(fci, fci_cache)
	ZEND_PARSE_PARAMETERS_END();

	ZVAL_ARR(return_value, zend_array_dup(Z_ARRVAL_P(array)));
	internal_drop_right_while(return_value, &fci, &fci_cache);
}
/* }}} */

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
static PHP_FUNCTION(epl_before)
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

/* {{{ arginfo
 */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO(arginfo_epl_chunk, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, size, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO(arginfo_epl_compact, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO(arginfo_epl_difference, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_VARIADIC_TYPE_INFO(0, values, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO(arginfo_epl_difference_by, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_VARIADIC_TYPE_INFO(0, values, IS_ARRAY, 0)
	ZEND_ARG_CALLABLE_INFO(0, iteratee, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO(arginfo_epl_difference_with, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_VARIADIC_TYPE_INFO(0, values, IS_ARRAY, 0)
	ZEND_ARG_CALLABLE_INFO(0, iteratee, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO(arginfo_epl_drop, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO(arginfo_epl_drop_while, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_CALLABLE_INFO(0, predicate, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO(arginfo_epl_drop_right, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO(arginfo_epl_drop_right_while, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_CALLABLE_INFO(0, predicate, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO(arginfo_epl_before, Closure, 0)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
	ZEND_ARG_CALLABLE_INFO(0, func, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ epl_functions[]
 */
static const zend_function_entry epl_functions[] = {
	ZEND_NS_NAMED_FE("epl", chunk, ZEND_FN(epl_chunk), arginfo_epl_chunk)
	ZEND_NS_NAMED_FE("epl", compact, ZEND_FN(epl_compact), arginfo_epl_compact)
	ZEND_NS_NAMED_FE("epl", difference, ZEND_FN(epl_difference), arginfo_epl_difference)
	ZEND_NS_NAMED_FE("epl", differenceBy, ZEND_FN(epl_difference_by), arginfo_epl_difference_by)
	ZEND_NS_NAMED_FE("epl", differenceWith, ZEND_FN(epl_difference_with), arginfo_epl_difference_with)
	ZEND_NS_NAMED_FE("epl", drop, ZEND_FN(epl_drop), arginfo_epl_drop)
	ZEND_NS_NAMED_FE("epl", dropWhile, ZEND_FN(epl_drop_while), arginfo_epl_drop_while)
	ZEND_NS_NAMED_FE("epl", dropRight, ZEND_FN(epl_drop_right), arginfo_epl_drop_right)
	ZEND_NS_NAMED_FE("epl", dropRightWhile, ZEND_FN(epl_drop_right_while), arginfo_epl_drop_right_while)
	ZEND_NS_NAMED_FE("epl", before, ZEND_FN(epl_before), arginfo_epl_before)
	PHP_FE_END
};
/* }}} */

/* {{{ epl_module_entry
 */
zend_module_entry epl_module_entry = {
	STANDARD_MODULE_HEADER,
	"epl",					/* Extension name */
	epl_functions,			/* zend_function_entry */
	NULL,					/* PHP_MINIT - Module initialization */
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
