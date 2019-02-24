# GNU Mes --- Maxwell Equations of Software
# Copyright © 2018,2019 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
#
# This file is part of GNU Mes.
#
# GNU Mes is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or (at
# your option) any later version.
#
# GNU Mes is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Mes.  If not, see <http://www.gnu.org/licenses/>.

AUTOMAKE_OPTIONS = std-options subdir-objects

CLEANFILES =

EXTRA_DIST =					\
 BLURB						\
 BOOTSTRAP					\
 HACKING					\
 ROADMAP					\
 .dir-locals.el					\
 configure.sh					\
 guix.scm					\
 guix/git/mes.scm				\
 build-aux/manifest.scm				\
 build-aux/mes-snarf.scm			\
 build-aux/bootstrap.sh.in			\
 build-aux/indent.sh

bin_SCRIPTS =					\
  scripts/diff.scm

gendir = $(bindir)
gen_SCRIPTS =					\
 scripts/mesar					\
 scripts/mescc					\
 scripts/mescc.scm

CLEANFILES += $(gen_SCRIPTS)

bin_PROGRAMS = bin/mes

nodist_noinst_SCRIPTS = \
  pre-inst-env

CPPFLAGS = -I include -I include/$(mes_kernel)/$(mes_cpu)
CFLAGS = -g -fno-stack-protector #--std=c99
LDFLAGS = -g

crt1_o_CFLAGS = -g -fno-stack-protector

noinst_LIBRARIES =

bin_mes_SOURCES =				\
 src/gc.c					\
 src/hash.c					\
 src/lib.c					\
 src/math.c					\
 src/mes.c					\
 src/module.c					\
 src/posix.c					\
 src/reader.c					\
 src/string.c					\
 src/struct.c					\
 src/vector.c

bin_mes_LDADD =
noinst_LIBRARIES += lib/libmes.a
lib_libmes_a_SOURCES = $(libmes_a_SOURCES)

lib_libmes_a_CPPFLAGS = -I lib

noinst_libdir =
include include/local.mk
include lib/local.mk

crt1.o: lib/linux/$(mes_cpu)-mes-$(compiler)/crt1.c
	$(AM_V_CC)depbase=`echo $@ | sed 's|[^/]*$$|$(DEPDIR)/&|;s|\.o$$||'`;\
	$(COMPILE) $(CPPFLAGS) $(crt1_o_CFLAGS) -c $<

if system_libc
CPPFLAGS += -D SYSTEM_LIBC=1

bin_mes_LDADD += lib/libmes.a

else !system_libc

CPPFLAGS += -nostdinc -fno-builtin

noinst_LIBRARIES += lib/libc.a
lib_libc_a_SOURCES = $(libc_a_SOURCES)
lib_libc_a_CPPFLAGS = -I lib

bin_mes_LDFLAGS = -nostdlib
bin_mes_LDADD += crt1.o
bin_mes_LDADD += lib/libc.a

endif !system_libc

## -------##
## Mescc. ##
## -------##

FLAGS_TO_PASS =					\
 "DESTDIR=$(DESTDIR)"				\
 "prefix=$(prefix)"				\
 "bindir=$(bindir)"				\
 "pkgdatadir=$(pkgdatadir)"			\
 "V=$(V)"

MESCC_FLAGS_TO_PASS = $(FLAGS_TO_PASS)

GCC_FLAGS_TO_PASS =				\
 $(FLAGS_TO_PASS)				\
 "libdir=$(libdir)"

install: install-exec
install: install-data

all: all-$(compiler)-lib
clean-local: clean-$(compiler)-lib
distclean-local: distclean-$(compiler)-lib
check: check-$(compiler)-lib
install: install-$(compiler)-lib
uninstall-hook: uninstall-$(compiler)-lib
configure-$(compiler)-lib: $(compiler)-lib/crt1.o
all-$(compiler)-lib: configure-$(compiler)-lib
check-$(compiler)-lib: all-$(compiler)-lib

gcc-lib/crt1.o: gcc-lib/$(mes_cpu)-mes-gcc/crt1.o
	cp $< $@

gcc-lib/$(mes_cpu)-mes-gcc/crt1.o: lib/linux/$(mes_cpu)-mes-gcc/crt1.c | gcc-lib/$(mes_cpu)-mes-gcc
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

gcc-lib:
	mkdir -p $@

gcc-lib/$(mes_cpu)-mes-gcc:
	mkdir -p $@

all-gcc-lib:
	$(am__v_at_$(V))if test -f gcc-lib/Makefile; then	\
	  $(MAKE) -C gcc-lib $(GCC_FLAGS_TO_PASS) all;		\
	else							\
	  true;							\
	fi

check-gcc-lib:
	$(am__v_at_$(V))if test -f gcc-lib/Makefile; then						\
	  $(MAKE) -C gcc-lib $(GCC_FLAGS_TO_PASS) CC="$(CC) -I $(abs_top_srcdir)/include" check;	\
	else												\
	  true;												\
	fi

clean-gcc-lib:
	$(am__v_at_$(V))if test -f gcc-lib/Makefile; then	\
	  $(MAKE) -C gcc-lib $(GCC_FLAGS_TO_PASS) clean;	\
	else							\
	  true;							\
	fi

distclean-gcc-lib:
	$(am__v_at_$(V))if test -f gcc-lib/Makefile; then	\
	  $(MAKE) -C gcc-lib $(GCC_FLAGS_TO_PASS) distclean;	\
	else							\
	  true;							\
	fi

install-gcc-lib:
	$(am__v_at_$(V))if test -f gcc-lib/Makefile; then	\
	  $(MAKE) -C gcc-lib $(GCC_FLAGS_TO_PASS) install;	\
	else							\
	  true;							\
	fi

all: all-mescc-lib
clean-local: clean-mescc-lib
distclean-local: distclean-mescc-lib
check: check-mescc-lib
install: install-mescc-lib
uninstall-hook: uninstall-mescc-lib
configure-mescc-lib: mescc-lib/crt1.o
all-mescc-lib: configure-mescc-lib
check-mescc-lib: all-mescc-lib

mescc-lib:
	mkdir -p $@

mescc-lib/$(mes_cpu)-mes-mescc:
	mkdir -p $@

mescc-lib/crt1.o: mescc-lib/$(mes_cpu)-mes-mescc/crt1.o
	cp $< $@

mescc-lib/$(mes_cpu)-mes-mescc/crt1.o: lib/linux/$(mes_cpu)-mes-mescc/crt1.c src/mes scripts/mescc | mescc-lib/$(mes_cpu)-mes-mescc
	./pre-inst-env mescc -m $(mes_bits) -c -o $@ $<

configure-gcc-lib:
	$(am__v_at_$(V))if test -f gcc-lib/Makefile; then			\
	  true;									\
	else									\
	  set -x;								\
	  origdir=`echo $@ | sed -e 's/configure-gcc-//'`;			\
	  dir=`echo $@ | sed -e 's/configure-//'`;				\
	  if [ "$(srcdir)" = "." ] ; then					\
	    srcdiroption="--srcdir=.";						\
	  else									\
	    srcdiroption="--srcdir=$(top_srcdir)/$${origdir}";			\
	  fi;									\
	  echo Configuring in $${dir};						\
	  configure=`cd $(srcdir); pwd`/$$origdir/configure;			\
	  cd $${dir};								\
	    GUILE_LOAD_PATH="$(GUILE_LOAD_PATH)";				\
	    GUILE_LOAD_COMPILED_PATH="$(GUILE_LOAD_COMPILED_PATH)";		\
	    PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)";				\
	    $${configure}							\
	      --build=$(build)							\
	      --host=$(build)							\
	      --enable-silent-rules=$(disable_silent_rules)			\
	      --with-courage=$(mes_courageous)					\
	      --with-system-libc=$(mes_libc)					\
	      AR="$(AR)"							\
	      CC="$(CC) -I $(abs_top_srcdir)/include -L $(abs_top_builddir)"	\
	      CFLAGS="$(CFLAGS) -I $(abs_top_srcdir)/include"			\
	      LDFLAGS="$(LDFLAGS) -L $(abs_top_builddir)";			\
        fi

configure-mescc-lib:
	$(am__v_at_$(V))if test -f mescc-lib/Makefile; then		\
	  true;								\
	else								\
	  set -x;							\
	  origdir=`echo $@ | sed -e 's/configure-mescc-//'`;		\
	  dir=`echo $@ | sed -e 's/configure-//'`;			\
	  if [ "$(srcdir)" = "." ] ; then				\
	    srcdiroption="--srcdir=.";					\
	  else								\
	    srcdiroption="--srcdir=$(top_srcdir)/$${origdir}";		\
	  fi;								\
	  echo Configuring in $${dir};					\
	  configure=`cd $(srcdir); pwd`/$$origdir/configure;		\
	  cd $${dir};							\
	    GUILE_LOAD_PATH="$(GUILE_LOAD_PATH)";			\
	    GUILE_LOAD_COMPILED_PATH="$(GUILE_LOAD_COMPILED_PATH)";	\
	    PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)";			\
	    $${configure}						\
	      --build=$(build)						\
	      --host=$(host)						\
	      --disable-dependency-tracking				\
	      --enable-silent-rules=$(disable_silent_rules)		\
	      --with-courage=$(mes_courageous)				\
	      --with-system-libc=mes					\
	      AR="$(abs_top_builddir)/pre-inst-env mesar"		\
	      RANLIB=true						\
	      CC="$(abs_top_builddir)/pre-inst-env mescc		\
		-m $(mes_bits)						\
		-I $(abs_top_srcdir)/include				\
		-L $(abs_top_builddir)"					\
	      CFLAGS="-m $(mes_bits) -I ../$(srcdir)/include"		\
	      LDFLAGS="-L $(abs_top_builddir)/mescc-lib";		\
        fi

all-mescc-lib:
	$(am__v_at_$(V))if test -f mescc-lib/Makefile; then	\
	  $(MAKE) -C mescc-lib $(MESCC_FLAGS_TO_PASS) all;	\
	else							\
	  true;							\
	fi

clean-mescc-lib:
	$(am__v_at_$(V))if test -f mescc-lib/Makefile; then	\
	  $(MAKE) -C mescc-lib $(MESCC_FLAGS_TO_PASS) clean;	\
	else							\
	  true;							\
	fi

distclean-mescc-lib:
	$(am__v_at_$(V))if test -f mescc-lib/Makefile; then		\
	  $(MAKE) -C mescc-lib $(MESCC_FLAGS_TO_PASS) distclean;	\
	else								\
	  true;								\
	fi

check-mescc-lib:
	$(am__v_at_$(V))if test -f mescc-lib/Makefile; then	\
	  $(MAKE) -C mescc-lib $(MESCC_FLAGS_TO_PASS) check;	\
	else							\
	  true;							\
	fi

install-mescc-lib:
	$(am__v_at_$(V))if test -f mescc-lib/Makefile; then	\
	  $(MAKE) -C mescc-lib $(MESCC_FLAGS_TO_PASS) install;	\
	else							\
	  true;							\
	fi

uninstall-mescc-lib:
	$(am__v_at_$(V))if test -f mescc-lib/Makefile; then		\
	  $(MAKE) -C mescc-lib $(MESCC_FLAGS_TO_PASS) uninstall;	\
	else								\
	  true;								\
	fi

## -------------- ##
## Documentation. ##
## -------------- ##

DOT_FILES =					\
  doc/images/gcc-mesboot-graph.dot

infoimagedir = $(infodir)/images
dist_infoimage_DATA =				\
  $(DOT_FILES:%.dot=%.png)

info_TEXINFOS = doc/mes.texi

doc_TEXINFOS =\
 doc/mes.texi\
 doc/fdl-1.3.texi

EXTRA_DIST +=\
 doc/gendocs_template\
 doc/images/README

.dot.png:
	$(AM_V_DOT)$(DOT) -Tpng $(DOT_OPTIONS) < "$<" > "$(srcdir)/$@.tmp"; \
	mv "$(srcdir)/$@.tmp" "$(srcdir)/$@"

.dot.pdf:
	$(AM_V_DOT)$(DOT) -Tpdf $(DOT_OPTIONS) < "$<" > "$(srcdir)/$@.tmp"; \
	mv "$(srcdir)/$@.tmp" "$(srcdir)/$@"

.dot.eps:
	$(AM_V_DOT)$(DOT) -Teps $(DOT_OPTIONS) < "$<" > "$(srcdir)/$@.tmp"; \
	mv "$(srcdir)/$@.tmp" "$(srcdir)/$@"

.png.eps:
	$(AM_V_GEN)convert "$<" "$@-tmp.eps"; \
	mv "$@-tmp.eps" "$@"

dist_man1_MANS =				\
  $(srcdir)/doc/mes.1				\
  $(srcdir)/doc/mescc.1

gen_man =						\
  LANGUAGE= $(top_builddir)/pre-inst-env $(HELP2MAN)	\
  $(HELP2MANFLAGS)

HELP2MANFLAGS = --source=GNU --info-page=$(PACKAGE_TARNAME)

$(srcdir)/doc/mes.1: src/mes
	-$(AM_V_HELP2MAN)$(gen_man) --output="$@" $<

$(srcdir)/doc/mescc.1: scripts/mescc
	-$(AM_V_HELP2MAN)$(gen_man) --output="$@" $<

mes_SOURCES =					\
 mes/module/mes/base.mes			\
 mes/module/mes/boot-0.scm			\
 mes/module/mes/boot-00.scm			\
 mes/module/mes/boot-01.scm			\
 mes/module/mes/boot-02.scm			\
 mes/module/mes/boot-03.scm			\
 mes/module/mes/catch.mes			\
 mes/module/mes/display.mes			\
 mes/module/mes/fluids.mes			\
 mes/module/mes/getopt-long.mes			\
 mes/module/mes/guile.mes			\
 mes/module/mes/lalr.mes			\
 mes/module/mes/lalr.scm			\
 mes/module/mes/let.mes				\
 mes/module/mes/match.mes			\
 mes/module/mes/match.scm			\
 mes/module/mes/mescc.mes			\
 mes/module/mes/misc.mes			\
 mes/module/mes/module.mes			\
 mes/module/mes/optargs.mes			\
 mes/module/mes/peg.mes				\
 mes/module/mes/peg/cache.scm			\
 mes/module/mes/peg/codegen.scm			\
 mes/module/mes/peg/simplify-tree.scm		\
 mes/module/mes/peg/string-peg.scm		\
 mes/module/mes/peg/using-parsers.scm		\
 mes/module/mes/pmatch.mes			\
 mes/module/mes/pmatch.scm			\
 mes/module/mes/posix.mes			\
 mes/module/mes/pretty-print.mes		\
 mes/module/mes/pretty-print.scm		\
 mes/module/mes/psyntax-0.mes			\
 mes/module/mes/psyntax-1.mes			\
 mes/module/mes/psyntax.mes			\
 mes/module/mes/psyntax.pp.header		\
 mes/module/mes/psyntax.pp			\
 mes/module/mes/psyntax.ss			\
 mes/module/mes/quasiquote.mes			\
 mes/module/mes/quasisyntax.mes			\
 mes/module/mes/quasisyntax.scm			\
 mes/module/mes/repl.mes			\
 mes/module/mes/scm.mes				\
 mes/module/mes/syntax.mes			\
 mes/module/mes/syntax.scm			\
 mes/module/mes/test.mes			\
 mes/module/mes/tiny-0.mes			\
 mes/module/mes/type-0.mes			\
 mes/module/mescc.mes				\
 mes/module/mescc/M1.mes			\
 mes/module/mescc/as.mes			\
 mes/module/mescc/bytevectors.mes		\
 mes/module/mescc/compile.mes			\
 mes/module/mescc/i386/as.mes			\
 mes/module/mescc/i386/info.mes			\
 mes/module/mescc/info.mes			\
 mes/module/mescc/mescc.mes			\
 mes/module/mescc/preprocess.mes		\
 mes/module/mescc/x86_64/as.mes			\
 mes/module/mescc/x86_64/info.mes		\
 mes/module/nyacc/lalr.mes			\
 mes/module/nyacc/lang/c99/cpp.mes		\
 mes/module/nyacc/lang/c99/parser.mes		\
 mes/module/nyacc/lang/c99/pprint.mes		\
 mes/module/nyacc/lang/calc/parser.mes		\
 mes/module/nyacc/lang/sx-util.mes		\
 mes/module/nyacc/lang/util.mes			\
 mes/module/nyacc/lex.mes			\
 mes/module/nyacc/parse.mes			\
 mes/module/nyacc/util.mes			\
 mes/module/nyacc/version.mes			\
 mes/module/rnrs/arithmetic/bitwise.mes		\
 mes/module/srfi/srfi-0.mes			\
 mes/module/srfi/srfi-1.mes			\
 mes/module/srfi/srfi-1.scm			\
 mes/module/srfi/srfi-13.mes			\
 mes/module/srfi/srfi-14.mes			\
 mes/module/srfi/srfi-16.mes			\
 mes/module/srfi/srfi-16.scm			\
 mes/module/srfi/srfi-26.mes			\
 mes/module/srfi/srfi-26.scm			\
 mes/module/srfi/srfi-43.mes			\
 mes/module/srfi/srfi-8.mes			\
 mes/module/srfi/srfi-9-struct.mes		\
 mes/module/srfi/srfi-9-vector.mes		\
 mes/module/srfi/srfi-9/gnu-struct.mes		\
 mes/module/srfi/srfi-9/gnu-vector.mes		\
 mes/module/sxml/xpath.mes			\
 mes/module/sxml/xpath.scm

EXTRA_DIST += $(mes_SOURCES)

mescc_SOURCES =					\
 module/mes/getopt-long.scm			\
 module/mes/guile.scm				\
 module/mes/misc.scm				\
 module/mes/test.scm				\
 module/mescc.scm				\
 module/mescc/M1.scm				\
 module/mescc/as.scm				\
 module/mescc/bytevectors.scm			\
 module/mescc/compile.scm			\
 module/mescc/i386/as.scm			\
 module/mescc/i386/info.scm			\
 module/mescc/info.scm				\
 module/mescc/mescc.scm				\
 module/mescc/preprocess.scm			\
 module/mescc/x86_64/as.scm			\
 module/mescc/x86_64/info.scm

EXTRA_DIST += $(mescc_SOURCES)

moddir=$(datadir)/guile/site/$(GUILE_EFFECTIVE_VERSION)
godir=$(libdir)/guile/$(GUILE_EFFECTIVE_VERSION)/ccache

mesdir = $(pkgdatadir)/module/../..
nobase_mes_DATA = $(mes_SOURCES)

mesccdir = $(moddir)
nobase_mescc_DATA = $(mescc_SOURCES)

if HAVE_GUILE
GOBJECTS = $(mescc_SOURCES:%.scm=%.go)
nobase_go_DATA = $(GOBJECTS)

guile_install_go_files = install-nobase_nodist_guileobjectDATA
$(guile_install_go_files): install-nobase_dist_guilemoduleDATA

GUILE_WARNINGS = -Wunbound-variable -Warity-mismatch -Wformat
SUFFIXES = .scm .go
.scm.go:
	$(AM_V_GEN)$(top_builddir)/pre-inst-env \
	    $(GUILE_TOOLS) compile $(GUILE_WARNINGS) -o "$@" "$<"
endif HAVE_GUILE

all-go: $(GOBJECTS)
clean-go:
	rm -f $(GOBJECTS)

## -------------- ##
## Check.         ##
## -------------- ##

TESTS =
BROKEN_TESTS =

TEST_EXTENSIONS = .scm .test
SCM_LOG_COMPILER = $(top_builddir)/pre-inst-env $(top_srcdir)/build-aux/test-boot.sh
TEST_LOG_DRIVER = $(top_builddir)/pre-inst-env $(top_srcdir)/build-aux/test-driver

semantic_TESTS =					\
 scaffold/boot/00-zero.scm				\
 scaffold/boot/01-true.scm				\
 scaffold/boot/00-zero.scm				\
 scaffold/boot/01-true.scm				\
 scaffold/boot/02-identifier.scm			\
 scaffold/boot/02-symbol.scm				\
 scaffold/boot/03-big-string.scm			\
 scaffold/boot/03-string.scm				\
 scaffold/boot/04-cons.scm				\
 scaffold/boot/04-quote.scm				\
 scaffold/boot/05-big-list.scm				\
 scaffold/boot/05-list-list.scm				\
 scaffold/boot/05-list.scm				\
 scaffold/boot/06-tick.scm				\
 scaffold/boot/07-if.scm				\
 scaffold/boot/08-if-if.scm				\
 scaffold/boot/10-cons.scm				\
 scaffold/boot/11-list.scm				\
 scaffold/boot/11-vector.scm				\
 scaffold/boot/12-car.scm				\
 scaffold/boot/13-cdr.scm				\
 scaffold/boot/14-exit.scm				\
 scaffold/boot/15-display.scm				\
 scaffold/boot/16-if-eq-quote.scm			\
 scaffold/boot/17-equal2.scm				\
 scaffold/boot/17-memq-keyword.scm			\
 scaffold/boot/17-memq.scm				\
 scaffold/boot/17-open-input-string.scm			\
 scaffold/boot/17-string-append.scm			\
 scaffold/boot/17-string-equal.scm			\
 scaffold/boot/20-define-quote.scm			\
 scaffold/boot/20-define-quoted.scm			\
 scaffold/boot/20-define.scm				\
 scaffold/boot/21-define-procedure.scm			\
 scaffold/boot/22-define-procedure-2.scm		\
 scaffold/boot/23-begin.scm				\
 scaffold/boot/24-begin-define.scm			\
 scaffold/boot/25-begin-define-2.scm			\
 scaffold/boot/26-begin-define-later.scm		\
 scaffold/boot/26-define-define.scm			\
 scaffold/boot/27-lambda-define.scm			\
 scaffold/boot/28-define-define.scm			\
 scaffold/boot/29-lambda-define.scm			\
 scaffold/boot/2a-lambda-lambda.scm			\
 scaffold/boot/2b-define-lambda.scm			\
 scaffold/boot/2c-define-lambda-recurse.scm		\
 scaffold/boot/2d-compose.scm				\
 scaffold/boot/2d-define-lambda-set.scm			\
 scaffold/boot/2e-define-first.scm			\
 scaffold/boot/2f-define-second-lambda.scm		\
 scaffold/boot/2f-define-second.scm			\
 scaffold/boot/2g-vector.scm				\
 scaffold/boot/30-capture.scm				\
 scaffold/boot/31-capture-define.scm			\
 scaffold/boot/32-capture-modify-close.scm		\
 scaffold/boot/33-procedure-override-close.scm		\
 scaffold/boot/34-cdr-override-close.scm		\
 scaffold/boot/35-closure-modify.scm			\
 scaffold/boot/36-closure-override.scm			\
 scaffold/boot/37-closure-lambda.scm			\
 scaffold/boot/38-simple-format.scm			\
 scaffold/boot/39-global-define-override.scm		\
 scaffold/boot/3a-global-define-lambda-override.scm	\
 scaffold/boot/40-define-macro.scm			\
 scaffold/boot/41-when.scm				\
 scaffold/boot/42-if-when.scm				\
 scaffold/boot/43-or.scm				\
 scaffold/boot/44-or-if.scm				\
 scaffold/boot/45-pass-if.scm				\
 scaffold/boot/46-report.scm				\
 scaffold/boot/47-pass-if-eq.scm			\
 scaffold/boot/48-let.scm				\
 scaffold/boot/49-macro-override.scm			\
 scaffold/boot/4a-define-macro-define-macro.scm		\
 scaffold/boot/4b-define-macro-define.scm		\
 scaffold/boot/4c-quasiquote.scm			\
 scaffold/boot/4d-let-map.scm				\
 scaffold/boot/4e-let-global.scm			\
 scaffold/boot/4f-string-split.scm			\
 scaffold/boot/50-keyword.scm				\
 scaffold/boot/50-make-string.scm			\
 scaffold/boot/50-primitive-load.scm			\
 scaffold/boot/50-string-join.scm			\
 scaffold/boot/53-closure-display.scm			\
 scaffold/boot/60-let-syntax.scm			\
 scaffold/boot/call-cc.scm				\
 scaffold/boot/memory.scm				\
 scaffold/boot/numbers.scm

TESTS += $(semantic_TESTS)

BROKEN_TESTS +=					\
 scaffold/boot/51-module.scm			\
 scaffold/boot/52-define-module.scm		\
 scaffold/boot/60-let-syntax-expanded.scm

functional_TESTS =				\
 tests/boot.test				\
 tests/read.test				\
 tests/srfi-0.test				\
 tests/macro.test				\
 tests/perform.test				\
 tests/base.test				\
 tests/quasiquote.test				\
 tests/let.test					\
 tests/closure.test				\
 tests/scm.test					\
 tests/display.test				\
 tests/cwv.test					\
 tests/math.test				\
 tests/vector.test				\
 tests/srfi-1.test				\
 tests/srfi-9.test				\
 tests/srfi-13.test				\
 tests/srfi-14.test				\
 tests/srfi-43.test				\
 tests/optargs.test				\
 tests/fluids.test				\
 tests/catch.test				\
 tests/getopt-long.test				\
 tests/guile.test				\
 tests/syntax.test				\
 tests/let-syntax.test				\
 tests/pmatch.test				\
 tests/posix.test				\
 tests/match.test				\
 tests/psyntax.test

TESTS += $(functional_TESTS)

EXTRA_DIST += $(TESTS)

check-guile:
	MES=guile $(MAKE) check

## -------------- ##
## Silent rules.  ##
## -------------- ##

AM_V_DOT = $(AM_V_DOT_$(V))
AM_V_DOT_ = $(AM_V_DOT_$(AM_DEFAULT_VERBOSITY))
AM_V_DOT_0 = @echo "  DOT     " $@;

AM_V_HELP2MAN = $(AM_V_HELP2MAN_$(V))
AM_V_HELP2MAN_ = $(AM_V_HELP2MAN_$(AM_DEFAULT_VERBOSITY))
AM_V_HELP2MAN_0 = @echo "  HELP2MAN" $@;

## -------------- ##
## Web site.      ##
## -------------- ##

WEB_IMAGES:=					\
 doc/images/gcc-mesboot-graph.eps		\
 doc/images/gcc-mesboot-graph.pdf		\
 doc/images/gcc-mesboot-graph.png

web: info $(WEB_IMAGES)
	GENDOCS_TEMPLATE_DIR=doc build-aux/gendocs.sh -I doc -s ${srcdest}doc/mes.texi -o doc/web/manual --email bug-mes@gnu.org mes "GNU Mes manual"

## ---------------- ##
## Release process. ##
## ---------------- ##

#  0. doc: Release update, Release commit, test guix bootstrap
#  1. make tag
#  2. make sign-dist
#  3. make release

guix-build:
	./pre-inst-env $(GUIX) build mes@$(VERSION) --with-source=$(DIST_ARCHIVES)

# Reproducible tarball
# Be friendly to Debian; avoid using EPOCH
MTIME = $(shell git show HEAD --format=%ct --no-patch 2>/dev/null || echo 0)
TAR = tar --sort=name --mtime=@$(MTIME) --owner=0 --group=0 --numeric-owner --mode=go=rX,u+rw,a-s

tag:
	git tag -s v$(PACKAGE_VERSION) -m "$(PACKAGE_NAME) $(PACKAGE_VERSION)."

sign-dist: dist
	gpg -a --output $(distdir).tar.gz.sig --detach-sig $(distdir).tar.gz

dist-hook: $(distdir)/ChangeLog gen-tarball-version

gen-tarball-version:
	echo $(VERSION) > "$(distdir)/.tarball-version"

gen-ChangeLog $(distdir)/ChangeLog: config.status
	$(AM_V_GEN)if test -e .git; then		\
	  $(top_srcdir)/build-aux/gitlog-to-changelog	\
	    > $(distdir)/cl-t;				\
	  rm -f $(distdir)/ChangeLog;			\
	  mv $(distdir)/cl-t $(distdir)/ChangeLog;	\
	fi

release: update-hash | gen-announce guix-build

GPG_KEY_ID:=1A858392E331EAFDB8C27FFBF3C1A0D9C1D65273
gen-announce:
	../gnulib/build-aux/announce-gen\
	    --release-type=beta\
	    --package-name=mes\
	    --previous-version='0.19 '\
            --current-version=$(VERSION)\
	    --gpg-key-id=$(GPG_KEY_ID)\
	    --url-directory=https://ftp.gnu.org/gnu/mes\
	    --news=NEWS\
            > doc/announce/ANNOUNCE-$(VERSION)-

## -------------- ##
## Help.          ##
## -------------- ##

define HELP_TOP
Usage: make [OPTION]... [TARGET]...

Main and non-standard targets:
  all             update everything
  all-go          update .go files
  dist            update $(DIST_ARCHIVES)
  doc             update documentation
  check           run unit tests
  clean           clean build tree
  clean-go        clean .go files
  info            update info documentation
  install         install in $(DESTDIR)$(prefix)
  install-info    install info docs in $(DESTDIR)$(infodir)
  release         dist and tag
  uninstall       uninstall from $(DESTDIR)$(prefix)
endef
export HELP_TOP
help:
	@echo "$$HELP_TOP"
