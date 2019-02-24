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

include_HEADERS =				\
 %D%/alloca.h					\
 %D%/argz.h					\
 %D%/ar.h					\
 %D%/assert.h					\
 %D%/ctype.h					\
 %D%/dirent.h					\
 %D%/dirstream.h				\
 %D%/dlfcn.h					\
 %D%/endian.h					\
 %D%/errno.h					\
 %D%/fcntl.h					\
 %D%/features.h					\
 %D%/float.h					\
 %D%/getopt.h					\
 %D%/grp.h					\
 %D%/inttypes.h					\
 %D%/libgen.h					\
 %D%/limits.h					\
 %D%/locale.h					\
 %D%/math.h					\
 %D%/memory.h					\
 %D%/pwd.h					\
 %D%/setjmp.h					\
 %D%/signal.h					\
 %D%/stdarg.h					\
 %D%/stdbool.h					\
 %D%/stddef.h					\
 %D%/stdint.h					\
 %D%/stdio.h					\
 %D%/stdlib.h					\
 %D%/stdnoreturn.h				\
 %D%/string.h					\
 %D%/strings.h					\
 %D%/termio.h					\
 %D%/time.h					\
 %D%/unistd.h

include_linux_HEADERS =				\
 %D%/linux/syscall.h

include_linuxdir = $(includedir)/linux

include_linux_x86_HEADERS =			\
 %D%/linux/x86/syscall.h

include_linux_x86dir = $(includedir)/linux/x86

include_linux_x86_64_HEADERS =			\
 %D%/linux/x86_64/syscall.h

include_linux_x86_64dir = $(includedir)/linux/x86_64

include_mes_HEADERS =				\
 %D%/mes/builtins.h				\
 %D%/mes/lib.h					\
 %D%/mes/lib-mini.h

include_mesdir = $(includedir)/mes

include_sys_HEADERS =				\
 %D%/sys/cdefs.h				\
 %D%/sys/dir.h					\
 %D%/sys/file.h					\
 %D%/sys/ioctl.h				\
 %D%/sys/mman.h					\
 %D%/sys/param.h				\
 %D%/sys/resource.h				\
 %D%/sys/select.h				\
 %D%/sys/stat.h					\
 %D%/sys/timeb.h				\
 %D%/sys/time.h					\
 %D%/sys/times.h				\
 %D%/sys/types.h				\
 %D%/sys/ucontext.h				\
 %D%/sys/user.h					\
 %D%/sys/wait.h

include_sysdir = $(includedir)/sys
