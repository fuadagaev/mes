/* -*-comment-start: "//";comment-end:""-*-
 * GNU Mes --- Maxwell Equations of Software
 * Copyright © 2018 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
 * Copyright © 2019 Jeremiah Orians <jeremiah@pdp10.guru>
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
make_module_type ()
{
  struct scm *record_type = cell_symbol_record_type;    /* FIXME */
  struct scm *fields = cell_nil;
  fields = cons (cstring_to_symbol ("globals"), fields);
  fields = cons (cstring_to_symbol ("locals"), fields);
  fields = cons (cstring_to_symbol ("name"), fields);
  fields = cons (fields, cell_nil);
  fields = cons (cell_symbol_module, fields);
  return make_struct (record_type, fields, cell_unspecified);
}

struct scm *
module_variable (struct scm *module, struct scm *name)
{
  /* struct scm* locals = struct_ref_ (module, 3); */
  struct scm *locals = module;
  struct scm *x = assq (name, locals);

  if (x == cell_f)
    {
      struct scm *globals = struct_ref_ (M0, 5);
      x = hashq_get_handle (globals, name, cell_f);
    }

  return x;
}


struct scm *
module_ref (struct scm *module, struct scm *name)
{
  struct scm *y = module_variable (module, name);

  if (y == cell_f)
    {
      return cell_undefined;
    }

  return y->cdr;
}

struct scm *
module_define_x (struct scm *module, struct scm *name, struct scm *value)
{
  struct scm *globals = struct_ref_ (M0, 5);
  return hashq_set_x (globals, name, value);
}

/* External functions */
struct scm *
module_variable_ (struct scm *module, struct scm *name) /* EXTERNAL */
{
  return module_variable (module, name);
}

struct scm *
module_ref_ (struct scm *module, struct scm *name)      /* EXTERNAL */
{
  return module_ref (module, name);
}

struct scm *
make_module_type_ ()            /* EXTERNAL */
{
  return make_module_type ();
}
