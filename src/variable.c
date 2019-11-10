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
