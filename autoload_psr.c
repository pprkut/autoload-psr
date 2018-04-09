
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "php_main.h"
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

/* {{{ */
/* Based on code from php_spl.c */
static int include_class_file(zend_string *class, char *class_file, int class_file_len)
{
    zval dummy;
    zend_file_handle file_handle;
    zend_op_array *new_op_array;
    zval result;
    int ret;

    ret = php_stream_open_for_zend_ex(class_file, &file_handle, USE_PATH|STREAM_OPEN_FOR_INCLUDE);

    if (ret == SUCCESS) {
        zend_string *opened_path;
        if (!file_handle.opened_path) {
            file_handle.opened_path = zend_string_init(class_file, class_file_len, 0);
        }
        opened_path = zend_string_copy(file_handle.opened_path);
        ZVAL_NULL(&dummy);
        if (zend_hash_add(&EG(included_files), opened_path, &dummy)) {
            new_op_array = zend_compile_file(&file_handle, ZEND_REQUIRE);
            zend_destroy_file_handle(&file_handle);
        } else {
            new_op_array = NULL;
            zend_file_handle_dtor(&file_handle);
        }
        zend_string_release(opened_path);
        if (new_op_array) {
            ZVAL_UNDEF(&result);
            zend_execute(new_op_array, &result);

            destroy_op_array(new_op_array);
            efree(new_op_array);
            if (!EG(exception)) {
                zval_ptr_dtor(&result);
            }

            AUTOLOAD_PSR_G(loaded) = 1;

            return zend_hash_exists(EG(class_table), class);
        }
    }

    return 0;
}
/* }}} */

/* {{{ */
/* Based on code from php_spl.c */
static int autoload_psr0(zend_string *class)
{
    char *class_file;
    int class_file_len;

    class_file_len = (int)spprintf(&class_file, 0, "%s.php", ZSTR_VAL(class));

#if DEFAULT_SLASH != '\\'
    {
        char *ptr = class_file;
        char *end = ptr + class_file_len;

        while ((ptr = memchr(ptr, '\\', (end - ptr))) != NULL) {
            *ptr = DEFAULT_SLASH;
        }
    }
#endif

    {
        char *ptr = class_file;
        char *end = ptr + class_file_len;

        while ((ptr = memchr(ptr, '_', (end - ptr))) != NULL) {
            *ptr = DEFAULT_SLASH;
        }
    }

    include_class_file(class, class_file, class_file_len);

    efree(class_file);

    return 0;
}
/* }}} */

/* {{{ */
static int autoload_psr4(zend_string *class)
{
    return 0;
}
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

/* {{{ proto void autoload_psr(string class)
   Autoload class based on the PSR-0 or PSR-4 standard */
PHP_FUNCTION(autoload_psr)
{
    zend_string *class;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STR(class);
    ZEND_PARSE_PARAMETERS_END();

    AUTOLOAD_PSR_G(loaded) = 0;

    autoload_psr0(class);

    if (!AUTOLOAD_PSR_G(loaded)) {
        autoload_psr4(class);
    }
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

    zval function, ret, params[1];

    ZVAL_STRING(&function, "spl_autoload_register");
    ZVAL_STRING(&params[0], "autoload_psr");

    call_user_function(EG(function_table), NULL, &function, &ret, 1, params);

    zval_ptr_dtor(&function);
    zval_ptr_dtor(&ret);

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
    PHP_FE(autoload_psr, NULL)
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
