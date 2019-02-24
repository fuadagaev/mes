dnl GNU Mes --- Maxwell Equations of Software
dnl Copyright © 2012, 2013, 2014, 2015, 2016, 2018 Ludovic Courtès <ludo@gnu.org>
dnl Copyright © 2014 Mark H Weaver <mhw@netris.org>
dnl Copyright © 2017 Efraim Flashner <efraim@flashner.co.il>
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

dnl MES_SYSTEM_TYPE
dnl
dnl Determine the Mes host system type, and store it in the
dnl `mes_system' variable.
AC_DEFUN([MES_SYSTEM_TYPE], [
  AC_REQUIRE([AC_CANONICAL_HOST])
  AC_MSG_CHECKING([for the compiler type])
  if test "x$GCC" = "xyes"; then
    compiler=gcc;
  elif test "x$CC" = "xmescc"; then
    compiler=mescc;
  else
    case "$CC" in
      gcc|*-gcc)
         compiler=gcc;;
      mescc|*-mescc|*mescc*)
        compiler=mescc;;
      *)
        compiler=unknown;;
    esac
  fi
  AC_MSG_RESULT([$compiler])
  if test "$compiler" = unknown; then
    AC_MSG_WARN([Using an unknown compiler, assuming gcc compatibility])
    compiler=gcc
  fi
  AC_SUBST(compiler, [$compiler])

  AC_LANG([C])
  AC_MSG_CHECKING([whether the _AC_LANG compiler works])
  AC_LANG_CONFTEST(
     [AC_LANG_SOURCE([[int main () {return 0;}]])])
  AS_IF([_AC_DO_VAR(ac_compile)], [can_compile=yes], [can_compile=no])
  AC_MSG_RESULT([$can_compile])
  if test "$can_compile" = no; then
    _AC_MSG_LOG_CONFTEST
    AC_MSG_FAILURE([compiling a trivial C program])
  fi

  # FIXME: let's not default to using the system library yet,
  # as that does not work atm.
  # AC_ARG_WITH(system-libc,
  #   AC_HELP_STRING([--with-system-libc],
  #     [compile Mes with system libc?]),
  #     [case "$withval" in
  #       yes|system)
  #         mes_libc=system;;
  #       *)
  #         mes_libc=mes;;
  #      esac],
  #     [AC_MSG_CHECKING([which C library to use])
  #      AS_IF([_AC_DO_VAR(ac_link)], [mes_libc=system], [mes_libc=mes])
  #      AC_MSG_RESULT([$mes_libc])
  #      if test "x$mes_libc" = "xmes"; then
  #        _AC_MSG_LOG_CONFTEST
  #      fi])

  AC_MSG_CHECKING([whether the system C library works])
  AS_IF([_AC_DO_VAR(ac_link)], [system_libc=yes], [system_libc=no])
  AC_MSG_RESULT([$system_libc])
  if test "x$system_libc" = "xno"; then
    _AC_MSG_LOG_CONFTEST
  fi

  mes_libc=mes
  AC_MSG_CHECKING([which C library to use])
  AC_ARG_WITH(system-libc,
    AC_HELP_STRING([--with-system-libc],
      [compile Mes with system libc?]),
      [case "$withval" in
        yes|system)
          mes_libc=system;;
        *)
          mes_libc=mes;;
       esac],
       [mes_libc=mes])
  AC_MSG_RESULT([$mes_libc])

  AM_CONDITIONAL([system_libc], [test "x$mes_libc" = "xsystem"])

  _AC_COMPILER_OBJEXT

  AC_MSG_CHECKING([for the Mes system type])
  machine=`$CC -dumpmachine`
  case "$machine" in
    i*86*-*|x86-*)
       mes_bits="32";
       mes_cpu="x86";;
    amd64*-*|x86_64-*)
       mes_bits="64";
       mes_cpu="x86_64";;
    arm-*|armv[[7-9]]*-*)
       mes_bits="32";
       mes_cpu="arm";;
    *)
       mes_bits="32";
       mes_cpu="$host_cpu";;
  esac

  case "$machine" in
    *linux-gnu*)
       mes_kernel=linux;;
    *gnu*)
       mes_kernel=hurd;;
    *)
       mes_kernel=${host_os%%-*};;
  esac
  case "$compiler" in
    mescc)
      mes_system="$mes_cpu-mes";;
    *)
      mes_system="$mes_cpu-mes-$compiler";;
  esac
  AC_MSG_RESULT([$mes_system])

  AC_SUBST([mes_bits])
  AC_SUBST([mes_kernel])
  AC_SUBST([mes_cpu])
  AC_SUBST([mes_libc])
  AC_SUBST([mes_system])

  AM_CONDITIONAL([have_x86], [test "$mes_cpu" = "x86"])
  AM_CONDITIONAL([have_x86_64], [test "$mes_cpu" = "x86_64"])
  AM_CONDITIONAL([have_linux], [test "x${mes_kernel}" = "xlinux"])
  AM_CONDITIONAL([have_gcc], [test "$compiler" = "gcc"])
  AM_CONDITIONAL([have_mescc], [test "$compiler" = "mescc"])

  AM_CONDITIONAL([x86_mescc], [test "x${mes_cpu}_${compiler}" = "xx86_mescc"])
  AM_CONDITIONAL([x86_gcc], [test "x${mes_cpu}_${compiler}" = "xx86_gcc"])
  AM_CONDITIONAL([x86_64_mescc], [test "x${mes_cpu}_${compiler}" = "xx86_64_mescc"])
  AM_CONDITIONAL([x86_64_gcc], [test "x${mes_cpu}_${compiler}" = "xx86_64_gcc"])

  AM_CONDITIONAL([x86_linux_mescc], [test "x${mes_cpu}_${mes_kernel}_${compiler}" = "xx86_linux_mescc"])
  AM_CONDITIONAL([x86_linux_gcc], [test "x${mes_cpu}_${mes_kernel}_${compiler}" = "xx86_linux_gcc"])
  AM_CONDITIONAL([x86_64_linux_mescc], [test "x${mes_cpu}_${mes_kernel}_${compiler}" = "xx86_64_linux_mescc"])
  AM_CONDITIONAL([x86_64_linux_gcc], [test "x${mes_cpu}_${mes_kernel}_${compiler}" = "xx86_64_linux_gcc"])
])

dnl MES_ASSERT_SUPPORTED_SYSTEM
dnl
dnl Assert that this is a system to which the distro is ported.
AC_DEFUN([MES_ASSERT_SUPPORTED_SYSTEM], [
  AC_REQUIRE([MES_SYSTEM_TYPE])

  AC_ARG_WITH([courage], [AC_HELP_STRING([--with-courage],
    [Assert that even if this platform is unsupported, you will be
courageous and port GNU Mes to it.)])],
    [mes_courageous="$withval"],
    [mes_courageous="no"])
  AC_SUBST([mes_courageous])

  # Currently only Linux-based systems are supported, and only on some
  # platforms.
  mes_supported=no
  case "$mes_system" in
    x86-mes-gcc|x86_64-mes-gcc)
      if test "x$mes_libc" = "xmes"; then
        mes_supported=yes
      fi
      ;;
    x86-mes|x86_64-mes)
      mes_supported=yes;;
    *)
      ;;
  esac
  if test "x$mes_supported" = "xno"; then
    if test "x$mes_courageous" = "xyes"; then
      AC_MSG_WARN([building Mes on `$mes_system' with $mes_libc libc, which is not supported])
    else
      AC_MSG_ERROR([`$mes_system' with $mes_libc libc is not supported.
See "Porting GNU Mes" in the manual, or try `--with-courage'.])
     fi
   fi
])
