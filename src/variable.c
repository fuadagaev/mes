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

struct scm *
variable_ref (struct scm *var)
{
  assert_variable (1, var);
  struct scm *ref = var->variable;
  struct scm *value = ref->cdr;
  if (value == cell_undefined)
    error (cell_symbol_unbound_variable, var);
  return value;
}

struct scm *
flat_variable_ref (struct scm *var)
{
  assert_variable (1, var);
  struct scm *value = var->variable;
  if (value == cell_undefined)
    error (cell_symbol_unbound_variable, var);
  return value;
}

struct scm *
variable_set_x (struct scm *var, struct scm *value)
{
  assert_variable (1, var);
  var->variable = value;
  return cell_unspecified;
}

struct scm *
variable_bound_p (struct scm *var)
{
  assert_variable (1, var);
  struct scm *value = var->variable;
  if (value != cell_undefined)
    return cell_t;
  return cell_f;
}

struct scm *
lookup_variable (struct scm *lookup, struct scm *name, struct scm *define_p)
{
  struct scm *handle = cell_f;
  if (lookup != cell_f)
    {
      if (lookup->type = TPAIR)
        handle = assq (name, lookup);
      // else
      //   return apply (lookup, cons (name, cons (define_p, cell_nil)), cell_unspecified);
    }

  if (handle == cell_f)
    {
      handle = hashq_get_handle_ (M0, name, cell_f);
      if (handle == cell_f && define_p == cell_t)
        {
          if (g_debug > 0)
            {
              eputs ("lookup + define: ");
              write_error_ (name);
              eputs ("\n");
            }
          handle = hashq_set_handle_x (M0, name, cell_f);
        }
    }

  return handle;
}

struct scm *
lookup_variable_ (struct scm *lookup, char const* name)
{
  return lookup_variable (lookup, cstring_to_symbol (name), cell_f);
}

struct scm *
lookup_ref (struct scm *lookup, struct scm *name)
{
  struct scm *x = lookup_variable (lookup, name, cell_f);
  if (x == cell_f)
    error (cell_symbol_unbound_variable, name);
  return x->cdr;
}
