/* -*-comment-start: "//";comment-end:""-*-
 * GNU Mes --- Maxwell Equations of Software
 * Copyright © 2017,2018,2022 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
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
#ifndef __MES_LOCALE_H
#define __MES_LOCALE_H 1

#if SYSTEM_LIBC
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#undef __MES_LOCALE_H
#include_next <locale.h>

#else // ! SYSTEM_LIBC

// *INDENT-OFF*
#ifndef LC_ALL
#define LC_CTYPE   0
#define LC_NUMERIC 1
#define LC_COLLATE 3
#define LC_ALL     6
#endif
// *INDENT-ON*

struct lconv
{
  char *decimal_point;
  char *thousands_sep;
  char *grouping;

  char *int_curr_symbol;
  char *currency_symbol;
  char *mon_decimal_point;
  char *mon_thousands_sep;
  char *mon_grouping;
  char *positive_sign;
  char *negative_sign;
  char int_frac_digits;
  char frac_digits;
  char p_cs_precedes;
  char p_sep_by_space;
  char n_cs_precedes;
  char n_sep_by_space;
  char p_sign_posn;
  char n_sign_posn;
};

struct lconv * localeconv (void);
char *setlocale (int category, char const *locale);

#endif // ! SYSTEM_LIBC

#endif // __MES_LOCALE_H
