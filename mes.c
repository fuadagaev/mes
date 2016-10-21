/* -*-comment-start: "//";comment-end:""-*-
 * Mes --- Maxwell Equations of Software
 * Copyright © 2016 Jan Nieuwenhuizen <janneke@gnu.org>
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

#define STRING_MAX 2048
#define _GNU_SOURCE
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define DEBUG 0
#define BUILTIN_QUASIQUOTE 1 // 6x speedup for mescc
#define MES_FULL 1

enum type {CHAR, MACRO, NUMBER, PAIR, SCM, STRING, SYMBOL, VALUES, VECTOR,
           FUNCTION0, FUNCTION1, FUNCTION2, FUNCTION3, FUNCTIONn};
struct scm_t;
typedef struct scm_t* (*function0_t) (void);
typedef struct scm_t* (*function1_t) (struct scm_t*);
typedef struct scm_t* (*function2_t) (struct scm_t*, struct scm_t*);
typedef struct scm_t* (*function3_t) (struct scm_t*, struct scm_t*, struct scm_t*);
typedef struct scm_t* (*functionn_t) (struct scm_t*);

typedef struct scm_t {
  enum type type;
  union {
    char const *name;
    struct scm_t* car;
    int length;
  };
  union {
    int value;
    function0_t function0;
    function1_t function1;
    function2_t function2;
    function3_t function3;
    functionn_t functionn;
    struct scm_t* cdr;
    struct scm_t* macro;
    struct scm_t** vector;
  };
} scm;

scm temp_number = {NUMBER, .name="nul", .value=0};

#define MES_C 1
#include "mes.h"

scm *display_ (FILE* f, scm *x); //internal
scm *display_helper (FILE*, scm*, bool, char const*, bool);

scm scm_nil = {SCM, "()"};
scm scm_dot = {SCM, "."};
scm scm_f = {SCM, "#f"};
scm scm_t = {SCM, "#t"};
scm scm_undefined = {SCM, "*undefined*"};
scm scm_unspecified = {SCM, "*unspecified*"};
scm scm_closure = {SCM, "*closure*"};
scm scm_circular = {SCM, "*circular*"};
#if BOOT
scm scm_label = {
  SCM, "label"};
#endif
scm scm_lambda = {SCM, "lambda"};

scm symbol_begin = {SCM, "begin"};
scm symbol_if = {SCM, "if"};
scm symbol_define = {SCM, "define"};
scm symbol_define_macro = {SCM, "define-macro"};
scm symbol_set_x = {SCM, "set!"};

scm symbol_quote = {SYMBOL, "quote"};
scm symbol_quasiquote = {SYMBOL, "quasiquote"};
scm symbol_unquote = {SYMBOL, "unquote"};
scm symbol_unquote_splicing = {SYMBOL, "unquote-splicing"};

scm symbol_sc_expand = {SYMBOL, "sc-expand"};
scm symbol_syntax = {SYMBOL, "syntax"};
scm symbol_quasisyntax = {SYMBOL, "quasisyntax"};
scm symbol_unsyntax = {SYMBOL, "unsyntax"};
scm symbol_unsyntax_splicing = {SYMBOL, "unsyntax-splicing"};

scm symbol_call_with_values = {SYMBOL, "call-with-values"};
scm symbol_current_module = {SYMBOL, "current-module"};


scm char_nul = {CHAR, .name="nul", .value=0};
scm char_backspace = {CHAR, .name="backspace", .value=8};
scm char_tab = {CHAR, .name="tab", .value=9};
scm char_newline = {CHAR, .name="newline", .value=10};
scm char_vt = {CHAR, .name="vt", .value=11};
scm char_page = {CHAR, .name="page", .value=12};
scm char_return = {CHAR, .name="return", .value=13};
scm char_space = {CHAR, .name="space", .value=32};

// PRIMITIVES

#define ATOM_P(x) (x->type == PAIR ? &scm_f : &scm_t)
scm *
atom_p (scm *x)
{
  return ATOM_P(x);
}

scm *
car (scm *x)
{
  assert (x->type == PAIR);
  return x->car;
}

scm *
cdr (scm *x)
{
  assert (x->type == PAIR);
  return x->cdr;
}

scm *
cons (scm *x, scm *y)
{
  scm *p = (scm*)malloc (sizeof (scm));
  p->type = PAIR;
  p->car = x;
  p->cdr = y;
  return p;
}

#define EQ_P(x, y)\
  ((x == y                                              \
    || (x->type == CHAR && y->type == CHAR              \
        && x->value == y->value)                        \
    || (x->type == NUMBER && y->type == NUMBER          \
        && x->value == y->value))                       \
   ? &scm_t : &scm_f)

scm *
eq_p (scm *x, scm *y)
{
  return EQ_P (x, y);
}

scm *
macro_p (scm *x)
{
  return x->type == MACRO ? &scm_t : &scm_f;
}

scm *
null_p (scm *x)
{
  return x == &scm_nil ? &scm_t : &scm_f;
}

#define PAIR_P(x) (x->type == PAIR ? &scm_t : &scm_f)
scm *
pair_p (scm *x)
{
  return PAIR_P(x);
}

scm *
set_car_x (scm *x, scm *e)
{
  assert (x->type == PAIR);
  x->car = e;
  return &scm_unspecified;
}

scm *
set_cdr_x (scm *x, scm *e)
{
  assert (x->type == PAIR);
  cache_invalidate (x->cdr);
  x->cdr = e;
  return &scm_unspecified;
}

scm *
set_env_x (scm *x, scm *e, scm *a)
{
  cache_invalidate (x);
  return set_cdr_x (assq (x, a), e);
}

scm *
quote (scm *x)
{
  return cons (&symbol_quote, x);
}

scm *
quasiquote (scm *x)
{
  return cons (&symbol_quasiquote, x);
}

#if BUILTIN_QUASIQUOTE
scm *
unquote (scm *x) //int must not add to environment
{
  return cons (&symbol_unquote, x);
}
scm *unquote (scm *x);
scm scm_unquote = {FUNCTION1, .name="unquote", .function1=&unquote};

scm *
unquote_splicing (scm *x) //int must not add to environment
{
  return cons (&symbol_unquote_splicing, x);
}
scm *unquote_splicing (scm *x);
scm scm_unquote_splicing = {FUNCTION1, .name="unquote-splicing", .function1=&unquote_splicing};
#endif // BUILTIN_QUASIQUOTE
scm *
syntax (scm *x)
{
  return cons (&symbol_syntax, x);
}

scm *
quasisyntax (scm *x)
{
  return cons (&symbol_quasisyntax, x);
}

scm *
unsyntax (scm *x) //int must not add to environment
{
  return cons (&symbol_unsyntax, x);
}
scm *unsyntax (scm *x);
scm scm_unsyntax = {FUNCTION1, .name="unsyntax", .function1=&unsyntax};

scm *
unsyntax_splicing (scm *x) //int must not add to environment
{
  return cons (&symbol_unsyntax_splicing, x);
}
scm *unsyntax_splicing (scm *x);
scm scm_unsyntax_splicing = {FUNCTION1, .name="unsyntax-splicing", .function1=&unsyntax_splicing};


//Library functions

// Derived, non-primitives
scm *caar (scm *x) {return car (car (x));}
scm *cadr (scm *x) {return car (cdr (x));}
scm *cdar (scm *x) {return cdr (car (x));}
scm *cddr (scm *x) {return cdr (cdr (x));}
scm *caaar (scm *x) {return car (car (car (x)));}
scm *caadr (scm *x) {return car (car (cdr (x)));}
scm *caddr (scm *x) {return car (cdr (cdr (x)));}
scm *cdadr (scm *x) {return cdr (car (cdr (x)));}
scm *cadar (scm *x) {return car (cdr (car (x)));}
scm *cddar (scm *x) {return cdr (cdr (car (x)));}
scm *cdddr (scm *x) {return cdr (cdr (cdr (x)));}

scm *
pairlis (scm *x, scm *y, scm *a)
{
  if (x == &scm_nil)
    return a;
  if (atom_p (x) == &scm_t)
    return cons (cons (x, y), a);
  return cons (cons (car (x), car (y)),
               pairlis (cdr (x), cdr (y), a));
}

scm *
assq (scm *x, scm *a)
{
  while (a != &scm_nil && EQ_P (x, a->car->car) == &scm_f) a = a->cdr;
  return a != &scm_nil ? a->car : &scm_f;
}

#define ENV_CACHE 1
#define CACHE_SIZE 30
#define ENV_HEAD 15

#if !ENV_CACHE
scm *
assq_ref_cache (scm *x, scm *a) //internal
{
  x = assq (x, a);
  if (x == &scm_f) return &scm_f;
  return x->cdr;
}
scm*cache_invalidate (scm*x){}
scm*cache_invalidate_range (scm*p,scm*a){}
scm*cache_save (scm*p){}
scm*cache_lookup (scm*x){}

#else // ENV_CACHE

scm *env_cache_cars[CACHE_SIZE];
scm *env_cache_cdrs[CACHE_SIZE];
int cache_threshold = 0;
scm *
cache_save (scm *p)
{
  int n = p->car->value;
  if (n < cache_threshold) return &scm_unspecified;
  int j = -1;
  for (int i=0; i < CACHE_SIZE; i++) {
    if (!env_cache_cars[i]) {
      j = i;
      break;
    }
    if (env_cache_cars[i] == p->car) return &scm_unspecified;
    if (n > env_cache_cars[i]->value) {
      n = env_cache_cars[i]->value;
      j = i;
    }
  }
  if (j >= 0) {
    cache_threshold = p->car->value;
    env_cache_cars[j] = p->car;
    env_cache_cdrs[j] = p->cdr;
  }
  return &scm_unspecified;
}

scm *
cache_lookup (scm *x)
{
  for (int i=0; i < CACHE_SIZE; i++) {
    if (!env_cache_cars[i]) break;
    if (env_cache_cars[i] == x) return env_cache_cdrs[i];
  }
  return &scm_undefined;
}

scm *
cache_invalidate (scm *x)
{
  for (int i=0; i < CACHE_SIZE; i++) {
    if (env_cache_cars[i] == x) {
      env_cache_cars[i] = 0;
      break;
    }
  }
  return &scm_unspecified;
}

scm *
cache_invalidate_range (scm *p, scm *a)
{
  do {
    cache_invalidate (p->car->car);
    p = p->cdr;
  } while (p != a);
  return &scm_unspecified;
}

scm *
assq_ref_cache (scm *x, scm *a)
{
  x->value++;
  scm *c = cache_lookup (x);
  if (c != &scm_undefined) return c;
  int i = 0;
  while (a != &scm_nil && x != a->car->car) {i++;a = a->cdr;}
  if (a == &scm_nil) return &scm_undefined;
  if (i>ENV_HEAD) cache_save (a->car);
  return a->car->cdr;
}
#endif // ENV_CACHE

scm *
evlis_env (scm *m, scm *a)
{
  if (m == &scm_nil) return &scm_nil;
  if (m->type != PAIR) return builtin_eval (m, a);
  scm *e = builtin_eval (car (m), a);
  return cons (e, evlis_env (cdr (m), a));
}

scm *
apply_env (scm *fn, scm *x, scm *a)
{
  if (fn->type != PAIR)
    {
      if (fn == &scm_car) return x->car->car;
      if (fn == &scm_cdr) return x->car->cdr;
      if (builtin_p (fn) == &scm_t)
        return call (fn, x);
      if (eq_p (fn, &symbol_call_with_values) == &scm_t)
        return call (&scm_call_with_values_env, append2 (x, cons (a, &scm_nil)));
      if (fn == &symbol_current_module) return a;
    }
  else if (fn->car == &scm_lambda) {
    scm *p = pairlis (cadr (fn), x, a);
    cache_invalidate_range (p, a->cdr);
    scm *r = builtin_eval (cons (&symbol_begin, cddr (fn)), cons (cons (&scm_closure, p), p));
    cache_invalidate_range (p, a->cdr);
    return r;
  }
  else if (fn->car == &scm_closure) {
    scm *args = caddr (fn);
    scm *body = cdddr (fn);
    a = cdadr (fn);
    a = cdr (a);
    scm *p = pairlis (args, x, a);
    cache_invalidate_range (p, a->cdr);
    scm *r = builtin_eval (cons (&symbol_begin, body), cons (cons (&scm_closure, p), p));
    cache_invalidate_range (p, a->cdr);
    return r;
  }
#if BOOT
  else if (fn->car == &scm_label)
    return apply_env (caddr (fn), x, cons (cons (cadr (fn), caddr (fn)), a));
#endif
  scm *efn = builtin_eval (fn, a);
  if (efn == &scm_f || efn == &scm_t) assert (!"apply bool");
  if (efn->type == NUMBER) assert (!"apply number");
  if (efn->type == STRING) assert (!"apply string");
  return apply_env (efn, x, a);
}

scm *
builtin_eval (scm *e, scm *a)
{
  if (builtin_p (e) == &scm_t) return e;
  if (internal_p (e) == &scm_t) return e;

  e = expand_macro_env (e, a);

  if (e->type == SYMBOL) {
    scm *y = assq_ref_cache (e, a);
    if (y == &scm_undefined) {
      fprintf (stderr, "eval: unbound variable: %s\n", e->name);
      assert (!"unbound variable");
    }
    return y;
  }
  else if (e->type != PAIR)
    return e;
  else if (e->car->type != PAIR)
    {
      if (e->car == &symbol_quote)
        return cadr (e);
      if (e->car == &symbol_syntax)
        return e;
      if (e->car == &symbol_begin)
        return begin (e, a);
      if (e->car == &scm_lambda)
        return make_closure (cadr (e), cddr (e), assq (&scm_closure, a));
      if (e->car == &scm_closure)
        return e;
      if (e->car == &symbol_if)
        return builtin_if (cdr (e), a);
#if !BOOT
      if (e->car == &symbol_define)
        return define (e, a);
      if (e->car == &symbol_define_macro)
        return define (e, a);
#else
      if (e->car == &symbol_define) {
        fprintf (stderr, "C DEFINE: %s\n", e->cdr->car->type == SYMBOL
                 ? e->cdr->car->name
                 : e->cdr->car->car->name);
      }
      assert (e->car != &symbol_define);
      assert (e->car != &symbol_define_macro);
#endif
      if (e->car == &symbol_set_x)
        return set_env_x (cadr (e), builtin_eval (caddr (e), a), a);
#if BUILTIN_QUASIQUOTE
      if (e->car == &symbol_unquote)
        return builtin_eval (cadr (e), a);
      if (e->car == &symbol_quasiquote)
        return eval_quasiquote (cadr (e), add_unquoters (a));
      if (e->car == &symbol_unsyntax)
        return builtin_eval (cadr (e), a);
      if (e->car == &symbol_quasisyntax)
        return eval_quasisyntax (cadr (e), add_unsyntaxers (a));
#endif //BUILTIN_QUASIQUOTE
    }
  return apply_env (e->car, evlis_env (e->cdr, a), a);
}

scm *
expand_macro_env (scm *e, scm *a)
{
  scm *macro;
  if (e->type == PAIR
      && (macro = lookup_macro (e->car, a)) != &scm_f)
    return expand_macro_env (apply_env (macro, e->cdr, a), a);
  return e;
}

scm *
begin (scm *e, scm *a)
{
  scm *r = &scm_unspecified;
  while (e != &scm_nil) {
    r = builtin_eval (e->car, a);
    e = e->cdr;
  }
  return r;
}

scm *
builtin_if (scm *e, scm *a)
{
  if (builtin_eval (car (e), a) != &scm_f)
    return builtin_eval (cadr (e), a);
  if (cddr (e) != &scm_nil)
    return builtin_eval (caddr (e), a);
  return &scm_unspecified;
}

#if BUILTIN_QUASIQUOTE
scm *
eval_quasiquote (scm *e, scm *a)
{
  if (e == &scm_nil) return e;
  else if (atom_p (e) == &scm_t) return e;
  else if (eq_p (car (e), &symbol_unquote) == &scm_t)
    return builtin_eval (cadr (e), a);
  else if (e->type == PAIR && e->car->type == PAIR
           && eq_p (caar (e), &symbol_unquote_splicing) == &scm_t)
      return append2 (builtin_eval (cadar (e), a), eval_quasiquote (cdr (e), a));
  return cons (eval_quasiquote (car (e), a), eval_quasiquote (cdr (e), a));
}

scm *
eval_quasisyntax (scm *e, scm *a)
{
  if (e == &scm_nil) return e;
  else if (atom_p (e) == &scm_t) return e;
  else if (eq_p (car (e), &symbol_unsyntax) == &scm_t)
    return builtin_eval (cadr (e), a);
  else if (e->type == PAIR && e->car->type == PAIR
           && eq_p (caar (e), &symbol_unsyntax_splicing) == &scm_t)
      return append2 (builtin_eval (cadar (e), a), eval_quasisyntax (cdr (e), a));
  return cons (eval_quasisyntax (car (e), a), eval_quasisyntax (cdr (e), a));
}

#else
scm*add_unquoters (scm *a){}
scm*add_unsyntaxers (scm *a){}
scm*eval_unsyntax (scm *e, scm *a){}
scm*eval_quasiquote (scm *e, scm *a){}
scm*eval_quasisyntax (scm *e, scm *a){}
#endif // BUILTIN_QUASIQUOTE

//Helpers

scm *
builtin_p (scm *x)
{
  return (x->type == FUNCTION0
          || x->type == FUNCTION1
          || x->type == FUNCTION2
          || x->type == FUNCTION3
          || x->type == FUNCTIONn)
    ? &scm_t : &scm_f;
}

scm *
boolean_p (scm *x)
{
  return (x == &scm_t || x == &scm_f) ? &scm_t : &scm_f;
}

scm *
char_p (scm *x)
{
  return x->type == CHAR ? &scm_t : &scm_f;
}

scm *
number_p (scm *x)
{
  return x->type == NUMBER ? &scm_t : &scm_f;
}

scm *
string_p (scm *x)
{
  return x->type == STRING ? &scm_t : &scm_f;
}

scm *
internal_p (scm *x)
{
  return x->type == SCM ? &scm_t : &scm_f;
}

scm *
symbol_p (scm *x)
{
  return x->type == SYMBOL ? &scm_t : &scm_f;
}

scm *
vector_p (scm *x)
{
  return x->type == VECTOR ? &scm_t : &scm_f;
}

scm *
display (scm *x/*...*/)
{
  scm *e = car (x);
  scm *p = cdr (x);
  int fd = 1;
  if (p->type == PAIR && p->car->type == NUMBER) fd = p->car->value;
  FILE *f = fd == 1 ? stdout : stderr;
  return display_helper (f, e, false, "", false);
}

scm *
display_ (FILE* f, scm *x) //internal
{
  return display_helper (f, x, false, "", false);
}

scm *
call (scm *fn, scm *x)
{
  if (fn->type == FUNCTION0)
    return fn->function0 ();
  if (x->car->type == VALUES)
    x = cons (x->car->cdr->car, &scm_nil);
  if (fn->type == FUNCTION1)
    return fn->function1 (car (x));
  if (fn->type == FUNCTION2)
    return fn->function2 (car (x), cadr (x));
  if (fn->type == FUNCTION3)
    return fn->function3 (car (x), cadr (x), caddr (x));
  if (fn->type == FUNCTIONn)
    return fn->functionn (x);
  return &scm_unspecified;
}

scm *
append2 (scm *x, scm *y)
{
  if (x == &scm_nil) return y;
  assert (x->type == PAIR);
  return cons (car (x), append2 (cdr (x), y));
}

scm *
append (scm *x/*...*/)
 {
  if (x == &scm_nil) return &scm_nil;
  return append2 (car (x), append (cdr (x)));
 }

scm *
make_char (int x)
{
  scm *p = (scm*)malloc (sizeof (scm));
  p->type = CHAR;
  p->value = x;
  return p;
}

scm *
make_macro (scm *name, scm *x)
{
  scm *p = (scm*)malloc (sizeof (scm));
  p->type = MACRO;
  p->macro = x;
  p->name = name->name;
  return p;
}

scm *
make_number (int x)
{
  scm *p = (scm*)malloc (sizeof (scm));
  p->type = NUMBER;
  p->value = x;
  return p;
}

scm *
make_string (char const *s)
{
  scm *p = (scm*)malloc (sizeof (scm));
  p->type = STRING;
  p->name = strdup (s);
  return p;
}

scm *symbols = 0;

scm *
internal_lookup_symbol (char const *s)
{
  scm *x = symbols;
  while (x && strcmp (s, x->car->name)) x = x->cdr;
  if (x) x = x->car;
  return x;
}

scm *
internal_make_symbol (char const *s)
{
  scm *x = (scm*)malloc (sizeof (scm));
  x->type = SYMBOL;
  x->name = strdup (s);
  x->value = 0;
  symbols = cons (x, symbols);
  return x;
}

scm *
make_symbol (char const *s)
{
  scm *x = internal_lookup_symbol (s);
  return x ? x : internal_make_symbol (s);
}

scm *
make_vector (scm *n)
{
  scm *p = (scm*)malloc (sizeof (scm));
  p->type = VECTOR;
  p->length = n->value;
  p->vector = (scm**)malloc (n->value * sizeof (scm*));
  for (int i=0; i<n->value; i++) p->vector[i] = &scm_unspecified;
  return p;
}

scm *
string (scm *x/*...*/)
{
  char buf[STRING_MAX] = "";
  char *p = buf;
  while (x != &scm_nil)
    {
      scm *s = car (x);
      assert (s->type == CHAR);
      *p++ = s->value;
      x = cdr (x);
    }
  return make_string (buf);
}

scm *
string_append (scm *x/*...*/)
{
  char buf[STRING_MAX] = "";

  while (x != &scm_nil)
    {
      scm *s = car (x);
      assert (s->type == STRING);
      strcat (buf, s->name);
      x = cdr (x);
    }
  return make_string (buf);
}

scm *
list_to_string (scm *x)
{
  char buf[STRING_MAX] = "";
  char *p = buf;
  while (x != &scm_nil)
    {
      scm *s = car (x);
      assert (s->type == CHAR);
      *p++ = s->value;
      x = cdr (x);
    }
  *p = 0;
  return make_string (buf);
}

scm *
string_length (scm *x)
{
  assert (x->type == STRING);
  return make_number (strlen (x->name));
}

scm *
string_ref (scm *x, scm *k)
{
  assert (x->type == STRING);
  assert (k->type == NUMBER);
  return make_char (x->name[k->value]);
}

scm *
substring (scm *x/*...*/)
{
  assert (x->type == PAIR);
  assert (x->car->type == STRING);
  char const *s = x->car->name;
  assert (x->cdr->car->type == NUMBER);
  int start = x->cdr->car->value;
  int end = strlen (s);
  if (x->cdr->cdr->type == PAIR) {
    assert (x->cdr->cdr->car->type == NUMBER);
    assert (x->cdr->cdr->car->value <= end);
    end = x->cdr->cdr->car->value;
  }
  char buf[STRING_MAX];
  strncpy (buf, s+start, end - start);
  buf[end-start] = 0;
  return make_string (buf);
}

scm *
length (scm *x)
{
  int n = 0;
  while (x != &scm_nil)
    {
      n++;
      x = cdr (x);
    }
  return make_number (n);
}

scm *
last_pair (scm *x)
{
  //if (x != &scm_nil && cdr (x) != &scm_nil)
  //return last_pair (cdr (x));
  while (x != &scm_nil && cdr (x) != &scm_nil)
    x = cdr (x);
  return x;
}

scm *
builtin_list (scm *x/*...*/)
{
  return x;
}

scm *
values (scm *x/*...*/)
{
  scm *v = cons (0, x);
  v->type = VALUES;
  return v;
}

scm *
call_with_values_env (scm *producer, scm *consumer, scm *a)
{
  scm *v = apply_env (producer, &scm_nil, a);
  if (v->type == VALUES)
    v = v->cdr;
  return apply_env (consumer, v, a);
}

scm *
vector_length (scm *x)
{
  assert (x->type == VECTOR);
  return make_number (x->length);
}

scm *
vector_ref (scm *x, scm *i)
{
  assert (x->type == VECTOR);
  assert (i->value < x->length);
  return x->vector[i->value];
}

scm *
vector_set_x (scm *x, scm *i, scm *e)
{
  assert (x->type == VECTOR);
  assert (i->value < x->length);
  x->vector[i->value] = e;
  return &scm_unspecified;
}

scm *
lookup (char const *s, scm *a)
{
  if (isdigit (*s) || (*s == '-' && isdigit (*(s+1))))
    return make_number (atoi (s));

  scm *x;
  x = internal_lookup_symbol (s);
  if (x) return x;

  if (*s == '\'') return &symbol_quote;
  if (*s == '`') return &symbol_quasiquote;
  if (*s == ',' && *(s+1) == '@') return &symbol_unquote_splicing;
  if (*s == ',') return &symbol_unquote;

  if (*s == '#' && *(s+1) == '\'') return &symbol_syntax;
  if (*s == '#' && *(s+1) == '`') return &symbol_quasisyntax;
  if (*s == '#' && *(s+1) == ',' && *(s+2) == '@') return &symbol_unsyntax_splicing;
  if (*s == '#' && *(s+1) == ',') return &symbol_unsyntax;

  if (!strcmp (s, "EOF")) {
    fprintf (stderr, "mes: got EOF\n");
    return &scm_nil; // `EOF': eval program, which may read stdin
  }

  return internal_make_symbol (s);
}

scm *
lookup_char (int c, scm *a)
{
  char buf[2];
  buf[0] = c;
  buf[1] = 0;
  return lookup (buf, a);
}

char const *
list2str (scm *l) // char*
{
  static char buf[STRING_MAX];
  char *p = buf;
  while (l != &scm_nil) {
    scm *c = car (l);
    assert (c->type == NUMBER);
    *p++ = c->value;
    l = cdr (l);
  }
  *p = 0;
  return buf;
}

scm*
list_to_vector (scm *x)
{
  temp_number.value = length (x)->value;
  scm *v = make_vector (&temp_number);
  scm **p = v->vector;
  while (x != &scm_nil)
    {
      *p++ = car (x);
      x = cdr (x);
    }
  return v;
}

scm*
integer_to_char (scm *x)
{
  assert (x->type == NUMBER);
  return make_char (x->value);
}

scm*
char_to_integer (scm *x)
{
  assert (x->type == CHAR);
  return make_number (x->value);
}

scm*
number_to_string (scm *x)
{
  assert (x->type == NUMBER);
  char buf[STRING_MAX];
  sprintf (buf,"%d", x->value);
  return make_string (buf);
}

scm*
builtin_exit (scm *x)
{
  assert (x->type == NUMBER);
  exit (x->value);
}

scm*
string_to_symbol (scm *x)
{
  assert (x->type == STRING);
  return make_symbol (x->name);
}

scm*
symbol_to_string (scm *x)
{
  assert (x->type == SYMBOL);
  return make_string (x->name);
}

scm*
vector_to_list (scm *v)
{
  scm *x = &scm_nil;
  for (int i = 0; i < v->length; i++)
    x = append2 (x, cons (v->vector[i], &scm_nil));
  return x;
}

scm *
newline (scm *p/*...*/)
{
  int fd = 1;
  if (p->type == PAIR && p->car->type == NUMBER) fd = p->car->value;
  FILE *f = fd == 1 ? stdout : stderr;
  fputs ("\n", f);
  return &scm_unspecified;
}

scm *
force_output (scm *p/*...*/)
{
  int fd = 1;
  if (p->type == PAIR && p->car->type == NUMBER) fd = p->car->value;
  FILE *f = fd == 1 ? stdout : stderr;
  fflush (f);
}

scm *
display_helper (FILE* f, scm *x, bool cont, char const *sep, bool quote)
{
  scm *r;
  fprintf (f, "%s", sep);
  if (x->type == CHAR && x->value == char_nul.value) fprintf (f, "#\\%s", char_nul.name);
  else if (x->type == CHAR && x->value == char_backspace.value) fprintf (f, "#\\%s", char_backspace.name);
  else if (x->type == CHAR && x->value == char_tab.value) fprintf (f, "#\\%s", char_tab.name);
  else if (x->type == CHAR && x->value == char_newline.value) fprintf (f, "#\\%s", char_newline.name);
  else if (x->type == CHAR && x->value == char_vt.value) fprintf (f, "#\\%s", char_vt.name);
  else if (x->type == CHAR && x->value == char_page.value) fprintf (f, "#\\%s", char_page.name);
  else if (x->type == CHAR && x->value == char_return.value) fprintf (f, "#\\%s", char_return.name);
  else if (x->type == CHAR && x->value == char_space.value) fprintf (f, "#\\%s", char_space.name);
  else if (x->type == CHAR) fprintf (f, "#\\%c", x->value);
  else if (x->type == MACRO) {
    fprintf (f, "(*macro* ");
    display_helper (f, x->macro, cont, sep, quote);
    fprintf (f, ")");
  }
  else if (x->type == NUMBER) fprintf (f, "%d", x->value);
  else if (x->type == PAIR) {
    if (car (x) == &scm_circular) {
      fprintf (f, "(*circ* . #-1#)");
      return &scm_unspecified;
    }
    if (car (x) == &scm_closure) {
      fprintf (f, "(*closure* . #-1#)");
      return &scm_unspecified;
    }
    if (car (x) == &scm_quote) {
      fprintf (f, "'");
      return display_helper (f, car (cdr (x)), cont, "", true);
    }
    if (!cont) fprintf (f, "(");
    display_ (f, car (x));
    if (cdr (x)->type == PAIR)
      display_helper (f, cdr (x), true, " ", false);
    else if (cdr (x) != &scm_nil) {
      fprintf (f, " . ");
      display_ (f, cdr (x));
    }
    if (!cont) fprintf (f, ")");
  }
  else if (x->type == VECTOR) {
    fprintf (f, "#(", x->length);
    for (int i = 0; i < x->length; i++) {
      if (x->vector[i]->type == VECTOR)
        fprintf (f, "%s#(...)", i ? " " : "");
      else
        display_helper (f, x->vector[i], false, i ? " " : "", false);
    }
    fprintf (f, ")");
  }
  else if (builtin_p (x) == &scm_t) fprintf (f, "#<procedure %s>", x->name);
  else if (atom_p (x) == &scm_t) fprintf (f, "%s", x->name);

  return &scm_unspecified;
}

// READ

int
ungetchar (int c) //int
{
  return ungetc (c, stdin);
}

int
peekchar () //int
{
  int c = getchar ();
  ungetchar (c);
  return c;
}

scm*
peek_char ()
{
  return make_char (peekchar ());
}

scm *
read_char ()
{
  return make_char (getchar ());
}

scm *
write_char (scm *x/*...*/)
{
  scm *c = car (x);
  scm *p = cdr (x);
  int fd = 1;
  if (p->type == PAIR && p->car->type == NUMBER) fd = p->car->value;
  FILE *f = fd == 1 ? stdout : stderr;
  assert (c->type == NUMBER || c->type == CHAR);
  fputc (c->value, f);
  return c;
}

scm*
unget_char (scm *c)
{
  assert (c->type == NUMBER || c->type == CHAR);
  ungetchar (c->value);
  return c;
}

int
readcomment (int c)
{
  if (c == '\n') return c;
  return readcomment (getchar ());
}

int
readblock (int c)
{
  if (c == '!' && peekchar () == '#') return getchar ();
  return readblock (getchar ());
}

scm *
readword (int c, char *w, scm *a)
{
  if (c == EOF && !w) return &scm_nil;
  if (c == '\n' && !w) return readword (getchar (), w, a);
  if (c == '\n' && *w == '.' && w[1] == 0) return &scm_dot;
  if (c == EOF || c == '\n') return lookup (w, a);
  if (c == ' ') return readword ('\n', w, a);
  if (c == '"' && !w) return readstring ();
  if (c == '"') {ungetchar (c); return lookup (w, a);}
  if (c == '(' && !w) return readlist (a);
  if (c == '(') {ungetchar (c); return lookup (w, a);}
  if (c == ')' && !w) {ungetchar (c); return &scm_nil;}
  if (c == ')') {ungetchar (c); return lookup (w, a);}
  if (c == ',' && peekchar () == '@') {getchar (); return cons (lookup (",@", a),
                                                                   cons (readword (getchar (), w, a),
                                                                         &scm_nil));}
  if ((c == '\''
       || c == '`'
       || c == ',')
      && !w) {return cons (lookup_char (c, a),
                                     cons (readword (getchar (), w, a),
                                           &scm_nil));}
  if (c == '#' && peekchar () == ',' && !w) {
    getchar ();
    if (peekchar () == '@'){getchar (); return cons (lookup ("#,@", a),
                                                     cons (readword (getchar (), w, a),
                                                           &scm_nil));}
    return cons (lookup ("#,", a), cons (readword (getchar (), w, a), &scm_nil));
  }
  if (c == '#'
     && (peekchar () == '\''
         || peekchar () == '`')
     && !w) {char buf[3] = "#"; buf[1] = getchar (); return cons (lookup (buf, a),
                          cons (readword (getchar (), w, a),
                                &scm_nil));}
  if (c == ';') {readcomment (c); return readword ('\n', w, a);}
  if (c == '#' && peekchar () == 'x') {getchar (); return read_hex ();}
  if (c == '#' && peekchar () == '\\') {getchar (); return read_character ();}
  if (c == '#' && !w && peekchar () == '(') {getchar (); return list_to_vector (readlist (a));}
  if (c == '#' && peekchar () == '(') {ungetchar (c); return lookup (w, a);}
  if (c == '#' && peekchar () == '!') {getchar (); readblock (getchar ()); return readword (getchar (), w, a);}
  char buf[STRING_MAX] = {0};
  char ch = c;
  char *p = w ? w + strlen (w) : buf;
  *p = ch;
  *(p+1) = 0;
  return readword (getchar (), w ? w : buf, a);
}

scm *
read_hex ()
{
  int n = 0;
  int c = peekchar ();
  while ((c >= '0' && c <= '9')
         || (c >= 'A' && c <= 'F')
         || (c >= 'a' && c <= 'f')) {
    n <<= 4;
    if (c >= 'a') n += c - 'a' + 10;
    else if (c >= 'A') n += c - 'A' + 10;
    else n+= c - '0';
    getchar ();
    c = peekchar ();
  }
  return make_number (n);
}

scm *
read_character ()
{
  int c = getchar ();
  if (c >= '0' && c <= '7'
      && peekchar () >= '0' && peekchar () <= '7') {
    c = c - '0';
    while (peekchar () >= '0' && peekchar () <= '7') {
      c <<= 3;
      c += getchar () - '0';
    }
  }
  else if (c >= 'a' && c <= 'z'
      && peekchar () >= 'a' && peekchar () <= 'z') {
    char buf[STRING_MAX];
    char *p = buf;
    *p++ = c;
    while (peekchar () >= 'a' && peekchar () <= 'z') {
      *p++ = getchar ();
    }
    *p = 0;
    if (!strcmp (buf, char_nul.name)) c = char_nul.value;
    else if (!strcmp (buf, char_backspace.name)) c = char_backspace.value;
    else if (!strcmp (buf, char_tab.name)) c = char_tab.value;
    else if (!strcmp (buf, char_newline.name)) c = char_newline.value;
    else if (!strcmp (buf, char_vt.name)) c = char_vt.value;
    else if (!strcmp (buf, char_page.name)) c = char_page.value;
    else if (!strcmp (buf, char_return.name)) c = char_return.value;
    else if (!strcmp (buf, char_space.name)) c = char_space.value;
    else {
      fprintf (stderr, "char not supported: %s\n", buf);
      assert (!"char not supported");
    }
  }
  return make_char (c);
}

scm *
readstring ()
{
  char buf[STRING_MAX];
  char *p = buf;
  int c = getchar ();
  while (true) {
    if (c == '"') break;
    if (c == '\\' && peekchar () == '"') *p++ = getchar ();
    else if (c == '\\' && peekchar () == 'n') {getchar (); *p++ = '\n';}
    else if (c == EOF) assert (!"EOF in string");
    else *p++ = c;
    c = getchar ();
  }
  *p = 0;
  return make_string (buf);
}

int
eat_whitespace (int c)
{
  while (c == ' ' || c == '\t' || c == '\n') c = getchar ();
  if (c == ';') return eat_whitespace (readcomment (c));
  if (c == '#' && peekchar () == '!') {getchar (); readblock (getchar ()); return eat_whitespace (getchar ());}
  return c;
}

scm *
readlist (scm *a)
{
  int c = getchar ();
  c = eat_whitespace (c);
  if (c == ')') return &scm_nil;
  scm *w = readword (c, 0, a);
  if (w == &scm_dot)
    return car (readlist (a));
  return cons (w, readlist (a));
}

scm *
read_env (scm *a)
{
  return readword (getchar (), 0, a);
}

scm *
greater_p (scm *x/*...*/)
{
  int n = INT_MAX;
  while (x != &scm_nil)
    {
      assert (x->car->type == NUMBER);
      if (x->car->value >= n) return &scm_f;
      n = x->car->value;
      x = cdr (x);
    }
  return &scm_t;
}

scm *
less_p (scm *x/*...*/)
{
  int n = INT_MIN;
  while (x != &scm_nil)
    {
      assert (x->car->type == NUMBER);
      if (x->car->value <= n) return &scm_f;
      n = x->car->value;
      x = cdr (x);
    }
  return &scm_t;
}

scm *
is_p (scm *x/*...*/)
{
  if (x == &scm_nil) return &scm_t;
  assert (x->car->type == NUMBER);
  int n = x->car->value;
  x = cdr (x);
  while (x != &scm_nil)
    {
      if (x->car->value != n) return &scm_f;
      x = cdr (x);
    }
  return &scm_t;
}

scm *
minus (scm *x/*...*/)
{
  scm *a = car (x);
  assert (a->type == NUMBER);
  int n = a->value;
  x = cdr (x);
  if (x == &scm_nil)
    n = -n;
  while (x != &scm_nil)
    {
      assert (x->car->type == NUMBER);
      n -= x->car->value;
      x = cdr (x);
    }
  return make_number (n);
}

scm *
plus (scm *x/*...*/)
{
  int n = 0;
  while (x != &scm_nil)
    {
      assert (x->car->type == NUMBER);
      n += x->car->value;
      x = cdr (x);
    }
  return make_number (n);
}

scm *
divide (scm *x/*...*/)
{
  int n = 1;
  if (x != &scm_nil) {
    assert (x->car->type == NUMBER);
    n = x->car->value;
    x = cdr (x);
  }
  while (x != &scm_nil)
    {
      assert (x->car->type == NUMBER);
      n /= x->car->value;
      x = cdr (x);
    }
  return make_number (n);
}

scm *
modulo (scm *a, scm *b)
{
  assert (a->type == NUMBER);
  assert (b->type == NUMBER);
  return make_number (a->value % b->value);
}

scm *
multiply (scm *x/*...*/)
{
  int n = 1;
  while (x != &scm_nil)
    {
      assert (x->car->type == NUMBER);
      n *= x->car->value;
      x = cdr (x);
    }
  return make_number (n);
}

scm *
logior (scm *x/*...*/)
{
  int n = 0;
  while (x != &scm_nil)
    {
      assert (x->car->type == NUMBER);
      n |= x->car->value;
      x = cdr (x);
    }
  return make_number (n);
}

scm *add_environment (scm *a, char const *name, scm *x);

#if BUILTIN_QUASIQUOTE
scm *
add_unquoters (scm *a)
{
  a = cons (cons (&symbol_unquote, &scm_unquote), a);
  a = cons (cons (&symbol_unquote_splicing, &scm_unquote_splicing), a);
  return a;
}

scm *
add_unsyntaxers (scm *a)
{
  a = cons (cons (&symbol_unsyntax, &scm_unsyntax), a);
  a = cons (cons (&symbol_unsyntax_splicing, &scm_unsyntax_splicing), a);
  return a;
}
#endif // BUILTIN_QUASIQUOTE

scm *
add_environment (scm *a, char const *name, scm *x)
{
  return cons (cons (make_symbol (name), x), a);
}

scm *
mes_environment ()
{
  scm *a = &scm_nil;

  #include "symbols.i"

#if BOOT
  symbols = cons (&scm_label, symbols);
  a = cons (cons (&scm_label, &scm_t), a);
#endif

  a = cons (cons (&scm_f, &scm_f), a);
  a = cons (cons (&scm_nil, &scm_nil), a);
  a = cons (cons (&scm_t, &scm_t), a);
  a = cons (cons (&scm_unspecified, &scm_unspecified), a);
  a = cons (cons (&symbol_begin, &symbol_begin), a);
  a = cons (cons (&symbol_quote, &scm_quote), a);
  a = cons (cons (&symbol_syntax, &scm_syntax), a);

#if MES_FULL
#include "environment.i"
#else
  a = add_environment (a, "display", &scm_display);
  a = add_environment (a, "newline", &scm_newline);
#endif
  a = cons (cons (&scm_closure, a), a);
  return a;
}

scm *
make_lambda (scm *args, scm *body)
{
  return cons (&scm_lambda, cons (args, body));
}

scm *
make_closure (scm *args, scm *body, scm *a)
{
  return cons (&scm_closure, cons (cons (&scm_circular, a), cons (args, body)));
}

#if !BOOT
scm *
define (scm *x, scm *a)
{
  scm *e;
  scm *name = cadr (x);
  if (name->type != PAIR)
    e = builtin_eval (caddr (x), cons (cons (cadr (x), cadr (x)), a));
  else {
    name = car (name);
    scm *p = pairlis (cadr (x), cadr (x), a);
    e = builtin_eval (make_lambda (cdadr (x), cddr (x)), p);
  }
  if (eq_p (car (x), &symbol_define_macro) == &scm_t)
    e = make_macro (name, e);
  scm *entry = cons (name, e);
  scm *aa = cons (entry, &scm_nil);
  set_cdr_x (aa, cdr (a));
  set_cdr_x (a, aa);
  scm *cl = assq (&scm_closure, a);
  set_cdr_x (cl, aa);
  return entry;
}
#else // BOOT
scm*define (scm *x, scm *a){}
#endif

scm *
define_macro (scm *x, scm *a)
{
}

scm *
lookup_macro (scm *x, scm *a)
{
  if (x->type != SYMBOL) return &scm_f;
  scm *m = assq_ref_cache (x, a);
  if (macro_p (m) == &scm_t) return m->macro;
  return &scm_f;
}

scm *
read_file (scm *e, scm *a)
{
  if (e == &scm_nil) return e;
#if DEBUG
  scm *x = cons (e, read_file (read_env (a), a));
  display_ (stderr, x);
#else
  return cons (e, read_file (read_env (a), a));
#endif
}

int
main (int argc, char *argv[])
{
  if (argc > 1 && !strcmp (argv[1], "--help")) return puts ("Usage: mes < FILE\n");
  if (argc > 1 && !strcmp (argv[1], "--version")) return puts ("Mes 0.0\n");
  scm *a = mes_environment ();
  display_ (stderr, builtin_eval (cons (&symbol_begin, read_file (read_env (a), a)), a));
  fputs ("", stderr);
  return 0;
}
