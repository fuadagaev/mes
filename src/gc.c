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

/* DUMP */
#include <fcntl.h>
int dump_fd;
#define ltoa(x) ntoab(x, 10, 0)
#define oputc(x) fdputc(x, dump_fd)
#define oputs(x) fdputs(x, dump_fd)

// CONSTANT FRAME_SIZE 5
#define FRAME_SIZE 5

#if __M2_PLANET__
#define M2_CELL_SIZE 12
// CONSTANT M2_CELL_SIZE 12
#else
#define M2_CELL_SIZE 1
// CONSTANT M2_CELL_SIZE 12
#endif

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

void
gc_init ()
{
#if SYSTEM_LIBC
  ARENA_SIZE = 100000000;       /* 2.3GiB */
#elif ! __M2_PLANET__
  ARENA_SIZE = 300000;          /* 32b: 3MiB, 64b: 6 MiB */
  ARENA_SIZE = 600000;          /* 32b: 6MiB, 64b: 12 MiB */
#else
  ARENA_SIZE = 20000000;
#endif
  STACK_SIZE = 20000;

#if !POINTER_CELLS
  JAM_SIZE = 20000;
  MAX_ARENA_SIZE = 100000000;
#elif !__M2_PLANET__
  JAM_SIZE = 10;
  MAX_ARENA_SIZE = 10000000;
#else
  JAM_SIZE = 10;
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
#if !POINTER_CELLS || GC_NOFLIP
  long alloc_bytes = arena_bytes + (STACK_SIZE * sizeof (struct scm));
#else
  long alloc_bytes = (arena_bytes * 2) + (STACK_SIZE * sizeof (struct scm*));
#endif

  g_arena = malloc (alloc_bytes);
  g_cells = g_arena;

#if !POINTER_CELLS || GC_NOFLIP
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

  /* FIXME: remove MES_MAX_STRING, grow dynamically. */
  g_buf = malloc (MAX_STRING);
}

long
gc_free ()
{
#if POINTER_CELLS
  return (g_free - g_cells) / M2_CELL_SIZE;
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
  i = i / M2_CELL_SIZE;
#endif
  eputs (where);
  eputs (": [");
  eputs (ltoa (i));
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
  i = i / M2_CELL_SIZE;
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
  i = i / M2_CELL_SIZE;
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

void
gc_init_news ()
{
#if !POINTER_CELLS
  g_news = g_cells + g_free;
  struct scm* ncell_arena = cell_arena;
#else

#if GC_NOFLIP
  g_news = g_free;
#else
  char* p = g_cells - M2_CELL_SIZE;
  if (p == g_arena)
    g_news = g_free;
  else
    g_news = g_arena;
#endif

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
}

void
gc_up_arena ()
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
  long realloc_bytes = arena_bytes + (STACK_SIZE * sizeof (struct scm));
#else
  long stack_offset = (arena_bytes * 2);
  long realloc_bytes = (arena_bytes * 2) + (STACK_SIZE * sizeof (struct scm));
#endif
  void *p = realloc (g_cells - M2_CELL_SIZE, realloc_bytes);
  if (p == 0)
    {
      eputs ("realloc failed, g_free=");
      eputs (ltoa (g_free));
      eputs (":");
#if !POINTER_CELLS
      long i = g_free;
#else
      long i = g_free - g_cells;
      i = i / M2_CELL_SIZE;
#endif
      eputs (ltoa (ARENA_SIZE - i));
      eputs ("\n");
      assert_msg (0, "0");
      exit (1);
    }
  g_cells = p;
  memcpy (p + stack_offset, p + old_arena_bytes, STACK_SIZE * sizeof (struct scm*));
  g_cells = g_cells + M2_CELL_SIZE;
}

void
gc_cellcpy (struct scm *dest, struct scm *src, size_t n)
{
  void *p = src;
  void *q = dest;
  long dist = p - q;
  while (n != 0)
    {
      long t = src->type;
      long a = src->car;
      long d = src->cdr;
      dest->type = t;
      if (t == TBROKEN_HEART)
        assert_msg (0, "gc_cellcpy: broken heart");
      if (t == TMACRO
          || t == TPAIR
          || t == TREF
          || t == TVARIABLE)
        dest->car = a - dist;
      else
        dest->car = a;
      if (t == TBYTES
          || t == TCLOSURE
          || t == TCONTINUATION
          || t == TKEYWORD
          || t == TMACRO
          || t == TPAIR
          || t == TPORT
          || t == TSPECIAL
          || t == TSTRING
          || t == TSTRUCT
          || t == TSYMBOL
          || t == TVALUES
          || t == TVECTOR)
        dest->cdr = d - dist;
      else
        dest->cdr = d;
      if (t == TBYTES)
        {
#if GC_TEST
          eputs ("copying bytes[");
          eputs (ntoab (&src->cdr, 16, 0));
          eputs (", ");
          eputs (ntoab (a, 10, 0));
          eputs ("]: ");
          eputs (&src->cdr);
          eputs ("\n to [");
          eputs (ntoab (&dest->cdr, 16, 0));
#endif
          memcpy (&dest->cdr, &src->cdr, a);
#if GC_TEST
          eputs ("]: ");
          eputs (&dest->cdr);
          eputs ("\n");
#endif
          int i = bytes_cells (a);
          n = n - i;
          int c = i * M2_CELL_SIZE;
          dest = dest + c;
          src = src + c;
        }
      else
        {
          n = n - 1;
          dest = dest + M2_CELL_SIZE;
          src = src + M2_CELL_SIZE;
        }
    }
}

void
gc_flip ()
{
#if POINTER_CELLS
  if (g_free - g_news > JAM_SIZE)
    JAM_SIZE = (g_free - g_news) + ((g_free - g_news) / 2);

#if GC_NOFLIP
  cell_arena = g_cells - M2_CELL_SIZE; /* FIXME? */
  gc_cellcpy (g_cells, g_news, (g_free - g_news) / M2_CELL_SIZE);

  void *p = g_news;
  void *q = g_cells;
  long dist = p - q;

  long i;
  i = g_free;
  g_free = i - dist;
#if !GC_TEST
  i = g_symbols;
  g_symbols = i - dist;
  i = g_macros;
  g_macros = i - dist;
  i = g_ports;
  g_ports = i - dist;
  i = M0;
  M0 = i - dist;
#endif

  for (i = g_stack; i < STACK_SIZE; i = i + 1)
    {
      long s = g_stack_array[i];
      /* copy_stack (i, gc_copy (g_stack_array[i])); */
      g_stack_array[i] = s - dist;
    }

#else

  g_cells = g_news;
  cell_arena = g_news - M2_CELL_SIZE;
  cell_zero = cell_arena + M2_CELL_SIZE;
  cell_nil = cell_zero + M2_CELL_SIZE;
#endif
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
      size_t length = new->length;
      memcpy (dest, src, length);
      g_free = g_free + ((bytes_cells (length) - 1) * M2_CELL_SIZE);

      if (g_debug > 4)
        {
          eputs ("gc copy bytes: ");
          eputs (src);
          eputs ("\n");
          eputs ("    length: ");
          eputs (ltoa (old->length));
          eputs ("\n");
          eputs ("    nlength: ");
          eputs (ltoa (new->length));
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
gc_loop (struct scm* scan)
{
  struct scm* car;
  struct scm* cdr;
  while (scan < g_free)
    {
      long t = scan->type;
      if (t == TBROKEN_HEART)
        assert_msg (0, "gc_loop: broken heart");
      /* *INDENT-OFF* */
      if (t == TMACRO
          || t == TPAIR
          || t == TREF
          || t == TVARIABLE)
        /* *INDENT-ON* */
        {
          car = gc_copy (scan->car);
          gc_relocate_car (scan, car);
        }
      /* *INDENT-OFF* */
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
        /* *INDENT-ON* */
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
#if !POINTER_CELLS
  long used = g_free + GC_SAFETY;
#else
  long used = ((g_free - g_cells) / M2_CELL_SIZE) + GC_SAFETY;
#endif
  if (used >= ARENA_SIZE)
    return gc ();
  return cell_unspecified;
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
      eputs (ltoa (ARENA_SIZE - gc_free ()));
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
          eputs (ltoa (g_cells));
          eputs (",");
          eputs (ltoa (g_news));
          eputs (":");
          eputs (ltoa (ARENA_SIZE));
          eputs (",");
          eputs (ltoa (MAX_ARENA_SIZE));
          eputs ("]...");
        }
      gc_up_arena ();
    }

  struct scm* new_cell_nil = g_free;
  struct scm* s;
  for (s = cell_nil; s < g_symbol_max; s = s + M2_CELL_SIZE)
    gc_copy (s);

#if POINTER_CELLS && !GC_NOFLIP
  cell_nil = new_cell_nil;
  cell_arena = g_news - M2_CELL_SIZE; /* for debugging */

#if GC_TEST
  cell_zero = cell_nil - M2_CELL_SIZE;
  g_symbol_max = g_free;
#else
  long save_gsymbols = g_symbols;
  g_symbols = 0;
  g_free = new_cell_nil;
  init_symbols_ ();
  g_symbol_max = g_symbol;
  g_symbols = save_gsymbols;
#endif

#endif

#if !GC_TEST
  g_symbols = gc_copy (g_symbols);
  g_macros = gc_copy (g_macros);
  g_ports = gc_copy (g_ports);
  M0 = gc_copy (M0);

  long i;
  for (i = g_stack; i < STACK_SIZE; i = i + 1)
    copy_stack (i, gc_copy (g_stack_array[i]));
#endif

  gc_loop (new_cell_nil);
}

struct scm*
gc ()
{
  gc_dump_arena (g_cells, gc_free ());
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
#if POINTER_CELLS && !GC_NOFLIP
  gc_ ();
#endif
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
  gc_dump_arena (g_cells, gc_free ());
  return cell_unspecified;
}

void
gc_push_frame ()
{
  if (g_stack < FRAME_SIZE)
    assert_msg (0, "STACK FULL");
  g_stack_array[g_stack - 1] = cell_f;
  g_stack_array[g_stack - 2] = R0;
  g_stack_array[g_stack - 3] = R1;
  g_stack_array[g_stack - 4] = R2;
  g_stack_array[g_stack - 5] = R3;
  g_stack = g_stack - FRAME_SIZE;
}

void
gc_peek_frame ()
{
  R3 = g_stack_array[g_stack];
  R2 = g_stack_array[g_stack + 1];
  R1 = g_stack_array[g_stack + 2];
  R0 = g_stack_array[g_stack + 3];
  g_stack_array[g_stack + FRAME_PROCEDURE];
}

void
gc_pop_frame ()
{
  gc_peek_frame ();
  g_stack = g_stack + FRAME_SIZE;
}

void
gc_dump_register (char const* n, struct scm* r)
{
  oputs (n); oputs (": ");
#if !POINTER_CELLS
  long i = r;
#else
  long i = r;
  long a = g_arena;
  i = i - a;
  i = i / M2_CELL_SIZE;
#endif
  oputs (ltoa (i));
  oputs ("\n");
}

void
gc_dump_state ()
{
  if (!dump_fd)
    dump_fd = mes_open ("dump.mo", O_CREAT|O_WRONLY, 0644);
  gc_dump_register ("R0", R0);
  gc_dump_register ("R1", R1);
  gc_dump_register ("R2", R2);
  gc_dump_register ("R3", R3);
  gc_dump_register ("M0", M0);
  gc_dump_register ("g_symbols", g_symbols);
  gc_dump_register ("g_symbol_max", g_symbol_max);
  gc_dump_register ("g_macros", g_macros);
  gc_dump_register ("g_ports", g_ports);
  gc_dump_register ("cell_zero", cell_zero);
  gc_dump_register ("cell_nil", cell_nil);
}

void
gc_dump_stack ()
{
  long i = g_stack;
  while (i < STACK_SIZE)
    {
      gc_dump_register (itoa (i), g_stack_array[i]);
      i = i + 1;
    }
}

void
gc_dump_arena (struct scm *cells, long size)
{
#if !POINTER_CELLS
  struct scm *dist = 0;
#else
  struct scm *dist = cells;
#endif
  if (!dump_fd)
    dump_fd = mes_open ("dump.mo", O_CREAT|O_WRONLY, 0644);
  oputs ("stack="); oputs (ltoa (g_stack)); oputc ('\n');
  oputs ("size="); oputs (ltoa (size)); oputc ('\n');
  gc_dump_state ();
  gc_dump_stack ();
  while (cells[size].type == 0 && cells[size].car == 0 && cells[size].cdr == 0)
    size = size - 1;
  while (size > 0)
    {
      int i;
      for (i=0; i < 16; i = i + 1)
        {
          long t = cells->type;
          long a = cells->car;
          long d = cells->cdr;
          if (size == 0)
            oputs ("0 0 0");
          else
            {
              oputs (ltoa (t));
              oputc (' ');
#if POINTER_CELLS
              if (t == TMACRO
                  || t == TPAIR
                  || t == TREF
                  || t == TVARIABLE)
                {
                  oputs (ltoa ((cells->car - dist) / M2_CELL_SIZE));
                  /* oputs ("["); oputs (ltoa (a)); oputs ("]"); */
                }
              else
#endif
                oputs (ltoa (a));
              oputc (' ');
              if (t != TBYTES)
                {
#if POINTER_CELLS
                  if (t == TCLOSURE
                      || t == TCONTINUATION
                      || t == TKEYWORD
                      || t == TMACRO
                      || t == TPAIR
                      || t == TPORT
                      || t == TSPECIAL
                      || t == TSTRING
                      || t == TSTRUCT
                      || t == TSYMBOL
                      || t == TVALUES
                      || t == TVECTOR)
                    {
                      oputs (ltoa ((cells->cdr - dist) / M2_CELL_SIZE));
                      /* oputs ("["); oputs (ltoa (d)); oputs ("]"); */
                    }
                  else
#endif
                    if (t == TNUMBER && d > 1000)
                      oputs (ltoa (1001));
                    else
                      oputs (ltoa (d));
                }
              if (t == TBYTES)
                {
                  int c = bytes_cells (a);
                  char *p = &cells->cdr;
                  size = size - c;
                  oputc ('"');
                  while (a > 0)
                    {
                      if (p[0] != 0)
                        oputc (p[0]);
                      p = p + 1;
                      a = a - 1;
                    }
                  oputc ('"');
                  cells = cells + c * M2_CELL_SIZE;
                  size = size - c;
                }
#if 0
              else if (t == TSTRUCT)
                {
                  cells = cells + (a + 1) * M2_CELL_SIZE;
                  size = size - a - 1;
                }
#endif
              else
                {
                  cells = cells + M2_CELL_SIZE;
                  size = size - 1;
                }
            }
          if (i != 15)
            oputs ("  ");
          else
            oputc ('\n');
        }
      oputc ('\n');
    }
}
