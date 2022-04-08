AC_INIT([libaccounts], [4.7])
AC_CANONICAL_SYSTEM
AC_CONFIG_MACRO_DIRS([m4])

AM_CONFIG_HEADER(config.h)

AM_MAINTAINER_MODE

AM_INIT_AUTOMAKE

AC_PROG_CC
AC_PROG_INSTALL
AM_PROG_LIBTOOL

AC_HEADER_STDC

AC_PATH_X
AC_PATH_XTRA
AC_SUBST(X_CFLAGS)

PKG_PROG_PKG_CONFIG

#+++++++++++++++
# Misc programs 
#+++++++++++++++

GTK_DOC_CHECK([1.3])

AC_PATH_PROG(GLIB_GENMARSHAL, glib-genmarshal)

PKG_CHECK_MODULES(LIBACCOUNTS, [hildon-1 libosso dbus-glib-1 gmodule-2.0])

#+++++++++++++++++++
# Directories setup
#+++++++++++++++++++

AC_ARG_WITH(accounts_dir, AS_HELP_STRING([--with-accounts-dir=<path>],[Directory for storing accounts plugins]))
if test -z "$with_accounts_dir" ; then
    pluginlibdir=$libdir/libaccounts-plugins
else
    pluginlibdir=$with_accounts_dir
fi
AC_SUBST(pluginlibdir)

AC_OUTPUT([
	Makefile
	libaccounts.pc
	src/Makefile
	doc/Makefile
])

