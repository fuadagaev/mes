/* -*-comment-start: "//";comment-end:""-*-
 * GNU Mes --- Maxwell Equations of Software
 * Copyright © 2016,2017,2018,2019 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
 * Copyright © 2019 Jeremiah Orians
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

#include "mes/lib.h"
#include "mes/mes.h"

SCM GC_SAFETY;
SCM ARENA_SIZE;
SCM MAX_ARENA_SIZE;
SCM JAM_SIZE;
// CONSTANT FRAME_SIZE 5
#define FRAME_SIZE 5

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

struct scm *g_news;

void
initialize_memory ()
{
  g_news = 0;
  MAX_ARENA_SIZE = get_env_value ("MES_MAX_ARENA", 100000000);
  ARENA_SIZE = get_env_value ("MES_ARENA", 10000000);
  JAM_SIZE = get_env_value ("MES_JAM", ARENA_SIZE / 10);
  GC_SAFETY = get_env_value ("MES_SAFETY", ARENA_SIZE / 100);
  STACK_SIZE = get_env_value ("MES_STACK", 20000);
  MAX_STRING = get_env_value ("MES_MAX_STRING", 524288);
}

void
gc_init_cells ()                /* ((internal)) */
{
  SCM stack_size = ((ARENA_SIZE + JAM_SIZE) * sizeof (struct scm)) + (STACK_SIZE * sizeof (SCM));
  g_stack_array = malloc (stack_size);
  g_buf = malloc (MAX_STRING);
}

struct scm *make_char (SCM c);
struct scm *
mes_g_stack (struct scm *a)     /* ((internal)) */
{
  g_stack = STACK_SIZE;
  R0 = a;
  R1 = make_char (0);
  R2 = make_char (0);
  R3 = make_char (0);
  return R0;
}

struct scm *
make_frame (SCM index)
{
  SCM array_index = (STACK_SIZE - (index * FRAME_SIZE));
  struct scm *procedure = g_stack_array[array_index + FRAME_PROCEDURE];

  if (!procedure)
    {
      procedure = cell_f;
    }

  return make_struct (make_frame_type (), cons (cell_symbol_frame, cons (procedure, cell_nil)),
                      cstring_to_symbol ("frame-printer"));
}

struct scm *
make_stack ()                   /* ((arity . n)) */
{
  struct scm *stack_type = make_stack_type ();
  SCM size = (STACK_SIZE - g_stack) / FRAME_SIZE;
  struct scm *frames = make_vector__ (size);
  SCM i;

  for (i = 0; i < size; i = i + 1)
    {
      struct scm *frame = make_frame (i);
      vector_set_x_ (frames, i, frame);
    }

  struct scm *values = cell_nil;
  values = cons (frames, values);
  values = cons (cell_symbol_stack, values);
  return make_struct (stack_type, values, cell_unspecified);
}


struct scm *
make_cell (struct scm *type, struct scm *car, struct scm *cdr)
{
  require (type->type == TNUMBER, "type does not match TNUMBER in src/gc.c: make_cell\n");

  if (type->value == TCHAR || type->value == TNUMBER)
    {
      if (0 != car)
        {
          car = car->car;
        }

      if (0 != cdr)
        {
          cdr = cdr->cdr;
        }
    }

  struct scm *x = malloc (sizeof (struct scm));
  x->type = type->value;
  x->car = car;
  x->cdr = cdr;
  return x;
}


struct scm *
make_bytes (char *s, SCM length)
{
  struct scm *x = malloc (sizeof (struct scm));
  x->type = TBYTES;
  x->length = length;
  x->string = malloc (length + 1);
  char *p = x->string;

  if (0 != length)
    {
      block_copy (s, p, length + 1);
    }

  return x;
}

struct scm *
make_tref (struct scm *y)
{
  struct scm *x = malloc (sizeof (struct scm));
  x->type = TREF;
  x->car = y;
  x->cdr = 0;
  return x;
}

struct scm *
make_vector__ (SCM k)
{
  struct scm *x = malloc (sizeof (struct scm));
  struct scm *v = malloc (k * sizeof (struct scm));
  x->type = TVECTOR;
  x->length = k;
  x->cdr = v;

  for (k = k - 1; k >= 0; k = k - 1)
    {
      v->type = TREF;
      v->car = cell_unspecified;
      v->cdr = 0;
      v = v + CELL_SIZE;
    }

  return x;
}

struct scm *
make_struct (struct scm *type, struct scm *fields, struct scm *printer)
{
  SCM size = 2 + length__ (fields);
  struct scm *v = malloc (size * sizeof (struct scm));
  struct scm *w = v + 1;
  struct scm *entry = vector_entry (type);
  struct scm *print = vector_entry (printer);

  v->type = entry->type;
  v->car = entry->car;
  v->cdr = entry->cdr;

  w->type = print->type;
  w->car = print->car;
  w->cdr = print->cdr;

  SCM i;
  for (i = 2; i < size; i = i + 1)
    {
      struct scm *e = cell_unspecified;

      if (fields != cell_nil)
        {
          e = fields->car;
          fields = fields->cdr;
        }

      entry = vector_entry (e);
      w = v + i;

      w->type = entry->type;
      w->car = entry->car;
      w->cdr = entry->cdr;
    }

  struct scm *r = malloc (sizeof (struct scm));
  r->type = TSTRUCT;
  r->length = size;
  r->cdr = v;
  return r;
}

struct scm *
gc_check ()
{
  return cell_unspecified;
}

struct scm *
gc ()
{
  return cell_unspecified;
}

struct scm *
make_tstring1 (SCM n)
{
  struct scm *x = malloc (sizeof (struct scm));
  x->type = TSTRING;
  x->length = n;
  x->cdr = 0;
  return x;
}

struct scm *
make_tstring2 (struct scm *a, struct scm *b)
{
  struct scm *x = malloc (sizeof (struct scm));
  x->type = TSTRING;
  x->car = a;
  x->cdr = b;
  return x;
}

struct scm *
make_keyword (struct scm *a, struct scm *b)
{
  struct scm *x = malloc (sizeof (struct scm));
  x->type = TKEYWORD;
  x->car = a;
  x->cdr = b;
  return x;
}

struct scm *
make_tsymbol (struct scm *a, struct scm *b)
{
  struct scm *x = malloc (sizeof (struct scm));
  x->type = TSYMBOL;
  x->car = a;
  x->cdr = b;
  return x;
}

struct scm *
make_port (SCM n, struct scm *s)
{
  struct scm *x = malloc (sizeof (struct scm));
  x->type = TPORT;
  x->port = n;
  x->cdr = s;
  return x;
}

struct scm *
make_char (SCM c)
{
  struct scm *x = malloc (sizeof (struct scm));
  x->type = TCHAR;
  x->car = 0;
  x->value = c;
  return x;
}

struct scm *
make_number (SCM n)
{
  struct scm *x = malloc (sizeof (struct scm));
  x->type = TNUMBER;
  x->car = 0;
  x->value = n;
  return x;
}

struct scm *
make_tmacro (struct scm *a, struct scm *b)
{
  struct scm *x = malloc (sizeof (struct scm));
  x->type = TMACRO;
  x->car = a;
  x->cdr = b;
  return x;
}

struct scm *
make_tcontinuation (SCM a, SCM b)
{
  struct scm *x = malloc (sizeof (struct scm));
  x->type = TCONTINUATION;
  x->length = a;
  x->value = b;
  return x;
}

struct scm *
make_tpair (struct scm *a, struct scm *b)
{
  struct scm *x = malloc (sizeof (struct scm));
  x->type = TPAIR;
  x->car = a;
  x->cdr = b;
  return x;
}

struct scm *
make_closure_ (struct scm *args, struct scm *body, struct scm *a)       /* ((internal)) */
{
  struct scm *x = malloc (sizeof (struct scm));
  x->type = TCLOSURE;
  x->car = cell_f;
  x->cdr = cons (cons (cell_circular, a), cons (args, body));
  return x;
}

struct scm *
make_variable_ (struct scm *var)        /* ((internal)) */
{
  struct scm *x = malloc (sizeof (struct scm));
  x->type = TVARIABLE;
  x->car = var;
  x->cdr = 0;
  return x;
}
