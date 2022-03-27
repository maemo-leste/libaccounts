MAINTAINERCLEANFILES 				= Makefile.in \
						  version.xml \
						  tmpl/*.sgml \
						  gtk-doc.make

DOC_MODULE					= libaccounts

DOC_MAIN_SGML_FILE				= $(DOC_MODULE)-docs.sgml

#SCAN_OPTIONS					=--deprecated-guards="HILDON_DISABLE_DEPRECATED"

SCANOBJ_OPTIONS					=--type-init-func="gtk_type_init(0)"

DOC_SOURCE_DIR					= $(top_srcdir)/src
DOC_BUILD_DIR 					= $(top_builddir)/libaccounts


HFILE_GLOB					= $(top_srcdir)/src/*.h

CFILE_GLOB					= $(top_srcdir)/src/*.c

IGNORE_HFILES 					= account-dialog-context.h

AM_CPPFLAGS 					= $(LIBACCOUNTS_CFLAGS) -I$(top_srcdir)/src

MKDB_OPTIONS					=--sgml-mode --output-format=xml

GTKDOC_CFLAGS 					= $(LIBACCOUNTS_CFLAGS) $(MAEMO_GTK)

GTKDOC_LIBS 					= $(LIBACCOUNTS_LIBS) $(top_builddir)/src/libaccounts.la

version.xml: ../configure.ac
	@echo $(VERSION) > version.xml

html: html-build.stamp

FIXXREF_OPTIONS					=

include $(top_srcdir)/doc/gtk-doc.make

