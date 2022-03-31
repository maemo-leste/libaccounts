lib_LTLIBRARIES = libaccounts.la
libaccounts_la_CFLAGS = $(LIBACCOUNTS_CFLAGS)
libaccounts_la_LDFLAGS =			\
	-Wl,--as-needed $(LIBACCOUNTS_LIBS)	\
	-Wl,--no-undefined -version-info 1:0:1

BUILT_SOURCES = 		\
	account-marshal.c	\
	account-marshal.h

libaccounts_la_SOURCES =	\
	account-plugin-loader.c	\
	account-plugin.c \
	accounts-list.c \
	account-item.c \
	account-service.c \
	account-plugin-manager.c \
	account-edit-context.c \
	account-dialog-context.c \
	account-wizard-context.c \
	account-marshal.c

account-marshal.c: account-marshal.list
	$(GLIB_GENMARSHAL) --prefix=account_marshal $<	--header --body	\
	--internal > xgen-$(@F)						\
	&& ( cmp -s xgen-$(@F) $@ || cp xgen-$(@F) $@ )			\
	&& rm -f xgen-$(@F)

account-marshal.h: account-marshal.list
	$(GLIB_GENMARSHAL) --prefix=account_marshal $< --header		\
	--internal > xgen-$(@F)						\
	&& ( cmp -s xgen-$(@F) $@ || cp xgen-$(@F) $@ )			\
	&& rm -f xgen-$(@F)

pkginclude_HEADERS = \
	account-dialog-context.h \
	account-edit-context.h \
	account-error.h \
	account-item.h \
	account-plugin.h \
	account-plugin-loader.h \
	account-plugin-manager.h \
	account-service.h \
	accounts-list.h \
	account-wizard-context.h

noinst_HEADERS = \
	account-marshal.h

CLEANFILES = $(BUILT_SOURCES)
MAINTAINERCLEANFILES = Makefile.in
