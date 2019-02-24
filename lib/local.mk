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

lib_EXTRA =
noinst_lib_DATA =

EXTRA_DIST +=					\
 %D%/Makefile.am				\
 %D%/Makefile.in				\
 %D%/configure.ac				\
 %D%/configure

libc_mini_a_SOURCES =				\
 %D%/mes/eputs.c				\
 %D%/mes/oputs.c

mes_libc_mini_a_SOURCES =			\
 %D%/mes/write.c				\
 %D%/string/strlen.c				\
 %D%/stdlib/puts.c				\
 %D%/stdlib/exit.c

if !system_libc
libc_mini_a_SOURCES += $(mes_libc_mini_a_SOURCES)
else
EXTRA_DIST += $(mes_libc_mini_a_SOURCES)
endif

x86_linux_mescc_libc_mini_a_SOURCES = %D%/linux/x86-mes-mescc/mini.c
if x86_linux_mescc
libc_mini_a_SOURCES += $(x86_linux_mescc_libc_mini_a_SOURCES)
else
EXTRA_DIST += $(x86_linux_mescc_libc_mini_a_SOURCES)
endif

x86_linux_gcc_libc_mini_a_SOURCES = %D%/linux/x86-mes-gcc/mini.c
if x86_linux_gcc
libc_mini_a_SOURCES += $(x86_linux_gcc_libc_mini_a_SOURCES)
else
EXTRA_DIST += $(x86_linux_gcc_libc_mini_a_SOURCES)
endif

x86_64_linux_mescc_libc_mini_a_SOURCES = %D%/linux/x86_64-mes-mescc/mini.c
if x86_64_linux_mescc
libc_mini_a_SOURCES += $(x86_64_linux_mescc_libc_mini_a_SOURCES)
else
EXTRA_DIST += $(x86_64_linux_mescc_libc_mini_a_SOURCES)
endif

x86_64_linux_gcc_libc_mini_a_SOURCES = %D%/linux/x86_64-mes-gcc/mini.c
if x86_64_linux_gcc
libc_mini_a_SOURCES += $(x86_64_linux_gcc_libc_mini_a_SOURCES)
else
EXTRA_DIST += $(x86_64_linux_gcc_libc_mini_a_SOURCES)
endif

EXTRA_DIST +=					\
 $(libc_mini_a_SOURCES)

libmes_a_SOURCES =				\
 $(libc_mini_a_SOURCES)				\
 %D%/ctype/isnumber.c				\
 %D%/mes/abtol.c				\
 %D%/mes/div.c					\
 %D%/mes/eputc.c				\
 %D%/mes/fdgetc.c				\
 %D%/mes/fdputc.c				\
 %D%/mes/fdputs.c				\
 %D%/mes/fdungetc.c				\
 %D%/mes/itoa.c					\
 %D%/mes/ltoa.c					\
 %D%/mes/ltoab.c				\
 %D%/mes/mes_open.c				\
 %D%/mes/ntoab.c				\
 %D%/mes/oputc.c				\
 %D%/mes/ultoa.c				\
 %D%/mes/utoa.c

mes_libmes_a_SOURCES =				\
 %D%/ctype/isdigit.c				\
 %D%/ctype/isspace.c				\
 %D%/ctype/isxdigit.c				\
 %D%/posix/write.c				\
 %D%/stdlib/atoi.c

if !system_libc
libmes_a_SOURCES += $(mes_libmes_a_SOURCES)

if have_linux
libmes_a_SOURCES +=				\
 %D%/linux/lseek.c
endif have_linux
else system_libc
libmes_a_SOURCES +=				\
 %D%/mes/abtod.c				\
 %D%/mes/dtoab.c

EXTRA_DIST += $(mes_libmes_a_SOURCES)
endif system_libc

EXTRA_DIST +=					\
 $(libmes_a_SOURCES)

libc_a_SOURCES =				\
 $(libmes_a_SOURCES)				\
 %D%/mes/__assert_fail.c			\
 %D%/mes/__buffered_read.c			\
 %D%/mes/__mes_debug.c				\
 %D%/posix/execv.c				\
 %D%/posix/getcwd.c				\
 %D%/posix/getenv.c				\
 %D%/posix/isatty.c				\
 %D%/posix/open.c				\
 %D%/posix/read.c				\
 %D%/posix/setenv.c				\
 %D%/posix/wait.c				\
 %D%/stdio/fgetc.c				\
 %D%/stdio/fputc.c				\
 %D%/stdio/fputs.c				\
 %D%/stdio/getc.c				\
 %D%/stdio/getchar.c				\
 %D%/stdio/putc.c				\
 %D%/stdio/putchar.c				\
 %D%/stdio/ungetc.c				\
 %D%/stdlib/free.c				\
 %D%/stdlib/malloc.c				\
 %D%/stdlib/realloc.c				\
 %D%/string/memchr.c				\
 %D%/string/memcmp.c				\
 %D%/string/memcpy.c				\
 %D%/string/memset.c				\
 %D%/string/memmove.c				\
 %D%/string/strcmp.c				\
 %D%/string/strcpy.c				\
 %D%/string/strncmp.c

if have_linux
libc_a_SOURCES +=				\
 %D%/linux/access.c				\
 %D%/linux/brk.c				\
 %D%/linux/chmod.c				\
 %D%/linux/clock_gettime.c			\
 %D%/linux/dup.c				\
 %D%/linux/dup2.c				\
 %D%/linux/execve.c				\
 %D%/linux/fork.c				\
 %D%/linux/fsync.c				\
 %D%/linux/_getcwd.c				\
 %D%/linux/gettimeofday.c			\
 %D%/linux/ioctl.c				\
 %D%/linux/_open3.c				\
 %D%/linux/_read.c				\
 %D%/linux/time.c				\
 %D%/linux/unlink.c				\
 %D%/linux/waitpid.c
endif

x86_linux_mescc_libc_a_SOURCES = %D%/linux/x86-mes-mescc/syscall.c
if x86_linux_mescc
libc_a_SOURCES += $(x86_linux_mescc_libc_a_SOURCES)
else
EXTRA_DIST += $(x86_linux_mescc_libc_a_SOURCES)
endif

x86_linux_gcc_libc_a_SOURCES = %D%/linux/x86-mes-gcc/syscall.c
if x86_linux_gcc
libc_a_SOURCES += $(x86_linux_gcc_libc_a_SOURCES)
else
EXTRA_DIST += $(x86_linux_gcc_libc_a_SOURCES)
endif

x86_64_linux_mescc_libc_a_SOURCES = %D%/linux/x86_64-mes-mescc/syscall.c
if x86_64_linux_mescc
libc_a_SOURCES += $(x86_64_linux_mescc_libc_a_SOURCES)
else
EXTRA_DIST += $(x86_64_linux_mescc_libc_a_SOURCES)
endif

x86_64_linux_gcc_libc_a_SOURCES = %D%/linux/x86_64-mes-gcc/syscall.c
if x86_64_linux_gcc
libc_a_SOURCES += $(x86_64_linux_gcc_libc_a_SOURCES)
else
EXTRA_DIST += $(x86_64_linux_gcc_libc_a_SOURCES)
endif

EXTRA_DIST += \
 $(libc_a_SOURCES)

libc_tcc_a_SOURCES =				\
 $(libc_a_SOURCES)				\
 %D%/ctype/islower.c				\
 %D%/ctype/isupper.c				\
 %D%/ctype/tolower.c				\
 %D%/ctype/toupper.c				\
 %D%/mes/abtod.c				\
 %D%/mes/dtoab.c				\
 %D%/mes/search-path.c				\
 %D%/posix/execvp.c				\
 %D%/stdio/fclose.c				\
 %D%/stdio/fdopen.c				\
 %D%/stdio/ferror.c				\
 %D%/stdio/fflush.c				\
 %D%/stdio/fopen.c				\
 %D%/stdio/fprintf.c				\
 %D%/stdio/fread.c				\
 %D%/stdio/fseek.c				\
 %D%/stdio/ftell.c				\
 %D%/stdio/fwrite.c				\
 %D%/stdio/printf.c				\
 %D%/stdio/remove.c				\
 %D%/stdio/snprintf.c				\
 %D%/stdio/sprintf.c				\
 %D%/stdio/sscanf.c				\
 %D%/stdio/vfprintf.c				\
 %D%/stdio/vprintf.c				\
 %D%/stdio/vsnprintf.c				\
 %D%/stdio/vsprintf.c				\
 %D%/stdio/vsscanf.c				\
 %D%/stdlib/calloc.c				\
 %D%/stdlib/qsort.c				\
 %D%/stdlib/strtod.c				\
 %D%/stdlib/strtof.c				\
 %D%/stdlib/strtol.c				\
 %D%/stdlib/strtold.c				\
 %D%/stdlib/strtoll.c				\
 %D%/stdlib/strtoul.c				\
 %D%/stdlib/strtoull.c				\
 %D%/string/memmem.c				\
 %D%/string/strcat.c				\
 %D%/string/strchr.c				\
 %D%/string/strlwr.c				\
 %D%/string/strncpy.c				\
 %D%/string/strrchr.c				\
 %D%/string/strstr.c				\
 %D%/string/strupr.c				\
 %D%/stub/sigaction.c				\
 %D%/stub/ldexp.c				\
 %D%/stub/mprotect.c				\
 %D%/stub/localtime.c				\
 %D%/stub/sigemptyset.c


linux_libc_tcc_a_SOURCES =			\
 %D%/linux/close.c				\
 %D%/linux/rmdir.c				\
 %D%/linux/stat.c

if have_linux
libc_tcc_a_SOURCES += $(linux_libc_tcc_a_SOURCES)
else
EXTRA_DIST += $(linux_libc_tcc_a_SOURCES)
endif

x86_mescc_libc_tcc_a_SOURCES = %D%/x86-mes-mescc/setjmp.c
if x86_mescc
libc_tcc_a_SOURCES += $(x86_mescc_libc_tcc_a_SOURCES)
else
EXTRA_DIST += $(x86_mescc_libc_tcc_a_SOURCES)
endif

x86_gcc_libc_tcc_a_SOURCES = %D%/x86-mes-gcc/setjmp.c
if x86_gcc
libc_tcc_a_SOURCES += $(x86_gcc_libc_tcc_a_SOURCES)
else
EXTRA_DIST += $(x86_gcc_libc_tcc_a_SOURCES)
endif

x86_64_mescc_libc_tcc_a_SOURCES = %D%/x86_64-mes-mescc/setjmp.c
if x86_64_mescc
libc_tcc_a_SOURCES += $(x86_64_mescc_libc_tcc_a_SOURCES)
else
EXTRA_DIST += $(x86_64_mescc_libc_tcc_a_SOURCES)
endif

x86_64_gcc_libc_tcc_a_SOURCES = %D%/x86_64-mes-gcc/setjmp.c
if x86_64_gcc
libc_tcc_a_SOURCES += $(x86_64_gcc_libc_tcc_a_SOURCES)
else
EXTRA_DIST += $(x86_64_gcc_libc_tcc_a_SOURCES)
endif

EXTRA_DIST += \
 $(libc_tcc_a_SOURCES)

libtcc1_a_SOURCES = %D%/libtcc1.c
EXTRA_DIST += $(libtcc1_a_EXTRA)

libc_gnu_a_SOURCES =				\
 $(libc_tcc_a_SOURCES)				\
 %D%/ctype/isalnum.c				\
 %D%/ctype/isalpha.c				\
 %D%/ctype/isascii.c				\
 %D%/ctype/iscntrl.c				\
 %D%/ctype/isprint.c				\
 %D%/ctype/ispunct.c				\
 %D%/dirent/__getdirentries.c			\
 %D%/dirent/closedir.c				\
 %D%/dirent/opendir.c				\
 %D%/dirent/readdir.c				\
 %D%/math/fabs.c				\
 %D%/mes/fdgets.c				\
 %D%/posix/alarm.c				\
 %D%/posix/execl.c				\
 %D%/posix/mktemp.c				\
 %D%/posix/raise.c				\
 %D%/posix/sbrk.c				\
 %D%/posix/sleep.c				\
 %D%/posix/unsetenv.c				\
 %D%/stdio/clearerr.c				\
 %D%/stdio/feof.c				\
 %D%/stdio/fgets.c				\
 %D%/stdio/fileno.c				\
 %D%/stdio/freopen.c				\
 %D%/stdio/perror.c				\
 %D%/stdlib/__exit.c				\
 %D%/stdlib/abort.c				\
 %D%/stdlib/abs.c				\
 %D%/stdlib/alloca.c				\
 %D%/stdlib/atexit.c				\
 %D%/stdlib/atol.c				\
 %D%/stdlib/mbstowcs.c				\
 %D%/string/bcmp.c				\
 %D%/string/bcopy.c				\
 %D%/string/bzero.c				\
 %D%/string/index.c				\
 %D%/string/rindex.c				\
 %D%/string/strcspn.c				\
 %D%/string/strdup.c				\
 %D%/string/strerror.c				\
 %D%/string/strncat.c				\
 %D%/string/strpbrk.c				\
 %D%/string/strspn.c				\
 %D%/stub/__cleanup.c				\
 %D%/stub/bsearch.c				\
 %D%/stub/chown.c				\
 %D%/stub/ctime.c				\
 %D%/stub/fpurge.c				\
 %D%/stub/freadahead.c				\
 %D%/stub/frexp.c				\
 %D%/stub/getlogin.c				\
 %D%/stub/getpwnam.c				\
 %D%/stub/getpwuid.c				\
 %D%/stub/gmtime.c				\
 %D%/stub/pclose.c				\
 %D%/stub/popen.c				\
 %D%/stub/rand.c				\
 %D%/stub/setbuf.c				\
 %D%/stub/setlocale.c				\
 %D%/stub/setvbuf.c				\
 %D%/stub/sigaddset.c				\
 %D%/stub/sigblock.c				\
 %D%/stub/sigdelset.c				\
 %D%/stub/sigsetmask.c				\
 %D%/stub/strftime.c				\
 %D%/stub/sys_siglist.c				\
 %D%/stub/system.c				\
 %D%/stub/times.c				\
 %D%/stub/ttyname.c				\
 %D%/stub/umask.c				\
 %D%/stub/utime.c

linux_libc_gnu_a_SOURCES =			\
 %D%/linux/chdir.c				\
 %D%/linux/fcntl.c				\
 %D%/linux/fstat.c				\
 %D%/linux/getdents.c				\
 %D%/linux/getegid.c				\
 %D%/linux/geteuid.c				\
 %D%/linux/getgid.c				\
 %D%/linux/getpid.c				\
 %D%/linux/getppid.c				\
 %D%/linux/getrusage.c				\
 %D%/linux/getuid.c				\
 %D%/linux/kill.c				\
 %D%/linux/link.c				\
 %D%/linux/lstat.c				\
 %D%/linux/mkdir.c				\
 %D%/linux/nanosleep.c				\
 %D%/linux/pipe.c				\
 %D%/linux/rename.c				\
 %D%/linux/setgid.c				\
 %D%/linux/settimer.c				\
 %D%/linux/setuid.c				\
 %D%/linux/signal.c				\
 %D%/linux/sigprogmask.c

if have_linux
libc_gnu_a_SOURCES += $(linux_libc_gnu_a_SOURCES)
else
EXTRA_DIST += $(linux_libc_gnu_a_SOURCES)
endif

EXTRA_DIST +=					\
 $(libc_gnu_a_SOURCES)

lib_EXTRA +=					\
 %D%/x86-mes/elf32-0header.hex2			\
 %D%/x86-mes/elf32-body-exit-42.hex2		\
 %D%/x86-mes/elf32-footer-single-main.hex2	\
 %D%/x86-mes/elf32-header.hex2\
 %D%/x86-mes/x86.M1

# TODO: scaffold
EXTRA_DIST +=					\
 %D%/x86-mes-mescc/exit-42.c

lib_EXTRA +=					\
 %D%/x86_64-mes/elf64-0header.hex2		\
 %D%/x86_64-mes/elf64-body-exit-42.hex2		\
 %D%/x86_64-mes/elf64-footer-single-main.hex2	\
 %D%/x86_64-mes/elf64-header.hex2		\
 %D%/x86_64-mes/x86_64.M1

# TODO: scaffold
EXTRA_DIST +=					\
 %D%/x86_64-mes-mescc/exit-42.c

EXTRA_DIST += $(lib_EXTRA)
