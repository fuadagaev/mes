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
current_module ()     /*:((internal)) */
{
  struct scm *module = hashq_ref_ (M0, cstring_to_symbol ("*current-module*"), cell_f);
  if (module != cell_f)
    return module;
  return M0;
}

struct scm *
module_defines (struct scm *module)     /*:((internal)) */
{
  if (module != cell_f && module != M0)
    return struct_ref_ (module, MODULE_DEFINES);
  return M0;
}

struct scm *
module_define_x (struct scm *module, struct scm *name, struct scm *value)
{
  struct scm *table = module_defines (module);
  return hashq_set_x (table, name, value);
}

struct scm *
module_handle (struct scm *module, struct scm *name)     /*:((internal)) */
{
  /* 1. Check module defines.  */
  struct scm *table = module_defines (module);
  if (g_debug > 2)
    {
      eputs ("module_handle:");
      eputs (" name = ");
      write_error_ (name);
      // eputs (" defines = ");
      // write_error_ (table);
      eputs ("\n");
    }

  struct scm *handle = hashq_get_handle_ (table, name);
  if (handle != cell_f)
    return handle;

  /* 2. Custom binder.  */
  /*
  struct scm *binder = struct_ref (module, MODULE_BINDER);
  if (binder != cell_f)
    {
      b = apply (binder->cdr, (cons (module, cons (name, cons (cell_f, cell_nil)))), cell_f);
      if (b != cell_f)
        return b;
    }
  */

  /* 3. Search the use list.  */
  struct scm *uses = struct_ref_ (module, MODULE_USES);
  while (uses->type == TPAIR)
    {
      handle = module_handle (uses->car, name);
      if (handle != cell_f)
        return handle;
      uses = uses->cdr;
    }

  /* 4. Hack for Mes: always look in M0. */
  handle = hashq_get_handle_ (M0, name);

  return handle;
}

/* NOT USED? */
struct scm *
module_variable (struct scm *module, struct scm *name)
{
  struct scm *handle = module_handle (module, name);
  if (handle != cell_f)
    return handle->cdr;
  return cell_f;
}
