/* -*-comment-start: "//";comment-end:""-*-
 * Mes --- Maxwell Equations of Software
 * Copyright © 2016,2017 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
 *
 * This file is part of Mes.
 *
 * Mes is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * Mes is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Mes.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <errno.h>

SCM
gc_up_arena () ///((internal))
{
  ARENA_SIZE *= 2;
  GC_SAFETY *= 2;
#if _POSIX_SOURCE
  void *p = realloc (g_cells-1, 2*ARENA_SIZE*sizeof(struct scm));
#else
  char *p = g_cells;
  p = realloc (p-sizeof (struct scm), 2*ARENA_SIZE*sizeof(struct scm));
#endif

#if _POSIX_SOURCE
  if (!p)
    error (cell_symbol_system_error, cons (MAKE_STRING (cstring_to_list (strerror (errno))), MAKE_NUMBER (g_free)));
  g_cells = (struct scm*)p;
  g_cells++;
#endif
  gc_init_news ();
  return 0;
}

SCM
gc_flip () ///((internal))
{
  struct scm *cells = g_cells;
  g_cells = g_news;
  g_news = cells;
  if (g_debug > 2)
    {
      eputs (";;;   => jam[");
      eputs (itoa (g_free));
      eputs ("]\n");
    }
  return g_stack;
}

SCM
gc_copy (SCM old) ///((internal))
{
  if (TYPE (old) == TBROKEN_HEART)
    return g_cells[old].car;
  SCM new = g_free++;
  g_news[new] = g_cells[old];
  if (NTYPE (new) == TVECTOR)
    {
      NVECTOR (new) = g_free;
      for (int i=0; i<LENGTH (old); i++)
        g_news[g_free++] = g_cells[VECTOR (old)+i];
    }
  TYPE (old) = TBROKEN_HEART;
  CAR (old) = new;
  return new;
}

SCM
gc_relocate_car (SCM new, SCM car) ///((internal))
{
  g_news[new].car = car;
  return cell_unspecified;
}

SCM
gc_relocate_cdr (SCM new, SCM cdr) ///((internal))
{
  g_news[new].cdr = cdr;
  return cell_unspecified;
}

SCM
gc_loop (SCM scan) ///((internal))
{
  while (scan < g_free)
    {
      if (NTYPE (scan) == TCLOSURE
          || NTYPE (scan) == TCONTINUATION
          || NTYPE (scan) == TFUNCTION
          || NTYPE (scan) == TKEYWORD
          || NTYPE (scan) == TMACRO
          || NTYPE (scan) == TPAIR
          || NTYPE (scan) == TREF
          || scan == 1 // null
          || NTYPE (scan) == TSPECIAL
          || NTYPE (scan) == TSTRING
          || NTYPE (scan) == TSYMBOL
          || NTYPE (scan) == TVARIABLE)
        {
          SCM car = gc_copy (g_news[scan].car);
          gc_relocate_car (scan, car);
        }
      if ((NTYPE (scan) == TCLOSURE
           || NTYPE (scan) == TCONTINUATION
           || NTYPE (scan) == TMACRO
           || NTYPE (scan) == TPAIR
           || NTYPE (scan) == TVALUES
           || NTYPE (scan) == TVARIABLE)
          && g_news[scan].cdr) // allow for 0 terminated list of symbols
        {
          SCM cdr = gc_copy (g_news[scan].cdr);
          gc_relocate_cdr (scan, cdr);
        }
      scan++;
    }
  return gc_flip ();
}

SCM
gc_check ()
{
  if (g_free + GC_SAFETY > ARENA_SIZE)
    gc_pop_frame (gc (gc_push_frame ()));
  return cell_unspecified;
}

SCM
gc ()
{
  if (g_debug == 2)
    eputs (".");
  if (g_debug > 2)
    {
      eputs (";;; gc[");
      eputs (itoa (g_free));
      eputs (":");
      eputs (itoa (ARENA_SIZE - g_free));
      eputs ("]...");
    }
  g_free = 1;
  if (g_cells < g_news && ARENA_SIZE < MAX_ARENA_SIZE)
    gc_up_arena ();
  for (int i=g_free; i<g_symbol_max; i++)
    gc_copy (i);
  make_tmps (g_news);
  g_symbols = gc_copy (g_symbols);
  g_macros = gc_copy (g_macros);
  SCM new = gc_copy (g_stack);
  if (g_debug > 3)
    {
      eputs ("new=");
      eputs (itoa (new));
      eputs ("\n");
    }
  g_stack = new;
  return gc_loop (1);
}
