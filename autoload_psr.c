
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "php_main.h"
#include "ext/standard/info.h"
#include "php_autoload_psr.h"

ZEND_DECLARE_MODULE_GLOBALS(autoload_psr);

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
static int initialize_hashtable()
{
    if (!AUTOLOAD_PSR_G(psr4_prefixes))
    {
        ALLOC_HASHTABLE(AUTOLOAD_PSR_G(psr4_prefixes));
        zend_hash_init(AUTOLOAD_PSR_G(psr4_prefixes), 0, NULL, ZVAL_PTR_DTOR, 0);
    }

    return SUCCESS;
}
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

    #if PHP_VERSION_ID >= 80100 /* if PHP version is 8.1.0 and later */
        zend_string *include_file;
        include_file = zend_strpprintf(0, "%s", class_file);
        zend_stream_init_filename_ex(&file_handle, include_file);
        ret = php_stream_open_for_zend_ex(&file_handle, USE_PATH|STREAM_OPEN_FOR_INCLUDE);
    #else
        ret = php_stream_open_for_zend_ex(class_file, &file_handle, USE_PATH|STREAM_OPEN_FOR_INCLUDE);
    #endif

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
            #if PHP_VERSION_ID >= 80100 /* if PHP version is 8.1.0 and later */
                zend_string_release(include_file);
            #endif
        } else {
            new_op_array = NULL;
            #if PHP_VERSION_ID >= 80100 /* if PHP version is 8.1.0 and later */
                zend_destroy_file_handle(&file_handle);
                zend_string_release(include_file);
            #else
                zend_file_handle_dtor(&file_handle);
            #endif
        }
        #if PHP_VERSION_ID >= 70300 /* if PHP version is 7.3.0 and later */
            zend_string_release_ex(opened_path, 0);
        #else
            zend_string_release(opened_path);
        #endif
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
static int autoload_psr0(zend_string *class, char *namespace, char *found)
{
    char *class_file, *class_name;
    int class_file_len, class_name_len;

    if (found) {
        class_name_len = (int)spprintf(&class_name, 0, "%s", found + 1);
    } else {
        class_name_len = (int)spprintf(&class_name, 0, "%s", ZSTR_VAL(class));
    }

    {
        char *ptr = class_name;
        char *end = ptr + class_name_len;

        while ((ptr = memchr(ptr, '_', (end - ptr))) != NULL) {
            *ptr = DEFAULT_SLASH;
        }
    }

    if (found) {
        class_file_len = (int)spprintf(&class_file, 0, "%.*s\\%s.php", (int)(found - namespace), namespace, class_name);
    } else {
        class_file_len = (int)spprintf(&class_file, 0, "%s.php", class_name);
    }

#if DEFAULT_SLASH != '\\'
    {
        char *ptr = class_file;
        char *end = ptr + class_file_len;

        while ((ptr = memchr(ptr, '\\', (end - ptr))) != NULL) {
            *ptr = DEFAULT_SLASH;
        }
    }
#endif

    include_class_file(class, class_file, class_file_len);

    efree(class_file);
    efree(class_name);

    return 0;
}
/* }}} */

/* {{{ */
static int autoload_psr4(zend_string *class, char *namespace, int namespace_len, char *found)
{
    char *e, *class_name, *class_file;
    int class_name_len, class_file_len;
    zval *base_path;

    initialize_hashtable();

    do
    {
        e = found - 1;
        namespace_len = found - namespace;

        class_name_len = (int)spprintf(&class_name, 0, "%s", ZSTR_VAL(class) + (namespace_len) + 1);

        base_path = zend_hash_str_find(AUTOLOAD_PSR_G(psr4_prefixes), namespace, namespace_len);

        if (base_path)
        {
#if DEFAULT_SLASH != '\\'
            {
                char *ptr = class_name;
                char *end = ptr + class_name_len;

                while ((ptr = memchr(ptr, '\\', (end - ptr))) != NULL) {
                    *ptr = DEFAULT_SLASH;
                }
            }
#endif

            if (Z_TYPE_P(base_path) == IS_ARRAY) {
                HashPosition position;
                zval *data;
                HashTable *prefixes = Z_ARR_P(base_path);

                for (zend_hash_internal_pointer_reset_ex(prefixes, &position);
                     (data = zend_hash_get_current_data_ex(prefixes, &position));
                     zend_hash_move_forward_ex(prefixes, &position))
                {
                        class_file_len = (int)spprintf(&class_file, 0, "%s%s.php", ZSTR_VAL(Z_STR_P(data)), class_name);

                        include_class_file(class, class_file, class_file_len);

                        efree(class_file);

                        if (AUTOLOAD_PSR_G(loaded)) {
                            efree(class_name);
                            return 0;
                        }
                }

            } else {
                class_file_len = (int)spprintf(&class_file, 0, "%s%s.php", ZSTR_VAL(Z_STR_P(base_path)), class_name);

                include_class_file(class, class_file, class_file_len);

                efree(class_file);

                if (AUTOLOAD_PSR_G(loaded)) {
                    efree(class_name);
                    return 0;
                }
            }
        }

        efree(class_name);
    }
    while ((found = (char *)zend_memrchr(namespace, '\\', (e - namespace))) != NULL);

    return 0;
}
/* }}} */

/* {{{ proto void autoload_register_psr4_prefix(string prefix, string path)
   Register a path for a namespace prefix to be used by the PSR-4 autoloader */
PHP_FUNCTION(autoload_register_psr4_prefix)
{
    zend_string *prefix, *path;
    zval *base_path;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_STR(prefix);
        Z_PARAM_STR(path);
    ZEND_PARSE_PARAMETERS_END();

    zval path_val;

    ZVAL_STR_COPY(&path_val, path);

    initialize_hashtable();

    base_path = zend_hash_find(AUTOLOAD_PSR_G(psr4_prefixes), prefix);

    if (!base_path) {
        zend_hash_update(AUTOLOAD_PSR_G(psr4_prefixes), prefix, &path_val);
    } else if (Z_TYPE_P(base_path) == IS_ARRAY) {
        zend_hash_next_index_insert(Z_ARR_P(base_path), &path_val);
    } else {
        HashTable* prefixes;

        ALLOC_HASHTABLE(prefixes);
        zend_hash_init(prefixes, 0, NULL, ZVAL_PTR_DTOR, 0);

        zval prefixes_arr;

        ZVAL_ARR(&prefixes_arr, prefixes);

        zend_hash_next_index_insert_new(prefixes, base_path);
        zend_hash_next_index_insert_new(prefixes, &path_val);

        zend_hash_update(AUTOLOAD_PSR_G(psr4_prefixes), prefix, &prefixes_arr);
    }
}
/* }}} */

/* {{{ proto void autoload_psr(string class)
   Autoload class based on the PSR-0 or PSR-4 standard */
PHP_FUNCTION(autoload_psr)
{
    zend_string *class;
    char *found, *namespace;
    int namespace_len;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STR(class);
    ZEND_PARSE_PARAMETERS_END();

    AUTOLOAD_PSR_G(loaded) = 0;

    namespace_len = (int)spprintf(&namespace, 0, "%s", ZSTR_VAL(class));

    found = (char *)zend_memrchr(namespace, '\\', namespace_len);

    autoload_psr0(class, namespace, found);

    if (!AUTOLOAD_PSR_G(loaded) && found) {
        autoload_psr4(class, namespace, namespace_len, found);
    }

    efree(namespace);
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

    AUTOLOAD_PSR_G(psr4_prefixes) = NULL;
    AUTOLOAD_PSR_G(loaded) = 0;

    zval function, ret, params[1];

    ZVAL_STRING(&function, "spl_autoload_register");
    ZVAL_STRING(&params[0], "autoload_psr");

    call_user_function(EG(function_table), NULL, &function, &ret, 1, params);

    zval_ptr_dtor(&function);
    zval_ptr_dtor(&ret);
    zval_ptr_dtor(&params[0]);

    return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(autoload_psr)
{
    if (AUTOLOAD_PSR_G(psr4_prefixes))
    {
        zend_hash_destroy(AUTOLOAD_PSR_G(psr4_prefixes));
        FREE_HASHTABLE(AUTOLOAD_PSR_G(psr4_prefixes));
    }

    return SUCCESS;
}
/* }}} */

/* {{{ PHP_GINIT_FUNCTION
 */
PHP_GINIT_FUNCTION(autoload_psr)
{
    autoload_psr_globals->psr4_prefixes = NULL;
    autoload_psr_globals->loaded = 0;
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

/**
 * Register the argument info for the functions
 */
ZEND_BEGIN_ARG_INFO_EX(arginfo_autoload_register_psr4_prefix, 0, 0, 2)
 ZEND_ARG_INFO(0, prefix)
 ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_autoload_psr, 0, 0, 1)
 ZEND_ARG_INFO(0, class)
ZEND_END_ARG_INFO()

/* {{{ autoload_psr_functions[]
 *
 * Every user visible function must have an entry in autoload_psr_functions[].
 */
const zend_function_entry autoload_psr_functions[] = {
    PHP_FE(autoload_register_psr4_prefix,    arginfo_autoload_register_psr4_prefix)
    PHP_FE(autoload_psr, arginfo_autoload_psr)
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
    NULL,
    NULL,
    STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */

#ifdef COMPILE_DL_AUTOLOAD_PSR
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE();
#endif
ZEND_GET_MODULE(autoload_psr);
#endif
