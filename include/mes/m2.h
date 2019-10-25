/* -*-comment-start: "//";comment-end:""-*-
 * GNU Mes --- Maxwell Equations of Software
 * Copyright © 2019 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
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

#ifndef __MES_M2_H
#define __MES_M2_H

#if __M2_PLANET__

char **environ;
int __stdin;
int __stdout;
int __stderr;

int __ungetc_p (int filedes);
int eputs (char *s);
int oputs (char *s);
int puts (char *s);
size_t strlen (char *s);
ssize_t _write ();
ssize_t write (int filedes, void *buffer, size_t size);
void __ungetc_clear (int filedes);
void __ungetc_init ();
void __ungetc_set (int filedes, int c);

/* Recieved SCM in program
struct timespec
{
  long tv_sec;
  long tv_nsec;
};

struct timeval
{
  long tv_sec;
  long tv_usec;
};

*/

#else

#include "mes/mes.h"

#endif

#define struct_size 12
#define CELL(x) ((x*struct_size)+g_cells)

SCM TYPE (SCM x);
SCM *TYPE_PTR (SCM x);

SCM CAR (SCM x);
SCM *CAR_PTR (SCM x);

SCM CDR (SCM x);
SCM *CDR_PTR (SCM x);

SCM NTYPE (SCM x);
SCM *NTYPE_PTR (SCM x);

SCM NCAR (SCM x);
SCM *NCAR_PTR (SCM x);

SCM NCDR (SCM x);
SCM *NCDR_PTR (SCM x);

SCM BYTES (SCM x);
SCM *BYTES_PTR (SCM x);

SCM LENGTH (SCM x);
SCM *LENGTH_PTR (SCM x);

SCM MACRO (SCM x);
SCM *MACRO_PTR (SCM x);

SCM PORT (SCM x);
SCM *PORT_PTR (SCM x);

SCM REF (SCM x);
SCM *REF_PTR (SCM x);

SCM VARIABLE (SCM x);
SCM *VARIABLE_PTR (SCM x);

SCM CLOSURE (SCM x);
SCM *CLOSURE_PTR (SCM x);

SCM CONTINUATION (SCM x);
SCM *CONTINUATION_PTR (SCM x);

SCM NAME (SCM x);
SCM *NAME_PTR (SCM x);

SCM STRING (SCM x);
SCM *STRING_PTR (SCM x);

SCM STRUCT (SCM x);
SCM *STRUCT_PTR (SCM x);

SCM VALUE (SCM x);
SCM *VALUE_PTR (SCM x);

SCM VECTOR (SCM x);
SCM *VECTOR_PTR (SCM x);

SCM NLENGTH (SCM x);
SCM *NLENGTH_PTR (SCM x);

SCM NVALUE (SCM x);
SCM *NVALUE_PTR (SCM x);

SCM NSTRING (SCM x);
SCM *NSTRING_PTR (SCM x);

SCM NVECTOR (SCM x);
SCM *NVECTOR_PTR (SCM x);

SCM CAAR (SCM x);
SCM CADR (SCM x);
SCM CDAR (SCM x);
SCM CDDR (SCM x);
SCM CADAR (SCM x);
SCM CADDR (SCM x);
SCM CDADR (SCM x);
SCM CDDAR (SCM x);

#endif /* __MES_M2_H */
