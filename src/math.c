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

#include "mes/lib.h"
#include "mes/mes.h"

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

void
assert_number (char const *name, struct scm* x)
{
  if (x->type != TNUMBER)
    {
      eputs (name);
      error (cell_symbol_not_a_number, x);
    }
}

struct scm*
greater_p (struct scm* x)               /*:((name . ">") (arity . n)) */
{
  if (x == cell_nil)
    return cell_t;
  assert_number ("greater_p", x->car);
  long n = x->car->value;
  x = x->cdr;
  while (x != cell_nil)
    {
      assert_number ("greater_p", x->car);
      struct scm* i = car (x);
      long v = i->value;
      if (v >= n)
        return cell_f;
      n = v;
      x = cdr (x);
    }
  return cell_t;
}

struct scm*
less_p (struct scm* x)                  /*:((name . "<") (arity . n)) */
{
  if (x == cell_nil)
    return cell_t;
  assert_number ("less_p", x->car);
  long n = x->car->value;
  x = x->cdr;
  while (x != cell_nil)
    {
      assert_number ("less_p", x->car);
      struct scm* i = car (x);
      long v = i->value;
      if (v <= n)
        return cell_f;
      n = v;
      x = cdr (x);
    }
  return cell_t;
}

struct scm*
is_p (struct scm* x)                    /*:((name . "=") (arity . n)) */
{
  if (x == cell_nil)
    return cell_t;
  assert_number ("is_p", x->car);
  long n = x->car->value;
  x = cdr (x);
  while (x != cell_nil)
    {
      struct scm* i = car (x);
      long v = i->value;
      if (v != n)
        return cell_f;
      x = cdr (x);
    }
  return cell_t;
}

struct scm*
minus (struct scm* x)                   /*:((name . "-") (arity . n)) */
{
  assert_number ("minus", x->car);
  long n = x->car->value;
  x = cdr (x);
  if (x == cell_nil)
    n = -n;
  while (x != cell_nil)
    {
      struct scm* i = car (x);
      assert_number ("minus", i);
      long v = i->value;
      n = n - v;
      x = cdr (x);
    }
  return make_number (n);
}

struct scm*
plus (struct scm* x)                    /*:((name . "+") (arity . n)) */
{
  long n = 0;
  while (x != cell_nil)
    {
      struct scm* i = car (x);
      assert_number ("plus", i);
      long v = i->value;
      n = n + v;
      x = cdr (x);
    }
  return make_number (n);
}

struct scm*
divide (struct scm* x)                  /*:((name . "/") (arity . n)) */
{
  long n = 1;
  if (x != cell_nil)
    {
      struct scm* i = car (x);
      assert_number ("divide", i);
      long v = i->value;
      n = v;
      x = cdr (x);
    }
  while (x != cell_nil)
    {
      struct scm* i = car (x);
      assert_number ("divide", i);
      long v = i->value;
      if (v == 0)
        error (cstring_to_symbol ("divide-by-zero"), x);
      if (n == 0)
        break;
      n = n / v;
      x = cdr (x);
    }
  return make_number (n);
}

struct scm*
modulo (struct scm* a, struct scm* b)
{
  assert_number ("modulo", a);
  assert_number ("modulo", b);
  long x = a->value;
  long y = b->value;
  if (y == 0)
    error (cstring_to_symbol ("divide-by-zero"), a);
  while (x < 0)
    x = x + y;
  if (x != 0)
    x = x % y;

  return make_number (x);
}

struct scm*
multiply (struct scm* x)                /*:((name . "*") (arity . n)) */
{
  long n = 1;
  while (x != cell_nil)
    {
      struct scm* i = car (x);
      assert_number ("multiply", i);
      long v = i->value;
      n = n * v;
      x = cdr (x);
    }
  return make_number (n);
}

struct scm*
logand (struct scm* x)                  /*:((arity . n)) */
{
  long n = 0;
  while (x != cell_nil)
    {
      struct scm* i = car (x);
      assert_number ("multiply", i);
      long v = i->value;
      n = n & v;
      x = cdr (x);
    }
  return make_number (n);
}

struct scm*
logior (struct scm* x)                  /*:((arity . n)) */
{
  long n = 0;
  while (x != cell_nil)
    {
      struct scm* i = car (x);
      assert_number ("logior", i);
      long v = i->value;
      n = n | v;
      x = cdr (x);
    }
  return make_number (n);
}

struct scm*
lognot (struct scm* x)
{
  assert_number ("lognot", x);
  long n = ~x->value;
  return make_number (n);
}

struct scm*
logxor (struct scm* x)                  /*:((arity . n)) */
{
  long n = 0;
  while (x != cell_nil)
    {
      struct scm* i = car (x);
      assert_number ("logxor", i);
      long v = i->value;
      n = n ^ v;
      x = cdr (x);
    }
  return make_number (n);
}

struct scm*
ash (struct scm* n, struct scm* count)
{
  assert_number ("ash", n);
  assert_number ("ash", count);
  long cn = n->value;
  long ccount = count->value;
  long result;
  if (ccount < 0)
    result = cn >> -ccount;
  else
    result = cn << ccount;
  return make_number (result);
}
