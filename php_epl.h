/* epl extension for PHP */

#ifndef PHP_EPL_H
# define PHP_EPL_H

extern zend_module_entry epl_module_entry;
# define phpext_epl_ptr &epl_module_entry

# define PHP_EPL_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_EPL)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_EPL_H */
