/* underscore extension for PHP */

#ifndef PHP_UNDERSCORE_H
# define PHP_UNDERSCORE_H

extern zend_module_entry underscore_module_entry;
# define phpext_underscore_ptr &underscore_module_entry

# define PHP_UNDERSCORE_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_UNDERSCORE)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_UNDERSCORE_H */
