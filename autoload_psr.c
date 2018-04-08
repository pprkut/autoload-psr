
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_autoload_psr.h"

ZEND_DECLARE_MODULE_GLOBALS(autoload_psr)

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("autoload_psr.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_autoload_psr_globals, autoload_psr_globals)
    STD_PHP_INI_ENTRY("autoload_psr.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_autoload_psr_globals, autoload_psr_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ proto void autoload_register_psr4_prefix(string prefix, string path)
   Register a path for a namespace prefix to be used by the PSR-4 autoloader */
PHP_FUNCTION(autoload_register_psr4_prefix)
{
    zend_string *prefix, *path;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_STR(prefix);
        Z_PARAM_STR(path);
    ZEND_PARSE_PARAMETERS_END();

    zval path_val;

    ZVAL_STR(&path_val, path);

    zend_hash_update(AUTOLOAD_PSR_G(psr4_prefixes), prefix, &path_val);
}
/* }}} */

/* {{{ php_autoload_psr_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_autoload_psr_init_globals(zend_autoload_psr_globals *autoload_psr_globals)
{
    autoload_psr_globals->global_value = 0;
    autoload_psr_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(autoload_psr)
{
    /* If you have INI entries, uncomment these lines
    REGISTER_INI_ENTRIES();
    */
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(autoload_psr)
{
    /* uncomment this line if you have INI entries
    UNREGISTER_INI_ENTRIES();
    */
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(autoload_psr)
{
#if defined(COMPILE_DL_AUTOLOAD_PSR) && defined(ZTS)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif

    return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(autoload_psr)
{
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_GINIT_FUNCTION
 */
PHP_GINIT_FUNCTION(autoload_psr)
{
    ALLOC_HASHTABLE(autoload_psr_globals->psr4_prefixes);

    zend_hash_init(autoload_psr_globals->psr4_prefixes, 0, NULL, ZVAL_PTR_DTOR, 0);
}
/* }}} */

/* {{{ PHP_GSHUTDOWN_FUNCTION
 */
PHP_GSHUTDOWN_FUNCTION(autoload_psr)
{
    zend_hash_destroy(autoload_psr_globals->psr4_prefixes);
    FREE_HASHTABLE(autoload_psr_globals->psr4_prefixes);
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(autoload_psr)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "autoload_psr support", "enabled");
    php_info_print_table_end();

    /* Remove comments if you have entries in php.ini
    DISPLAY_INI_ENTRIES();
    */
}
/* }}} */

/* {{{ autoload_psr_functions[]
 *
 * Every user visible function must have an entry in autoload_psr_functions[].
 */
const zend_function_entry autoload_psr_functions[] = {
    PHP_FE(autoload_register_psr4_prefix,    NULL)
    PHP_FE_END    /* Must be the last line in autoload_psr_functions[] */
};
/* }}} */

/* {{{ autoload_psr_module_entry
 */
zend_module_entry autoload_psr_module_entry = {
    STANDARD_MODULE_HEADER,
    "autoload_psr",
    autoload_psr_functions,
    PHP_MINIT(autoload_psr),
    PHP_MSHUTDOWN(autoload_psr),
    PHP_RINIT(autoload_psr),
    PHP_RSHUTDOWN(autoload_psr),
    PHP_MINFO(autoload_psr),
    PHP_AUTOLOAD_PSR_VERSION,
    PHP_MODULE_GLOBALS(autoload_psr),
    PHP_GINIT(autoload_psr),
    PHP_GSHUTDOWN(autoload_psr),
    NULL,
    STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */

#ifdef COMPILE_DL_AUTOLOAD_PSR
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(autoload_psr)
#endif
