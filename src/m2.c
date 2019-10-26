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

//#include "mes/mes.h"
#include "mes/m2.h"

#if __M2_PLANET__
#define M2_CELL_SIZE 12
// CONSTANT M2_CELL_SIZE 12
#else
#define M2_CELL_SIZE 1
// CONSTANT M2_CELL_SIZE 12
#endif

#if M2_FUNCTIONS

SCM
TYPE (SCM x)
{
  struct scm *s = &g_cells[x];
  return s->type;
}

SCM *
TYPE_PTR (SCM x)
{
  char *p = g_cells + (x * M2_CELL_SIZE);
  return p;
}

SCM
CAR (SCM x)
{
  struct scm *s = &g_cells[x];
  return s->car;
}

SCM *
CAR_PTR (SCM x)
{
  char *p = g_cells + (x * M2_CELL_SIZE);
  return p + sizeof (long);
}

SCM
CDR (SCM x)
{
  struct scm *s = &g_cells[x];
  return s->cdr;
}

SCM *
CDR_PTR (SCM x)
{
  char *p = g_cells + (x * M2_CELL_SIZE);
  return p + sizeof (long) + sizeof (long);
}

SCM
NTYPE (SCM x)
{
  struct scm *s = &g_news[x];
  return s->type;
}

SCM *
NTYPE_PTR (SCM x)
{
  char *p = g_news + (x * M2_CELL_SIZE);
  return p;
}

SCM
NCAR (SCM x)
{
  struct scm *s = &g_news[x];
  return s->car;
}

SCM *
NCAR_PTR (SCM x)
{
  char *p = g_news + (x * M2_CELL_SIZE);
  return p + sizeof (long);
}

SCM
NCDR (SCM x)
{
  struct scm *s = &g_news[x];
  return s->cdr;
}

SCM *
NCDR_PTR (SCM x)
{
  char *p = g_news + (x * M2_CELL_SIZE);
  return p + sizeof (long) + sizeof (long);
}

SCM
BYTES (SCM x)
{
  struct scm *s = &g_cells[x];
  return s->car;
}

SCM
LENGTH (SCM x)
{
  struct scm *s = &g_cells[x];
  return s->car;
}

SCM *
LENGTH_PTR (SCM x)
{
  char *p = g_cells + (x * M2_CELL_SIZE);
  return p + sizeof (long);
}

SCM
REF (SCM x)
{
  struct scm *s = &g_cells[x];
  return s->car;
}

SCM
VARIABLE (SCM x)
{
  struct scm *s = &g_cells[x];
  return s->car;
}

SCM
CLOSURE (SCM x)
{
  struct scm *s = &g_cells[x];
  return s->cdr;
}

SCM
CONTINUATION (SCM x)
{
  struct scm *s = &g_cells[x];
  return s->cdr;
}

SCM *
CONTINUATION_PTR (SCM x)
{
  char *p = g_cells + (x * M2_CELL_SIZE);
  return p  + sizeof (long) + sizeof (long);
}

SCM
MACRO (SCM x)
{
  struct scm *s = &g_cells[x];
  return s->car;
}

SCM
NAME (SCM x)
{
  struct scm *s = &g_cells[x];
  return s->cdr;
}

SCM
PORT (SCM x)
{
  struct scm *s = &g_cells[x];
  return s->car;
}

SCM
STRING (SCM x)
{
  struct scm *s = &g_cells[x];
  return s->cdr;
}

SCM *
STRING_PTR (SCM x)
{
  char *p = g_cells + (x * M2_CELL_SIZE);
  return p  + sizeof (long) + sizeof (long);
}

SCM
STRUCT (SCM x)
{
  struct scm *s = &g_cells[x];
  return s->cdr;
}

SCM
VALUE (SCM x)
{
  struct scm *s = &g_cells[x];
  return s->cdr;
}

SCM *
VALUE_PTR (SCM x)
{
  char *p = g_cells + (x * M2_CELL_SIZE);
  return p  + sizeof (long) + sizeof (long);
}

SCM
VECTOR (SCM x)
{
  struct scm *s = &g_cells[x];
  return s->cdr;
}

SCM *
VECTOR_PTR (SCM x)
{
  char *p = g_cells + (x * M2_CELL_SIZE);
  return p  + sizeof (long) + sizeof (long);
}

SCM
NLENGTH (SCM x)
{
  struct scm *s = &g_news[x];
  return s->car;
}

SCM *
NLENGTH_PTR (SCM x)
{
  char *p = g_cells + (x * M2_CELL_SIZE);
  return p  + sizeof (long);
}

SCM
NVALUE (SCM x)
{
  return (x * M2_CELL_SIZE) + g_news + sizeof (long) + sizeof (long);
}

SCM *
NVALUE_PTR (SCM x)
{
  char *p = g_news + (x * M2_CELL_SIZE);
  return p + sizeof (long) + sizeof (long);
}

SCM
NSTRING (SCM x)
{
  char *p = g_news + (x * M2_CELL_SIZE);
  return p + sizeof (long) + sizeof (long);
}

SCM
NVECTOR (SCM x)
{
  char *p = g_news + (x * M2_CELL_SIZE);
  return p + sizeof (long) + sizeof (long);
}

SCM *
NVECTOR_PTR (SCM x)
{
  char *p = g_news + (x * M2_CELL_SIZE);
  return p + sizeof (long) + sizeof (long);
}

SCM
CAAR (SCM x)
{
  return CAR (CAR (x));
}

SCM
CADR (SCM x)
{
  return CAR (CDR (x));
}

SCM
CDAR (SCM x)
{
  return CDR (CAR (x));
}

SCM
CDDR (SCM x)
{
  return CDR (CDR (x));
}

SCM
CADAR (SCM x)
{
  return CAR (CDR (CAR (x)));
}

SCM
CADDR (SCM x)
{
  return CAR (CDR (CDR (x)));
}

SCM
CDADR (SCM x)
{
  return CDR (CAR (CDR (x)));
}

SCM
CDDAR (SCM x)
{
  return CDR (CDR (CAR (x)));
}

#endif /* M2_FUNCTIONS */
