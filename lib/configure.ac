dnl                                              -*- Autoconf -*-
dnl Process this file with autoconf to produce a configure script.

dnl GNU Mes --- Maxwell Equations of Software
dnl Copyright © 2019 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
dnl
dnl This file is part of GNU Mes.
dnl
dnl GNU Mes is free software; you can redistribute it and/or modify it
dnl under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; either version 3 of the License, or (at
dnl your option) any later version.
dnl
dnl GNU Mes is distributed in the hope that it will be useful, but
dnl WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with GNU Mes.  If not, see <http://www.gnu.org/licenses/>.

AC_PREREQ([2.69])
AC_INIT([GNU Mes CC],
  [m4_esyscmd([../build-aux/git-version-gen .tarball-version])],
  [bug-mes@gnu.org], [mes],
  [https://gnu.org/s/mes/])
AC_CONFIG_SRCDIR([mes/eputc.c])
AC_CONFIG_AUX_DIR([../build-aux])
AC_CONFIG_MACRO_DIR([../m4])

dnl We use foreign to prevent the useless boilerplate INSTALL
AM_INIT_AUTOMAKE([foreign color-tests silent-rules subdir-objects])
AM_SILENT_RULES([yes])

AC_PATH_PROG([BASH], [bash], [sh])

if test "x$srcdir" != "x."; then
   rm -f include
   ln -s $srcdir/../include include
   rm -f lib
   ln -s $srcdir lib
fi

MES_SYSTEM_TYPE
MES_ASSERT_SUPPORTED_SYSTEM

# Checks for programs.
# FIXME: AC_PATH_PROG depends on fopen, fwrite, ferror and we will have
# them after building the Mes C libary that we are configuring.
#AC_PROG_CC([$CC mescc gcc cc])

# FIXME: The Automake manual strongly advises not to use _AM_DEPENDENCIES
# but does not point to alternatives.  It seems that this problem
#    @am__include@ @am__quote@./$(DEPDIR)/libc-mini.Po@am__quote@ # am--include-marker
# is fixed by ignoring the advice.
_AM_DEPENDENCIES(CC)

# FIXME: check for AR is missing?
#AC_PROG_AR
AC_CHECK_PROGS(AR, ar)
AC_PROG_RANLIB
AC_PROG_INSTALL

GUILE_PKG([3.0 2.2 2.0])
GUILE_PROGS
GUILE_SITE_DIR

AC_ARG_WITH(system-libc,
  AC_HELP_STRING([--with-system-libc],
    [Compile Mes with system libc?]),
  [system_libc=yes;library=system],
  [system_libc=no;library=mes])
AM_CONDITIONAL([SYSTEM_LIBC], [test "$system_libc" = "yes"])

AC_CONFIG_HEADERS([config.h])
AC_SUBST(CC, [$CC])
AC_SUBST(OBJEXT, [o])
AM_CONDITIONAL(AMDEP, [test "$compiler" = "gcc"])
AM_CONDITIONAL(am__fastdepCC, [test "$compiler" = "gcc"])
AC_SUBST(compiler, [$compiler])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([config.sh:../build-aux/config.sh.in])
AC_CACHE_SAVE
AC_OUTPUT

mkdir -p assert ctype dirent math mes posix stdio stdlib string stub mes

dnl cat <<EOF

cat <<EOF

GNU Mes C Library is configured for
   compiler: $compiler
   system:   $mes_system
   cpu:      $mes_cpu
   bits:     $mes_bits
   libc:     $mes_libc

Run:
  make            to build GNU Mes C Library
  make help       for help on other targets
EOF
