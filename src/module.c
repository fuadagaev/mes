/* -*-comment-start: "//";comment-end:""-*-
 * GNU Mes --- Maxwell Equations of Software
 * Copyright © 2018,2019 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
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
make_initial_module (struct scm *a)     /*:((internal)) */
{
  struct scm *module = make_hash_table_ (100);
  while (a->type == TPAIR)
    {
      hashq_set_x (module, a->car->car, a->car->cdr);
      a = a->cdr;
    }
  return module;
}

struct scm *
initial_module ()
{
  return M0;
}

struct scm *
module_define_x (struct scm *module, struct scm *name, struct scm *value)
{
  return hashq_set_x (M0, name, value);
}

struct scm *
scm_module_lookup_closure (struct scm *module)
{
  if (module == cell_f)
    return cell_f;
  else
    return struct_ref (module, MODULE_EVAL_CLOSURE);
}

struct scm *
scm_current_module_lookup_closure ()
{
  if (scm_module_system_booted_p)
    return scm_module_lookup_closure (scm_current_module ());
  return cell_f;
}

struct scm *
scm_eval_closure_lookup (struct scm *eclo, struct scm *name, struct scm *define_p)
{
  struct scm *module = eclo;
  if (define_p == cell_f)
    return module_variable (module, name);
  else
    {
#if 0
      if (struct scm *_EVAL_CLOSURE_INTERFACE_P (eclo))
        return struct cell_f;
#endif
      return apply (module_make_local_var_x_var, cons (module, cons (name, cell_nil)));
    }
}

struct scm *
module_variable (struct scm *module, struct scm *name)
{
  /* 1. Check module obarray */
  struct scm *a = struct_ref_ (module, MODULE_OBARRAY);
  struct scm *b = scm_hashq_ref (a, name, cell_f);
  if (b != cell_f)
    return b;

  /* 2. Custom binder */
  struct scm *binder = struct_ref (module, MODULE_BINDER);
  if (binder != cell_f)
    {
      b = apply (binder->cdr, (cons (module, cons (name, cons (cell_f, cell_nil)))), cell_f);
      if (b != cell_f)
        return b;
    }

  /* 3. Search the use list */
  struct scm *uses = struct_ref (module, MODULE_USES);
  while (uses->type == TPAIR)
    {
      b = module_variable (uses->car, name);
      if (b != cell_f)
        return b;
      uses = uses->cdr;
    }

  return cell_f;
}
