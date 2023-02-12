/* -*-comment-start: "//";comment-end:""-*-
 * GNU Mes --- Maxwell Equations of Software
 * Copyright © 2021 W. J. van der Laan <laanwj@protonmail.com>
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
#ifndef __MES_LINUX_RISCV64_KERNEL_STAT_H
#define __MES_LINUX_RISCV64_KERNEL_STAT_H 1

// *INDENT-OFF*
struct stat
{
  unsigned long  st_dev;
  unsigned long  st_ino;
  unsigned int   st_mode;
  unsigned int   st_nlink;
  unsigned int   st_uid;
  unsigned int   st_gid;
  unsigned long  st_rdev;
  long           st_size;
  unsigned long  st_blksize;
  unsigned long  st_blocks;
  time_t         st_atime;
  unsigned long  st_atime_usec;
  time_t         st_mtime;
  unsigned long  st_mtime_usec;
  time_t         st_ctime;
  unsigned long  st_ctime_usec;
  unsigned long  __foo0;
  unsigned long  __foo1;
};

#endif // __MES_LINUX_RISCV64_KERNEL_STAT_H
