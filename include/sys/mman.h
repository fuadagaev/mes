/* -*-comment-start: "//";comment-end:""-*-
 * GNU Mes --- Maxwell Equations of Software
 * Copyright © 2017,2019 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
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
#ifndef __MES_SYS_MMAN_H
#define __MES_SYS_MMAN_H 1

#if WITH_GLIBC
#undef __MES_SYS_MMAN_H
#include_next <sys/mman.h>
#else // ! WITH_GLIBC

#include <sys/types.h>

#define MAP_SHARED    0x01
#define MAP_PRIVATE   0x02
#define MAP_ANONYMOUS 0x20
#define MAP_POPULATE  0x08000

#define MAP_ANON MAP_ANONYMOUS
#define MAP_FAILED ((void*)-1)

#define PROT_NONE 0
#define PROT_READ 1
#define PROT_WRITE 2
#define PROT_EXEC 4

void *mmap (void *address, size_t length, int protect, int flags, int filedes,
            off_t offset);
int mprotect (void *addr, size_t len, int prot);
int munmap (void *addr, size_t length);

#endif // ! WITH_GLIBC

#endif // __MES_SYS_MMAN_H
