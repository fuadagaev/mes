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

CLEANFILES = \
 crt1.S\
 libc+gnu.S\
 libc+tcc.S\
 libc-mini.S\
 libc.S\
 libtcc1.S\
 x86-mes-mescc/crt1.S\
 x86_64-mes-mescc/crt1.S

EXTRA_DIST =
noinst_libdir = $(prefix)/lib

include local.mk

libdir = $(prefix)/lib/$(mes_system)

CPPFLAGS =\
 -I $(srcdir)/../include\
 -I $(srcdir)/../include/$(mes_kernel)/$(mes_cpu)\
 -I $(srcdir)/../lib

if system_libc

CFLAGS = -fno-builtin
LDFLAGS = -L ../lib

else !system_libc

CFLAGS =					\
 -nostdinc					\
 -nostdlib					\
 -fno-builtin

endif !system_libc

lib_LIBRARIES =

if !system_libc
lib_LIBRARIES += libc-mini.a
lib_LIBRARIES += libc.a
lib_LIBRARIES += libc+tcc.a
lib_LIBRARIES += libtcc1.a
lib_LIBRARIES += libc+gnu.a
endif !system_libc

TEST_EXTENSIONS = .c
C_LOG_COMPILER = $(top_srcdir)/../build-aux/test-c.sh

all: arch-libs

$(mes_system):
	$(am__v_at_$(V))mkdir -p $@

arch-libs: $(lib_LIBRARIES:%=$(mes_system)/%)

$(mes_system)/%.a: %.a | $(mes_system)
	$(am__v_at_$(V))mkdir -p $(@D)
	$(am__v_at_$(V))if test -f $(<:%.a=%.S); then cp -f $(<:%.a=%.S) $(@:%.a=%.S); fi
	$(am__v_at_$(V))if test -f $(<:%.a=%.s); then cp -f $(<:%.a=%.s) $(@:%.a=%.s); fi
	$(am__v_at_$(V))cp -f $< $@

export CC
export CFLAGS

TESTS =							\
 tests/scaffold/t.c					\
 tests/scaffold/00-exit-0.c				\
 tests/scaffold/01-return-0.c				\
 tests/scaffold/02-return-1.c				\
 tests/scaffold/03-call.c				\
 tests/scaffold/04-call-0.c				\
 tests/scaffold/05-call-1.c				\
 tests/scaffold/06-call-2.c				\
 tests/scaffold/06-call-not-1.c				\
 tests/scaffold/06-call-string.c			\
 tests/scaffold/06-call-variable.c			\
 tests/scaffold/06-not-call-1.c				\
 tests/scaffold/06-return-void.c			\
 tests/scaffold/07-include.c				\
 tests/scaffold/08-assign.c				\
 tests/scaffold/08-assign-global.c			\
 tests/scaffold/08-assign-negative.c			\
 tests/scaffold/10-if-0.c				\
 tests/scaffold/11-if-1.c				\
 tests/scaffold/12-if-eq.c				\
 tests/scaffold/13-if-neq.c				\
 tests/scaffold/14-if-goto.c				\
 tests/scaffold/15-if-not-f.c				\
 tests/scaffold/16-if-t.c				\
 tests/scaffold/17-compare-and.c			\
 tests/scaffold/17-compare-and-or.c			\
 tests/scaffold/17-compare-assign.c			\
 tests/scaffold/17-compare-call.c			\
 tests/scaffold/17-compare-char.c			\
 tests/scaffold/17-compare-ge.c				\
 tests/scaffold/17-compare-gt.c				\
 tests/scaffold/17-compare-le.c				\
 tests/scaffold/17-compare-lt.c				\
 tests/scaffold/17-compare-or.c				\
 tests/scaffold/17-compare-unsigned-char-le.c		\
 tests/scaffold/17-compare-unsigned-ge.c		\
 tests/scaffold/17-compare-unsigned-gt.c		\
 tests/scaffold/17-compare-unsigned-le.c		\
 tests/scaffold/17-compare-unsigned-long-le.c		\
 tests/scaffold/17-compare-unsigned-lt.c		\
 tests/scaffold/17-compare-unsigned-short-le.c		\
 tests/scaffold/18-assign-shadow.c			\
 tests/scaffold/20-while.c				\
 tests/scaffold/21-char-array.c				\
 tests/scaffold/21-char-array-simple.c			\
 tests/scaffold/22-while-char-array.c			\
 tests/scaffold/23-global-pointer-init.c		\
 tests/scaffold/23-global-pointer-init-null.c		\
 tests/scaffold/23-global-pointer-pointer-ref.c		\
 tests/scaffold/23-global-pointer-ref.c			\
 tests/scaffold/23-pointer.c				\
 tests/scaffold/23-pointer-sub.c			\
 tests/scaffold/32-call-wrap.c				\
 tests/scaffold/32-compare.c				\
 tests/scaffold/33-and-or.c				\
 tests/scaffold/34-pre-post.c				\
 tests/scaffold/35-compare-char.c			\
 tests/scaffold/36-compare-arithmetic.c			\
 tests/scaffold/37-compare-assign.c			\
 tests/scaffold/38-compare-call-2.c			\
 tests/scaffold/38-compare-call-3.c			\
 tests/scaffold/38-compare-call.c			\
 tests/scaffold/40-if-else.c				\
 tests/scaffold/41-ternary.c				\
 tests/scaffold/42-goto-label.c				\
 tests/scaffold/43-for-do-while.c			\
 tests/scaffold/44-switch-body-fallthrough.c		\
 tests/scaffold/44-switch.c				\
 tests/scaffold/44-switch-fallthrough.c			\
 tests/scaffold/45-void-call.c				\
 tests/scaffold/46-function-static.c			\
 tests/scaffold/47-function-expression.c		\
 tests/scaffold/48-global-static.c			\
 tests/scaffold/51-pointer-sub.c			\
 tests/scaffold/54-argc.c				\
 tests/scaffold/54-argv.c				\
 tests/scaffold/55-char-array.c				\
 tests/scaffold/60-math.c				\
 tests/scaffold/61-array.c				\
 tests/scaffold/62-array.c				\
 tests/scaffold/63-struct-array-assign.c		\
 tests/scaffold/63-struct-array.c			\
 tests/scaffold/63-struct-array-compare.c		\
 tests/scaffold/63-struct-assign.c			\
 tests/scaffold/63-struct.c				\
 tests/scaffold/63-struct-cell.c			\
 tests/scaffold/63-struct-function.c			\
 tests/scaffold/63-struct-local.c			\
 tests/scaffold/63-struct-pointer.c			\
 tests/scaffold/64-make-cell.c				\
 tests/scaffold/65-read.c				\
 tests/scaffold/66-local-char-array.c			\
 tests/scaffold/70-stdarg.c				\
 tests/scaffold/71-struct-array.c			\
 tests/scaffold/72-typedef-struct-def.c			\
 tests/scaffold/73-union.c				\
 tests/scaffold/73-union-hello.c			\
 tests/scaffold/74-multi-line-string.c			\
 tests/scaffold/75-struct-union.c			\
 tests/scaffold/76-pointer-arithmetic.c			\
 tests/scaffold/76-pointer-arithmetic-pp.c		\
 tests/scaffold/77-pointer-assign.c			\
 tests/scaffold/78-union-struct.c			\
 tests/scaffold/79-int-array.c				\
 tests/scaffold/79-int-array-simple.c			\
 tests/scaffold/7a-struct-char-array.c			\
 tests/scaffold/7b-struct-int-array.c			\
 tests/scaffold/7b-struct-int-array-hello.c		\
 tests/scaffold/7b-struct-int-array-pointer.c		\
 tests/scaffold/7c-dynarray.c				\
 tests/scaffold/7d-cast-char.c				\
 tests/scaffold/7e-struct-array-access.c		\
 tests/scaffold/7f-struct-pointer-arithmetic.c		\
 tests/scaffold/7g-struct-byte-word-field.c		\
 tests/scaffold/7h-struct-assign.c			\
 tests/scaffold/7i-struct-struct.c			\
 tests/scaffold/7i-struct-struct-simple.c		\
 tests/scaffold/7k-empty-for.c				\
 tests/scaffold/7k-for-each-elem.c			\
 tests/scaffold/7k-for-each-elem-simple.c		\
 tests/scaffold/7l-struct-any-size-array.c		\
 tests/scaffold/7l-struct-any-size-array-simple.c	\
 tests/scaffold/7m-struct-char-array-assign.c		\
 tests/scaffold/7n-struct-struct-array.c		\
 tests/scaffold/7o-struct-pre-post.c			\
 tests/scaffold/7o-struct-pre-post-simple.c		\
 tests/scaffold/7p-struct-cast.c			\
 tests/scaffold/7q-bit-field.c				\
 tests/scaffold/7q-bit-field-simple.c			\
 tests/scaffold/7r-sign-extend.c			\
 tests/scaffold/7s-struct-short.c			\
 tests/scaffold/7s-unsigned-compare.c			\
 tests/scaffold/7t-function-destruct.c			\
 tests/scaffold/7u-call-ternary.c			\
 tests/scaffold/7u-double.c				\
 tests/scaffold/7u-inc-byte-word.c			\
 tests/scaffold/7u-long-long.c				\
 tests/scaffold/7u-struct-func.c			\
 tests/scaffold/7u-struct-size10.c			\
 tests/scaffold/7u-ternary-expression.c			\
 tests/scaffold/7u-vstack.c				\
 tests/scaffold/82-define.c				\
 tests/scaffold/83-heterogenoous-init.c			\
 tests/scaffold/84-struct-field-list.c			\
 tests/scaffold/85-sizeof.c				\
 tests/scaffold/90-goto-var.c				\
 tests/scaffold/91-goto-array.c				\
 tests/scaffold/a0-call-trunc-char.c			\
 tests/scaffold/a0-call-trunc-int.c			\
 tests/scaffold/a0-call-trunc-short.c			\
 tests/scaffold/a0-math-divide-signed-negative.c	\
 tests/scaffold/a1-global-no-align.c			\
 tests/scaffold/a1-global-no-clobber.c			\
 tests/assert/50-assert.c				\
 tests/dirent/90-readdir.c				\
 tests/io/90-stat.c					\
 tests/mes/30-oputs.c					\
 tests/mes/50-itoa.c					\
 tests/posix/50-getenv.c				\
 tests/posix/90-unsetenv.c				\
 tests/setjmp/80-setjmp.c				\
 tests/signal/90-signal.c				\
 tests/stdio/70-printf.c				\
 tests/stdio/70-printf-hello.c				\
 tests/stdio/70-printf-simple.c				\
 tests/stdio/80-sscanf.c				\
 tests/stdio/90-fopen-append.c				\
 tests/stdio/90-fopen.c					\
 tests/stdio/90-fread-fwrite.c				\
 tests/stdio/90-fseek.c					\
 tests/stdlib/50-malloc.c				\
 tests/stdlib/70-strtoull.c				\
 tests/stdlib/80-qsort.c				\
 tests/stdlib/80-qsort-dupes.c				\
 tests/stdlib/90-strtol.c				\
 tests/string/30-strlen.c				\
 tests/string/50-strcmp.c				\
 tests/string/50-strcpy.c				\
 tests/string/50-strncmp.c				\
 tests/string/70-strchr.c				\
 tests/string/80-strncpy.c				\
 tests/string/80-strrchr.c				\
 tests/string/90-snprintf.c				\
 tests/string/90-strpbrk.c				\
 tests/string/90-strspn.c

XFAIL_TESTS =

if have_mescc
XFAIL_TESTS +=					\
 tests/scaffold/17-compare-unsigned-char-le.c	\
 tests/scaffold/17-compare-unsigned-short-le.c	\
 tests/scaffold/66-local-char-array.c		\
 tests/scaffold/90-goto-var.c			\
 tests/scaffold/91-goto-array.c

if have_x86_64
XFAIL_TESTS +=					\
 tests/scaffold/t.c				\
 tests/scaffold/60-math.c			\
 tests/scaffold/a0-call-trunc-int.c		\
 tests/signal/90-signal.c
endif have_x86_64
endif have_mescc

if have_gcc
XFAIL_TESTS +=					\
 tests/mes/90-abtod.c

if have_x86_64
XFAIL_TESTS +=					\
 tests/posix/90-execlp.c			\
 tests/scaffold/70-extern.c			\
 tests/stdio/70-printf-hello.c			\
 tests/stdio/70-printf-simple.c			\
 tests/stdio/70-printf.c			\
 tests/stdio/80-sscanf.c			\
 tests/string/90-snprintf.c

endif have_x86_64
endif have_gcc

if have_x86_64
XFAIL_TESTS +=					\
 tests/stdio/70-printf-stdarg.c

endif have_x86_64

EXTRA_DIST +=					\
 tests/scaffold/02-return-1.exit		\
 tests/scaffold/05-call-1.exit			\
 tests/scaffold/07-include.exit
