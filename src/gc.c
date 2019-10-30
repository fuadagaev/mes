/* -*-comment-start: "//";comment-end:""-*-
 * GNU Mes --- Maxwell Equations of Software
 * Copyright © 2016,2017 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
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

#include <errno.h>
#include <string.h>
#include <stdlib.h>

// CONSTANT FRAME_SIZE 5
#define FRAME_SIZE 5

#if __M2_PLANET__
#define M2_CELL_SIZE 12
// CONSTANT M2_CELL_SIZE 12
#else
#define M2_CELL_SIZE 1
// CONSTANT M2_CELL_SIZE 12
#endif

#if !POINTER_CELLS
struct scm* g_stack;
#else
long g_stack;
#endif

#if !POINTER_CELLS
long g_symbol;
#else
struct scm* g_symbol;
#endif

struct scm* *g_stack_array;

struct scm *g_cells;
struct scm *g_news;

char *
cell_bytes (struct scm* x)
{
#if POINTER_CELLS
  char *p = x;
  return p + (2 * sizeof (long));
#else
  return &x->cdr;
#endif
}

char *
news_bytes (struct scm* x)
{
#if POINTER_CELLS
  char *p = x;
  return p + (2 * sizeof (long));
#else
  return &x->cdr;
#endif
}

struct scm*
gc_init ()                      /*:((internal)) */
{
#if SYSTEM_LIBC
  ARENA_SIZE = 100000000;       /* 2.3GiB */
#elif ! __M2_PLANET__
  ARENA_SIZE = 300000;          /* 32b: 3MiB, 64b: 6 MiB */
#else
  ARENA_SIZE = 20000000;
#endif
  STACK_SIZE = 20000;

#if !POINTER_CELLS
  JAM_SIZE = 20000;
  MAX_ARENA_SIZE = 100000000;
#elif !__M2_PLANET__
  JAM_SIZE = 1000;
  MAX_ARENA_SIZE = 10000000;
#else
  JAM_SIZE = 20000;
  MAX_ARENA_SIZE = 20000000;
#endif
  GC_SAFETY = 2000;
  MAX_STRING = 524288;

  char *p;
  p = getenv ("MES_MAX_ARENA");
  if (p != 0)
    MAX_ARENA_SIZE = atoi (p);
  p = getenv ("MES_ARENA");
  if (p != 0)
    ARENA_SIZE = atoi (p);
  JAM_SIZE = ARENA_SIZE / 10;
  p = getenv ("MES_JAM");
  if (p != 0)
    JAM_SIZE = atoi (p);
  GC_SAFETY = ARENA_SIZE / 100;
  p = getenv ("MES_SAFETY");
  if (p != 0)
    GC_SAFETY = atoi (p);
  p = getenv ("MES_STACK");
  if (p != 0)
    STACK_SIZE = atoi (p);
  p = getenv ("MES_MAX_STRING");
  if (p != 0)
    MAX_STRING = atoi (p);

  long arena_bytes = (ARENA_SIZE + JAM_SIZE) * sizeof (struct scm);
#if! POINTER_CELLS
  long alloc_bytes = arena_bytes + (STACK_SIZE * sizeof (struct scm*));
#else
  long alloc_bytes = (arena_bytes * 2) + (STACK_SIZE * sizeof (struct scm*));
#endif
  g_arena = malloc (alloc_bytes);
  g_cells = g_arena;

#if! POINTER_CELLS
  g_stack_array = g_arena + arena_bytes;
#else
  g_stack_array = g_arena + (arena_bytes * 2);
#endif

#if !POINTER_CELLS
  /* The vector that holds the arenea. */
  cell_arena = -1;
#else
  /* The vector that holds the arenea. */
  cell_arena = g_cells;
#endif

  cell_zero = cell_arena + M2_CELL_SIZE;

  g_cells = g_cells + M2_CELL_SIZE; /* Hmm? */

  cell_arena->type = TVECTOR;
  cell_arena->length = 1000;
  cell_arena->vector = cell_zero;

  cell_zero->type = TCHAR;
  cell_zero->value = 'c';


#if !POINTER_CELLS
  g_free = 1;
#else
  g_free = g_cells + M2_CELL_SIZE;
#endif
  g_symbols = 0;
  g_symbol_max = 0;
  g_macros = 0;
  g_ports = 0;

  /* FIXME: remove MES_MAX_STRING, grow dynamically. */
  g_buf = malloc (MAX_STRING);

  return 0;
}

long
gc_free ()
{
#if POINTER_CELLS
  return g_free - g_cells;
#else
  return g_free;
#endif
}

void
gc_stats_ (char const* where)
{
#if !POINTER_CELLS
  long i = g_free;
#else
  long i = g_free - g_cells;
#endif
  eputs (where);
  eputs (": [");
  eputs (ntoab (i, 10, 0));
  eputs ("]\n");
}

struct scm*
alloc (long n)
{
  struct scm* x = g_free;
  g_free = g_free + (n * M2_CELL_SIZE);
#if !POINTER_CELLS
  long i = g_free;
#else
  long i = g_free - g_cells;
#endif
  if (i > ARENA_SIZE)
    assert_msg (0, "alloc: out of memory");
  return x;
}

struct scm*
make_cell (long type, struct scm* car, struct scm* cdr)
{
  struct scm* x = g_free;
  g_free = g_free + M2_CELL_SIZE;
#if !POINTER_CELLS
  long i = g_free;
#else
  long i = g_free - g_cells;
#endif
  if (i > ARENA_SIZE)
    assert_msg (0, "alloc: out of memory");
  x->type = type;
  x->car = car;
  x->cdr = cdr;
  return x;
}

void
copy_cell (struct scm* to, struct scm* from)
{
  to->type = from->type;
  to->car = from->car;
  to->cdr = from->cdr;
}

void
copy_news (struct scm* to, struct scm* from)
{
  to->type = from->type;
  to->car = from->car;
  to->cdr = from->cdr;
}

void
copy_stack (long index, struct scm* from)
{
  g_stack_array[index] = from;
}

struct scm*
cell_ref (struct scm* cell, long index)
{
  return cell + (index * M2_CELL_SIZE);
}

struct scm*
cons (struct scm* x, struct scm* y)
{
  return make_cell (TPAIR, x, y);
}

size_t
bytes_cells (size_t length)
{
  return (sizeof (long) + sizeof (long) + length - 1 + sizeof (struct scm*)) / sizeof (struct scm*);
}

struct scm*
make_bytes (char const *s, size_t length)
{
  size_t size = bytes_cells (length);
  struct scm* x = alloc (size);
  x->type = TBYTES;
  x->length = length;
  char *p = cell_bytes (x);
  if (length == 0)
    p[0] = 0;
  else
    memcpy (p, s, length);

  return x;
}

struct scm*
make_char (int n)
{
  return make_cell (TCHAR, 0, n);
}

struct scm*
make_continuation (long n)
{
  return make_cell (TCONTINUATION, n, g_stack);
}

struct scm*
make_macro (struct scm* name, struct scm* x)    /*:((internal)) */
{
  return make_cell (TMACRO, x, name->string);
}

struct scm*
make_number (long n)
{
  return make_cell (TNUMBER, 0, n);
}

struct scm*
make_ref (struct scm* x)                /*:((internal)) */
{
  return make_cell (TREF, x, 0);
}

struct scm*
make_string (char const *s, size_t length)
{
  if (length > MAX_STRING)
    assert_max_string (length, "make_string", s);
  struct scm* x = make_cell (TSTRING, length, 0);
  struct scm* v = make_bytes (s, length + 1);
  x->cdr = v;
  return x;
}

struct scm*
make_string0 (char const *s)
{
  return make_string (s, strlen (s));
}

struct scm*
make_string_port (struct scm* x)        /*:((internal)) */
{
  return make_cell (TPORT, -length__ (g_ports) - 2, x);
}

struct scm*
gc_init_news ()                 /*:((internal)) */
{
#if !POINTER_CELLS
  g_news = g_cells + g_free;
  struct scm* ncell_arena = cell_arena;
#else
  g_news = g_free;
  struct scm* ncell_arena = g_news;
#endif

  struct scm* ncell_zero = ncell_arena + M2_CELL_SIZE;

  g_news = g_news + M2_CELL_SIZE;

  ncell_arena->type = TVECTOR;
  ncell_arena->length = cell_arena->length;

#if !POINTER_CELLS
  ncell_arena->vector = 0;
#else
  ncell_arena->vector = g_news;
#endif

  ncell_zero->type = TCHAR;
  ncell_zero->value = 'n';

  return 0;
}

struct scm*
gc_up_arena ()                  /*:((internal)) */
{
  long old_arena_bytes = (ARENA_SIZE + JAM_SIZE) * sizeof (struct scm);
  if (ARENA_SIZE >> 1 < MAX_ARENA_SIZE >> 2)
    {
      ARENA_SIZE = ARENA_SIZE << 1;
      JAM_SIZE = JAM_SIZE << 1;
      GC_SAFETY = GC_SAFETY << 1;
    }
  else
    ARENA_SIZE = MAX_ARENA_SIZE - JAM_SIZE;
  long arena_bytes = (ARENA_SIZE + JAM_SIZE) * sizeof (struct scm);
#if !POINTER_CELLS
  long stack_offset = arena_bytes;
  long realloc_bytes = (arena_bytes * 2) + (STACK_SIZE * sizeof (struct scm));
#else
  long stack_offset = (arena_bytes * 2);
  long realloc_bytes = arena_bytes + (STACK_SIZE * sizeof (struct scm));
#endif
void *p = realloc (g_cells - M2_CELL_SIZE, realloc_bytes);
  if (p == 0)
    {
      eputs ("realloc failed, g_free=");
      eputs (itoa (g_free));
      eputs (":");
#if !POINTER_CELLS
      long i = g_free;
#else
      long i = g_free - g_cells;
#endif
      eputs (itoa (ARENA_SIZE - i));
      eputs ("\n");
      assert_msg (0, "0");
      exit (1);
    }
  g_cells = p;

  memcpy (p + stack_offset, p + old_arena_bytes, STACK_SIZE * sizeof (struct scm*));
  g_cells = g_cells + M2_CELL_SIZE;

  return 0;
}

void
gc_flip ()
{
#if POINTER_CELLS
  g_cells = g_news;
  cell_arena = g_news - M2_CELL_SIZE;
  cell_zero = cell_arena + M2_CELL_SIZE;
  cell_nil = cell_zero + M2_CELL_SIZE;
#endif

  if (g_debug > 2)
    gc_stats_ (";;; => jam");

#if POINTER_CELLS
  /* Nothing. */
  return;
#else
  if (g_free > JAM_SIZE)
    JAM_SIZE = g_free + g_free / 2;
  memcpy (g_cells, g_news, g_free * sizeof (struct scm));
#endif
}

struct scm*
gc_copy (struct scm* old)               /*:((internal)) */
{
  if (old->type == TBROKEN_HEART)
    return old->car;
  struct scm* new = g_free;
  g_free = g_free + M2_CELL_SIZE;
  copy_news (new, old);
  if (new->type == TSTRUCT || new->type == TVECTOR)
    {
      new->vector = g_free;
      long i;
      for (i = 0; i < old->length; i = i + 1)
        {
          copy_news (g_free, cell_ref (old->vector, i));
          g_free = g_free + M2_CELL_SIZE;
        }
    }
  else if (new->type == TBYTES)
    {
      char const *src = cell_bytes (old);
      char *dest = news_bytes (new);
#if !POINTER_CELLS
      size_t length = new->length;
#else
      size_t length = old->length;
#endif
      memcpy (dest, src, length);
      g_free = g_free + ((bytes_cells (length) - 1) * M2_CELL_SIZE);

      if (g_debug > 4)
        {
          eputs ("gc copy bytes: ");
          eputs (src);
          eputs ("\n");
          eputs ("    length: ");
          eputs (itoa (old->length));
          eputs ("\n");
          eputs ("    nlength: ");
          eputs (itoa (new->length));
          eputs ("\n");
          eputs ("        ==> ");
          eputs (dest);
          eputs ("\n");
        }
    }
  old->type = TBROKEN_HEART;
  old->car = new;
  return new;
}

struct scm*
gc_relocate_car (struct scm* new, struct scm* car)      /*:((internal)) */
{
  new->car = car;
  return cell_unspecified;
}

struct scm*
gc_relocate_cdr (struct scm* new, struct scm* cdr)      /*:((internal)) */
{
  new->cdr = cdr;
  return cell_unspecified;
}

void
gc_loop (struct scm* scan)              /*:((internal)) */
{
  struct scm* car;
  struct scm* cdr;
  while (scan < g_free)
    {
      long t = scan->type;
      if (t == TBROKEN_HEART)
        assert_msg (0, "broken heart");
      if (t == TMACRO
          || t == TPAIR
          || t == TREF
          || t == TVARIABLE)
        {
          car = gc_copy (scan->car);
          gc_relocate_car (scan, car);
        }
      if (t == TCLOSURE
          || t == TCONTINUATION
          || t == TKEYWORD
          || t == TMACRO
          || t == TPAIR
          || t == TPORT
          || t == TSPECIAL
          || t == TSTRING
          /*|| t == TSTRUCT handled by gc_copy */
          || t == TSYMBOL
          || t == TVALUES
          /*|| t == TVECTOR handled by gc_copy */
          )
        {
          cdr = gc_copy (scan->cdr);
          gc_relocate_cdr (scan, cdr);
        }
      if (t == TBYTES)
        scan = scan + (bytes_cells (scan->length) * M2_CELL_SIZE);
      else
        scan = scan + M2_CELL_SIZE;
    }
  gc_flip ();
}

struct scm*
gc_check ()
{
#if POINTER_CELLS
  if ((g_free - g_cells) + GC_SAFETY < ARENA_SIZE)
    return cell_unspecified;
#else
  if (g_free + GC_SAFETY < ARENA_SIZE)
    return cell_unspecified;
#endif
  return gc ();
}

void
gc_ ()
{
  gc_init_news ();
  if (g_debug == 2)
    eputs (".");
  if (g_debug > 2)
    {
      gc_stats_ (";;; gc");
      eputs (";;; free: [");
      eputs (itoa (ARENA_SIZE - gc_free ()));
      eputs ("]...");
    }
#if !POINTER_CELLS
  g_free = 1;
#else
  g_free = g_news + M2_CELL_SIZE;
#endif

  if (ARENA_SIZE < MAX_ARENA_SIZE && g_cells == g_arena + M2_CELL_SIZE)
    {
      if (g_debug == 2)
        eputs ("+");
      if (g_debug > 2)
        {
          eputs (" up[");
          eputs (itoa (g_cells));
          eputs (",");
          eputs (itoa (g_news));
          eputs (":");
          eputs (itoa (ARENA_SIZE));
          eputs (",");
          eputs (itoa (MAX_ARENA_SIZE));
          eputs ("]...");
        }
      gc_up_arena ();
    }

#if POINTER_CELLS
  struct scm* save_gfree = g_free;
#endif
  struct scm* s;
  for (s = cell_nil; s < g_symbol_max; s = s + M2_CELL_SIZE)
    gc_copy (s);

#if POINTER_CELLS
#if GC_TEST
  cell_nil = save_gfree;
#else
  long save_gsymbols = g_symbols;
  cell_nil = save_gfree;
  g_symbols = 0;
  g_free = save_gfree;
  init_symbols_ ();
  g_symbol_max = g_symbol;
  g_symbols = save_gsymbols;
#endif
#endif

  g_symbols = gc_copy (g_symbols);
  g_macros = gc_copy (g_macros);
  g_ports = gc_copy (g_ports);
  M0 = gc_copy (M0);
  long i;
  for (i = g_stack; i < STACK_SIZE; i = i + 1)
    copy_stack (i, gc_copy (g_stack_array[i]));

  gc_loop (cell_nil);
}

struct scm*
gc ()
{
  if (g_debug > 5)
    {
      eputs ("symbols: ");
      write_error_ (g_symbols);
      eputs ("\n");
      eputs ("R0: ");
      write_error_ (R0);
      eputs ("\n");
    }
  gc_push_frame ();
  gc_ ();
  gc_pop_frame ();
  if (g_debug > 5)
    {
      eputs ("symbols: ");
      write_error_ (g_symbols);
      eputs ("\n");
      eputs ("R0: ");
      write_error_ (R0);
      eputs ("\n");
    }
  return cell_unspecified;
}

struct scm*
gc_push_frame ()                /*:((internal)) */
{
  if (g_stack < FRAME_SIZE)
    assert_msg (0, "STACK FULL");
  g_stack_array[g_stack - 1] = cell_f;
  g_stack_array[g_stack - 2] = R0;
  g_stack_array[g_stack - 3] = R1;
  g_stack_array[g_stack - 4] = R2;
  g_stack_array[g_stack - 5] = R3;
  g_stack = g_stack - FRAME_SIZE;
  return g_stack;
}

struct scm*
gc_peek_frame ()                /*:((internal)) */
{
  R3 = g_stack_array[g_stack];
  R2 = g_stack_array[g_stack + 1];
  R1 = g_stack_array[g_stack + 2];
  R0 = g_stack_array[g_stack + 3];
  return g_stack_array[g_stack + FRAME_PROCEDURE];
}

struct scm*
gc_pop_frame ()                 /*:((internal)) */
{
  struct scm* x = gc_peek_frame ();
  g_stack = g_stack + FRAME_SIZE;
  return x;
}
