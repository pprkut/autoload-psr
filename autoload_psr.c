
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_autoload_psr.h"

/* If you declare any globals in php_autoload_psr.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(autoload_psr)
*/

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
    char *arg = NULL;
    size_t arg_len, len;
    zend_string *strg;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
        return;
    }

    strg = strpprintf(0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "autoload_psr", arg);

    RETURN_STR(strg);
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
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_AUTOLOAD_PSR
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(autoload_psr)
#endif
