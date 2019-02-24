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
AC_INIT([GNU Mes],
  [m4_esyscmd([build-aux/git-version-gen .tarball-version])],
  [bug-mes@gnu.org], [mes],
  [https://gnu.org/s/mes/])
AC_CONFIG_SRCDIR([src/mes.c])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

# We cannot DIST_SUBDIRS = lib to have make install recurse into lib:
# make install
# make[1]: Entering directory '/home/janneke/src/mes/wip-auto/mescc-lib'
# /bin/sh ./config.status --recheck
# configure: error: source directory already configured; run "make distclean" there first
# AC_CONFIG_SUBDIRS(lib)

dnl We use foreign to prevent the useless boilerplate INSTALL
AM_INIT_AUTOMAKE([foreign color-tests silent-rules subdir-objects])
AM_SILENT_RULES([yes])
disable_silent_rules=no
if test "x$enable_silent_rules" = "xno"; then
  disable_silent_rules=yes
fi
AC_SUBST([disable_silent_rules])

AC_PATH_PROG([BASH], [bash], [sh],
  [AC_MSG_ERROR([Bash could not be found; please install it.])])
AC_PATH_PROG([M1], [M1], [M1], [no])
if test "$M1" = "no"; then
   AC_MSG_ERROR([M1 could not be found; please install mescc-tools.])
fi
AC_PATH_PROG([HEX2], [hex2], [hex2], [no])
if test "$HEX2" = "no"; then
   AC_MSG_ERROR([hex2 could not be found; please install mescc-tools.])
fi
AC_PATH_PROG([BLOOD_ELF], [blood-elf], [blood-elf], [no])
if test "$BLOOD_ELF" = "no"; then
   AC_MSG_ERROR([blood-elf could not be found; please install mescc-tools.])
fi
AC_PATH_PROG([M2_PLANET], [M2-Planet], [M2-Planet], [no])

# Checks for programs.
#AC_PROG_CC
AC_CHECK_TOOLS([CC], gcc cc mescc, [no])
if test "$CC" = "no"; then
  AC_MSG_ERROR([No C compiler found; please install one.])
fi
_AM_DEPENDENCIES(CC)
AC_SUBST([CC])

MES_SYSTEM_TYPE
MES_ASSERT_SUPPORTED_SYSTEM

AC_PROG_RANLIB

pkgdatadir="${datarootdir}/mes"
AC_DEFINE_UNQUOTED([pkgdatadir], ["${pkgdatadir}"], [Where Mes looks for its modules])

AM_MISSING_PROG([DOT], [dot])
AM_MISSING_PROG([MAKEINFO], [makeinfo])
AM_MISSING_PROG([HELP2MAN], [help2man])

GUILE_PKG([3.0 2.2 2.0])
GUILE_PROGS
GUILE_SITE_DIR
AM_CONDITIONAL([HAVE_GUILE], [$GUILE --help >/dev/null 2>&1 ])

GUILE_MODULE_AVAILABLE([have_nyacc], [(nyacc lalr)])
if test "x$have_nyacc" != "xyes"; then
  AC_MSG_ERROR([Guile Nyacc could not be found; please install it.])
fi

AC_MSG_CHECKING([Nyacc version])
nyacc_version=$($GUILE -c '(use-modules (nyacc lalr)) (display *nyacc-version*)')
AC_MSG_RESULT([$nyacc_version])
if test "$nyacc_version" != "0.93.0"\
   && test "$nyacc_version" != "0.94.0"\
   && test "$nyacc_version" != "0.99.0"; then
  AC_MSG_ERROR([Nyacc version $nyacc_version found; please install 0.99.0])
fi

AC_CONFIG_HEADERS([include/mes/config.h])
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([pre-inst-env:build-aux/pre-inst-env.in],
  [chmod +x pre-inst-env])
AC_CONFIG_FILES([scripts/mescc:scripts/mescc.in],
  [chmod +x scripts/mescc])
AC_CONFIG_FILES([scripts/mescc.scm:scripts/mescc.scm.in],
  [chmod +x scripts/mescc.scm])
AC_CONFIG_FILES([scripts/mesar:scripts/mesar.in],
  [chmod +x scripts/mesar])


if test "x$srcdir" != "x."; then
    srcdest="$srcdir"/
fi
AC_SUBST(srcdest, [$srcdest])

AC_CACHE_SAVE
AC_OUTPUT

cat <<EOF

GNU Mes is configured for
   compiler: $compiler
   system:   $mes_system
   cpu:      $mes_cpu
   bits:     $mes_bits
   libc:     $mes_libc

Run:
  make            to build GNU Mes
  make help       for help on other targets
EOF
