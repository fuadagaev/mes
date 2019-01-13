/* -*-comment-start: "//";comment-end:""-*-
 * GNU Mes --- Maxwell Equations of Software
 * Copyright © 2016,2017,2018,2019 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
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

#include <mes/lib.h>

char *
ntoab (SCM x, int base, int signed_p)
{
  static char itoa_buf[20];
  char *p = itoa_buf + 11;
  *p-- = 0;
  int sign_p = 0;
  SCM u = x;

  if (signed_p && x < 0)
    {
      sign_p = 1;
      u = -x;
    }

  do
    {
      SCM i = u % base;
      if (i > 9)
        {
          *p = 'a' + i - 10;
        }
      else
        {
          *p = '0' + i;
        }
      p = p - 1;
      u = u / base;
    }
  while (u);

  if (sign_p && *(p + 1) != '0')
    {
      *p = '-';
      p = p - 1;
    }

  return p + 1;
}
