## Copyright (C) 2019 Jeremiah Orians
## This file is part of Gnu Mes
##
## Gnu Mes is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## Gnu Mes is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with Gnu Mes.  If not, see <http://www.gnu.org/licenses/>.

# Prevent rebuilding
VPATH = bin:test:test/results

CC = gcc
M2_PLANET = M2-Planet

CFLAGS:=					\
  $(CFLAGS)					\
 -D _GNU_SOURCE					\
 -D const=					\
 -ggdb						\
 -D WITH_GLIBC=1				\
 -D 'MES_VERSION="git"'				\
 -D 'MES_PKGDATADIR="/usr/local/share/mes"'	\
 -I include					\
 -fno-builtin					\
 -Wno-discarded-qualifiers			\
 -Wno-discarded-array-qualifiers		\
 -Wno-ignored-qualifiers			\
 -Wno-incompatible-pointer-types		\
 -Wno-int-conversion

MES_SOURCES =					\
 src/builtins.c					\
 src/eval.c					\
 src/gc.c					\
 src/hash.c					\
 src/init.c					\
 src/lib.c					\
 src/math.c					\
 src/mes.c						\
 src/module.c					\
 src/posix.c					\
 src/printer.c					\
 src/reader.c					\
 src/string.c					\
 src/struct.c					\
 src/vector.c

LIB_SOURCES =					\
 lib/mes/eputs.c				\
 lib/mes/itoa.c

M2_SOURCES =					\
 lib/m2/mes_open.c				\
 lib/m2/in_set.c				\
 lib/m2/numerate.c				\

M2_TODO =					\
 lib/m2/file_print.c				\
 lib/m2/ntoab.c					\
 lib/mes/fdgetc.c				\
 lib/mes/fdungetc.c

SOURCES = $(M2_SOURCES) $(LIB_SOURCES) $(MES_SOURCES)
INCLUDES =					\
 include/mes/constants.h			\
 include/mes/gcc.h				\
 include/mes/mes.h

GCC_SOURCES =					\
 lib/m2/file_print.c				\
 lib/mes/fdgetc.c				\
 lib/mes/fdungetc.c				\
 lib/mes/fdputc.c				\
 lib/mes/fdputs.c				\
 lib/mes/ntoab.c				\
 $(SOURCES)

mes-gcc: makefile $(GCC_SOURCES) $(INCLUDES) | bin
	$(CC) $(CFLAGS) $(GCC_SOURCES) -o src/mes

M2_PLANET_INCLUDES =				\
 include/mes/mes.h				\
 include/mes/m2.h				\
 include/mes/constants.h

M2_PLANET_PREFIX = ../M2-Planet
M2_PLANET_SOURCES =						\
 $(M2_PLANET_PREFIX)/test/common_amd64/functions/exit.c		\
 $(M2_PLANET_PREFIX)/test/common_amd64/functions/malloc.c	\
 $(M2_PLANET_PREFIX)/functions/calloc.c				\
 $(M2_PLANET_INCLUDES)						\
 $(SOURCES:%.c=%.m)

$(info M2_PLANET_SOURCES: $(M2_PLANET_SOURCES))

%.m: %.c makefile
	@sed -r 's@^(#include.*)@/* \1 */@' $< > $*.im
	@$(CC) -E -I include			\
	    -D FUNCTION0=FUNCTION		\
	    -D FUNCTION1=FUNCTION		\
	    -D FUNCTION2=FUNCTION		\
	    -D FUNCTION3=FUNCTION		\
	    -D FUNCTIONN=FUNCTION		\
	    -D const=				\
	    -o $@ -x c $*.im

mes-m2: makefile $(M2_PLANET_SOURCES) $(M2_PLANET_INCLUDES) | bin
	$(M2_PLANET) $(M2_PLANET_FLAGS) $(M2_PLANET_SOURCES:%=-f %) -o bin/mes-m2 || rm -f bin/mes-m2

# Clean up after ourselves
.PHONY: clean
clean:
	rm -rf bin/ test/results/
#	./test/test000/cleanup.sh

# Directories
bin:
	mkdir -p bin

results:
	mkdir -p test/results

# tests
test: test000.answer \
	test001.answer \
	test100.answer \
	test101.answer \
	test102.answer \
	test103.answer \
	test105.answer \
	test106.answer \
	test109.answer \
	test133.answer \
	test200-binary | results
	sha256sum -c test/test.answers
#	test104.answer
#	test107.answer
#	test108.answer
#	test110.answer
#	test111.answer
#	test112.answer
#	test113.answer
#	test114.answer
#	test115.answer
#	test116.answer
#	test117.answer
#	test118.answer
#	test119.answer
#	test120.answer
#	test121.answer
#	test122.answer
#	test123.answer
#	test124.answer
#	test125.answer
#	test126.answer
#	test127.answer
#	test128.answer
#	test129.answer
#	test130.answer
#	test131.answer
#	test132.answer

test000.answer: results mes-m2
	test/test000/hello.sh

test001.answer: results mes-m2
	test/test001/hello.sh

test100.answer: results mes-m2
	test/test100/hello.sh

test101.answer: results mes-m2
	test/test101/hello.sh

test102.answer: results mes-m2
	test/test102/hello.sh

test103.answer: results mes-m2
	test/test103/hello.sh

test104.answer: results mes-m2
	test/test104/hello.sh

test105.answer: results mes-m2
	test/test105/hello.sh

test106.answer: results mes-m2
	test/test106/hello.sh

test107.answer: results mes-m2
	test/test107/hello.sh

test108.answer: results mes-m2
	test/test108/hello.sh

test109.answer: results mes-m2
	test/test109/hello.sh

test110.answer: results mes-m2
	test/test110/hello.sh

test111.answer: results mes-m2
	test/test111/hello.sh

test112.answer: results mes-m2
	test/test112/hello.sh

test113.answer: results mes-m2
	test/test113/hello.sh

test114.answer: results mes-m2
	test/test114/hello.sh

test115.answer: results mes-m2
	test/test115/hello.sh

test116.answer: results mes-m2
	test/test116/hello.sh

test117.answer: results mes-m2
	test/test117/hello.sh

test118.answer: results mes-m2
	test/test118/hello.sh

test119.answer: results mes-m2
	test/test119/hello.sh

test120.answer: results mes-m2
	test/test120/hello.sh

test121.answer: results mes-m2
	test/test121/hello.sh

test122.answer: results mes-m2
	test/test122/hello.sh

test123.answer: results mes-m2
	test/test123/hello.sh

test124.answer: results mes-m2
	test/test124/hello.sh

test125.answer: results mes-m2
	test/test125/hello.sh

test126.answer: results mes-m2
	test/test126/hello.sh

test127.answer: results mes-m2
	test/test127/hello.sh

test128.answer: results mes-m2
	test/test128/hello.sh

test129.answer: results mes-m2
	test/test129/hello.sh

test130.answer: results mes-m2
	test/test130/hello.sh

test131.answer: results mes-m2
	test/test131/hello.sh

test132.answer: results mes-m2
	test/test132/hello.sh

test133.answer: results mes-m2
	test/test133/hello.sh

test200-binary: results mes-m2
	test/test200/hello.sh

# Generate test answers
.PHONY: Generate-test-answers
Generate-test-answers:
	sha256sum test/results/* >| test/test.answers

DESTDIR:=
PREFIX:=/usr/local
bindir:=$(DESTDIR)$(PREFIX)/bin
.PHONY: install
install: mes-m2
	mkdir -p $(bindir)
	cp $^ $(bindir)

TAGS:
	etags $(shell find . -name '*.c' -o -name '*.h') --language=scheme $(shell find mes module -name '*.mes' -o -name '*.scm')
