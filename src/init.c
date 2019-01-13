/* -*-comment-start: "//";comment-end:""-*-
 * GNU Mes --- Maxwell Equations of Software
 * Copyright © 2016,2017,2018 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
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
#include "mes/mes.h"
void
initialize_constants ()
{
  cell_nil = malloc (sizeof (struct scm));
  cell_f = malloc (sizeof (struct scm));
  cell_t = malloc (sizeof (struct scm));
  cell_dot = malloc (sizeof (struct scm));
  cell_arrow = malloc (sizeof (struct scm));
  cell_undefined = malloc (sizeof (struct scm));
  cell_unspecified = malloc (sizeof (struct scm));
  cell_closure = malloc (sizeof (struct scm));
  cell_circular = malloc (sizeof (struct scm));
  cell_begin = malloc (sizeof (struct scm));
  cell_call_with_current_continuation = malloc (sizeof (struct scm));
  cell_vm_apply = malloc (sizeof (struct scm));
  cell_vm_apply2 = malloc (sizeof (struct scm));
  cell_vm_begin = malloc (sizeof (struct scm));
  cell_vm_begin_eval = malloc (sizeof (struct scm));
  cell_vm_begin_expand = malloc (sizeof (struct scm));
  cell_vm_begin_expand_eval = malloc (sizeof (struct scm));
  cell_vm_begin_expand_macro = malloc (sizeof (struct scm));
  cell_vm_begin_expand_primitive_load = malloc (sizeof (struct scm));
  cell_vm_begin_primitive_load = malloc (sizeof (struct scm));
  cell_vm_begin_read_input_file = malloc (sizeof (struct scm));
  cell_vm_call_with_current_continuation2 = malloc (sizeof (struct scm));
  cell_vm_call_with_values2 = malloc (sizeof (struct scm));
  cell_vm_eval = malloc (sizeof (struct scm));
  cell_vm_eval2 = malloc (sizeof (struct scm));
  cell_vm_eval_check_func = malloc (sizeof (struct scm));
  cell_vm_eval_define = malloc (sizeof (struct scm));
  cell_vm_eval_macro_expand_eval = malloc (sizeof (struct scm));
  cell_vm_eval_macro_expand_expand = malloc (sizeof (struct scm));
  cell_vm_eval_pmatch_car = malloc (sizeof (struct scm));
  cell_vm_eval_pmatch_cdr = malloc (sizeof (struct scm));
  cell_vm_eval_set_x = malloc (sizeof (struct scm));
  cell_vm_evlis = malloc (sizeof (struct scm));
  cell_vm_evlis2 = malloc (sizeof (struct scm));
  cell_vm_evlis3 = malloc (sizeof (struct scm));
  cell_vm_if = malloc (sizeof (struct scm));
  cell_vm_if_expr = malloc (sizeof (struct scm));
  cell_vm_macro_expand = malloc (sizeof (struct scm));
  cell_vm_macro_expand_car = malloc (sizeof (struct scm));
  cell_vm_macro_expand_cdr = malloc (sizeof (struct scm));
  cell_vm_macro_expand_define = malloc (sizeof (struct scm));
  cell_vm_macro_expand_define_macro = malloc (sizeof (struct scm));
  cell_vm_macro_expand_lambda = malloc (sizeof (struct scm));
  cell_vm_macro_expand_set_x = malloc (sizeof (struct scm));
  cell_vm_return = malloc (sizeof (struct scm));
  cell_symbol_dot = malloc (sizeof (struct scm));
  cell_symbol_lambda = malloc (sizeof (struct scm));
  cell_symbol_begin = malloc (sizeof (struct scm));
  cell_symbol_if = malloc (sizeof (struct scm));
  cell_symbol_quote = malloc (sizeof (struct scm));
  cell_symbol_define = malloc (sizeof (struct scm));
  cell_symbol_define_macro = malloc (sizeof (struct scm));
  cell_symbol_quasiquote = malloc (sizeof (struct scm));
  cell_symbol_unquote = malloc (sizeof (struct scm));
  cell_symbol_unquote_splicing = malloc (sizeof (struct scm));
  cell_symbol_syntax = malloc (sizeof (struct scm));
  cell_symbol_quasisyntax = malloc (sizeof (struct scm));
  cell_symbol_unsyntax = malloc (sizeof (struct scm));
  cell_symbol_unsyntax_splicing = malloc (sizeof (struct scm));
  cell_symbol_set_x = malloc (sizeof (struct scm));
  cell_symbol_sc_expand = malloc (sizeof (struct scm));
  cell_symbol_macro_expand = malloc (sizeof (struct scm));
  cell_symbol_portable_macro_expand = malloc (sizeof (struct scm));
  cell_symbol_sc_expander_alist = malloc (sizeof (struct scm));
  cell_symbol_call_with_values = malloc (sizeof (struct scm));
  cell_symbol_call_with_current_continuation = malloc (sizeof (struct scm));
  cell_symbol_boot_module = malloc (sizeof (struct scm));
  cell_symbol_current_module = malloc (sizeof (struct scm));
  cell_symbol_primitive_load = malloc (sizeof (struct scm));
  cell_symbol_read_input_file = malloc (sizeof (struct scm));
  cell_symbol_write = malloc (sizeof (struct scm));
  cell_symbol_display = malloc (sizeof (struct scm));
  cell_symbol_car = malloc (sizeof (struct scm));
  cell_symbol_cdr = malloc (sizeof (struct scm));
  cell_symbol_not_a_number = malloc (sizeof (struct scm));
  cell_symbol_not_a_pair = malloc (sizeof (struct scm));
  cell_symbol_system_error = malloc (sizeof (struct scm));
  cell_symbol_throw = malloc (sizeof (struct scm));
  cell_symbol_unbound_variable = malloc (sizeof (struct scm));
  cell_symbol_wrong_number_of_args = malloc (sizeof (struct scm));
  cell_symbol_wrong_type_arg = malloc (sizeof (struct scm));
  cell_symbol_buckets = malloc (sizeof (struct scm));
  cell_symbol_builtin = malloc (sizeof (struct scm));
  cell_symbol_frame = malloc (sizeof (struct scm));
  cell_symbol_hashq_table = malloc (sizeof (struct scm));
  cell_symbol_module = malloc (sizeof (struct scm));
  cell_symbol_procedure = malloc (sizeof (struct scm));
  cell_symbol_record_type = malloc (sizeof (struct scm));
  cell_symbol_size = malloc (sizeof (struct scm));
  cell_symbol_stack = malloc (sizeof (struct scm));
  cell_symbol_argv = malloc (sizeof (struct scm));
  cell_symbol_mes_datadir = malloc (sizeof (struct scm));
  cell_symbol_mes_version = malloc (sizeof (struct scm));
  cell_symbol_internal_time_units_per_second = malloc (sizeof (struct scm));
  cell_symbol_compiler = malloc (sizeof (struct scm));
  cell_symbol_arch = malloc (sizeof (struct scm));
  cell_symbol_pmatch_car = malloc (sizeof (struct scm));
  cell_symbol_pmatch_cdr = malloc (sizeof (struct scm));
  cell_type_bytes = malloc (sizeof (struct scm));
  cell_type_char = malloc (sizeof (struct scm));
  cell_type_closure = malloc (sizeof (struct scm));
  cell_type_continuation = malloc (sizeof (struct scm));
  cell_type_function = malloc (sizeof (struct scm));
  cell_type_keyword = malloc (sizeof (struct scm));
  cell_type_macro = malloc (sizeof (struct scm));
  cell_type_number = malloc (sizeof (struct scm));
  cell_type_pair = malloc (sizeof (struct scm));
  cell_type_port = malloc (sizeof (struct scm));
  cell_type_ref = malloc (sizeof (struct scm));
  cell_type_special = malloc (sizeof (struct scm));
  cell_type_string = malloc (sizeof (struct scm));
  cell_type_struct = malloc (sizeof (struct scm));
  cell_type_symbol = malloc (sizeof (struct scm));
  cell_type_values = malloc (sizeof (struct scm));
  cell_type_variable = malloc (sizeof (struct scm));
  cell_type_vector = malloc (sizeof (struct scm));
  cell_type_broken_heart = malloc (sizeof (struct scm));
  cell_symbol_test = malloc (sizeof (struct scm));
  cell_test = malloc (sizeof (struct scm));
}
