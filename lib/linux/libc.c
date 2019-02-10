/* -*-comment-start: "//";comment-end:""-*-
 * GNU Mes --- Maxwell Equations of Software
 * Copyright © 2016,2017,2018,2019 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
 *
 * This file is part of GNU Mes.
 *
 * GNU Mes is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * GNU Mes is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Mes.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <mes/lib.h>

#include <fcntl.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

#if __MESC__ && __i386__
#include <linux/x86-mes/mes.c>
#elif __MESC__ && __x86_64__
#include <linux/x86_64-mes/mes.c>
#elif __i386__
#include <linux/x86-mes-gcc/mes.c>
#elif __x86_64__
#include <linux/x86_64-mes-gcc/mes.c>
#else
#error arch not supported
#endif

#include <linux/access.c>
#include <linux/brk.c>
#include <linux/chmod.c>
#include <linux/clock_gettime.c>
#include <linux/dup.c>
#include <linux/dup2.c>
#include <linux/execve.c>
#include <linux/fork.c>
#include <linux/fsync.c>
#include <linux/getcwd.c>
#include <linux/gettimeofday.c>
#include <linux/ioctl.c>
#include <linux/open.c>
#include <linux/read.c>
#include <linux/time.c>
#include <linux/unlink.c>
#include <linux/waitpid.c>
