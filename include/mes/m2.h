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

#ifndef __MES_M2_H
#define __MES_M2_H

#define __FILEDES_MAX 512

SCM length__ (struct scm *x);
char *char_lookup (int c, int type);
char *env_lookup (char *token, char **envp);
char *itoa (int number);
char *ltoa (SCM number);
char *ltoab (SCM x, int base);
char *ntoab (SCM number, int base, int signed_p);
char *search_path (char *file_name);
int __mes_debug ();
int __ungetc_p (int filedes);
int _fdungetc_p (int fd);
int _open2 (char *file_name, int flags);
int _open3 (char *file_name, int flags, int mask);
int eputc (int c);
int fdgetc (int fd);
int fdputc (int c, int fd);
int fdputs (char *s, int fd);
int fdungetc (int c, int fd);
int get_env_value (char *c, int alt);
int in_set (int c, char *s);
int isdigit (int c);
int isspace (int c);
int isxdigit (int c);
int match (char *a, char *b);
int mes_open (char *file_name, int flags, int mask);
int numerate_string (char *a);
int oputc (int c);
int peekchar ();
int readchar ();
int string_len (char *a);
int unreadchar ();
struct scm *acons (struct scm *key, struct scm *value, struct scm *alist);
struct scm *append2 (struct scm *x, struct scm *y);
struct scm *apply (struct scm *f, struct scm *x);
struct scm *apply_builtin (struct scm *fn, struct scm *x);
struct scm *assert_defined (struct scm *x, struct scm *e);
struct scm *assoc (struct scm *x, struct scm *a);
struct scm *assq (struct scm *x, struct scm *a);
struct scm *builtin_arity (struct scm *builtin);
struct scm *builtin_p (struct scm *x);
struct scm *call_lambda (struct scm *e, struct scm *x);
struct scm *cdr (struct scm *x);
struct scm *check_apply (struct scm *f, struct scm *e);
struct scm *check_formals (struct scm *f, struct scm *formals, struct scm *args);
struct scm *cons (struct scm *x, struct scm *y);
struct scm *cstring_to_symbol (char *s);
struct scm *current_input_port ();
struct scm *display_ (struct scm *x);
struct scm *display_error_ (struct scm *x);
struct scm *eq_p (struct scm *x, struct scm *y);
struct scm *equal2_p (struct scm *a, struct scm *b);
struct scm *error (struct scm *key, struct scm *x);
struct scm *eval_apply ();
struct scm *expand_variable (struct scm *x, struct scm *formals);
struct scm *fdisplay_ (struct scm *, int, int);
struct scm *gc ();
struct scm *gc_check ();
struct scm *gc_pop_frame ();
struct scm *get_macro (struct scm *name);
struct scm *hash_ref (struct scm *table, struct scm *key, struct scm *dflt);
struct scm *hash_set_x (struct scm *table, struct scm *key, struct scm *value);
struct scm *hashq_get_handle (struct scm *table, struct scm *key, struct scm *dflt);
struct scm *hashq_set_x (struct scm *table, struct scm *key, struct scm *value);
struct scm *init_time (struct scm *a);
struct scm *list_to_vector (struct scm *x);
struct scm *macro_get_handle (struct scm *name);
struct scm *macro_set_x (struct scm *name, struct scm *value);
struct scm *make_bytes (char *s, SCM length);
struct scm *make_char (SCM c);
struct scm *make_closure_ (struct scm *args, struct scm *body, struct scm *a);
struct scm *make_frame_type ();
struct scm *make_hash_table_ (SCM size);
struct scm *make_hashq_type ();
struct scm *make_keyword (struct scm *a, struct scm *b);
struct scm *make_module_type ();
struct scm *make_number (SCM n);
struct scm *make_port (SCM n, struct scm *s);
struct scm *make_stack_type ();
struct scm *make_string (char *s, int length);
struct scm *make_string_ (char *s);
struct scm *make_struct (struct scm *type, struct scm *fields, struct scm *printer);
struct scm *make_tcontinuation (SCM a, SCM b);
struct scm *make_tmacro (struct scm *a, struct scm *b);
struct scm *make_tpair (struct scm *a, struct scm *b);
struct scm *make_tref (struct scm *x);
struct scm *make_tstring1 (SCM n);
struct scm *make_tstring2 (struct scm *a, struct scm *b);
struct scm *make_tsymbol (struct scm *a, struct scm *b);
struct scm *make_variable_ (struct scm *var);
struct scm *make_vector__ (SCM k);
struct scm *mes_builtins (struct scm *a);
struct scm *mes_g_stack (struct scm *a);
struct scm *mes_symbols ();
struct scm *module_define_x (struct scm *module, struct scm *name, struct scm *value);
struct scm *module_printer (struct scm *module);
struct scm *module_ref (struct scm *module, struct scm *name);
struct scm *module_variable (struct scm *module, struct scm *name);
struct scm *open_input_file (struct scm *file_name);
struct scm *pairlis (struct scm *x, struct scm *y, struct scm *a);
struct scm *push_cc (struct scm *p1, struct scm *p2, struct scm *a, struct scm *c);
struct scm *read_env (struct scm *a);
struct scm *read_input_file_env ();
struct scm *reverse_x_ (struct scm *x, struct scm *t);
struct scm *set_cdr_x (struct scm *x, struct scm *e);
struct scm *set_current_input_port (struct scm *port);
struct scm *set_env_x (struct scm *x, struct scm *e, struct scm *a);
struct scm *string_equal_p (struct scm *a, struct scm *b);
struct scm *struct_ref_ (struct scm *x, SCM i);
struct scm *symbol_to_keyword (struct scm *symbol);
struct scm *vector_entry (struct scm *x);
struct scm *vector_equal_p (struct scm *a, struct scm *b);
struct scm *vector_length (struct scm *x);
struct scm *vector_ref_ (struct scm *table, long i);
struct scm *vector_ref_ (struct scm *x, SCM i);
struct scm *write_error_ (struct scm *x);
void __ungetc_clear (int filedes);
void __ungetc_init ();
void __ungetc_set (int filedes, int c);
void assert_max_string (int i, char *msg, char *string);
void block_copy (void *source, void *destination, int num);
void gc_init_cells ();
void gc_push_frame ();
void initialize_constants ();
void initialize_memory ();
void raw_print (char *s, int fd);
void require (int bool, char *error);
void vector_set_x_ (struct scm *x, SCM i, struct scm *e);

#if __M2_PLANET__
char *MES_VERSION = "git";
char *MES_PKGDATADIR = "/usr/local/share/mes";

SCM strlen (char *s);
SCM write (int filedes, void *buffer, SCM size);
int atoi (char *s);
int eputs (char *s);
int fdputs (char s, int fd);
int open (char *file_name, int flags, int mask);
int oputs (char s);
#endif

#endif //__MES_M2_H
