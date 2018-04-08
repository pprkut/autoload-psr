
#ifndef PHP_AUTOLOAD_PSR_H
#define PHP_AUTOLOAD_PSR_H

extern zend_module_entry autoload_psr_module_entry;
#define phpext_autoload_psr_ptr &autoload_psr_module_entry

#define PHP_AUTOLOAD_PSR_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#    define PHP_AUTOLOAD_PSR_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#    define PHP_AUTOLOAD_PSR_API __attribute__ ((visibility("default")))
#else
#    define PHP_AUTOLOAD_PSR_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_BEGIN_MODULE_GLOBALS(autoload_psr)
    HashTable  *psr4_prefixes;
ZEND_END_MODULE_GLOBALS(autoload_psr)

/* Always refer to the globals in your function as AUTOLOAD_PSR_G(variable).
   You are encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/
#define AUTOLOAD_PSR_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(autoload_psr, v)

#if defined(ZTS) && defined(COMPILE_DL_AUTOLOAD_PSR)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif    /* PHP_AUTOLOAD_PSR_H */

