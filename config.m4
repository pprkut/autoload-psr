dnl config.m4 for extension autoload_psr

PHP_ARG_WITH(autoload_psr, for autoload_psr support,
Make sure that the comment is aligned:
[  --with-autoload_psr             Include autoload_psr support])

if test "$PHP_AUTOLOAD_PSR" != "no"; then
  dnl # --with-autoload_psr -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/autoload_psr.h"  # you most likely want to change this
  dnl if test -r $PHP_AUTOLOAD_PSR/$SEARCH_FOR; then # path given as parameter
  dnl   AUTOLOAD_PSR_DIR=$PHP_AUTOLOAD_PSR
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for autoload_psr files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       AUTOLOAD_PSR_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$AUTOLOAD_PSR_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the autoload_psr distribution])
  dnl fi

  dnl # --with-autoload_psr -> add include path
  dnl PHP_ADD_INCLUDE($AUTOLOAD_PSR_DIR/include)

  dnl # --with-autoload_psr -> check for lib and symbol presence
  dnl LIBNAME=autoload_psr # you may want to change this
  dnl LIBSYMBOL=autoload_psr # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $AUTOLOAD_PSR_DIR/$PHP_LIBDIR, AUTOLOAD_PSR_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_AUTOLOAD_PSRLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong autoload_psr lib version or lib not found])
  dnl ],[
  dnl   -L$AUTOLOAD_PSR_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(AUTOLOAD_PSR_SHARED_LIBADD)

  PHP_NEW_EXTENSION(autoload_psr, autoload_psr.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
