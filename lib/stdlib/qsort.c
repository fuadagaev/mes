/* -*-comment-start: "//";comment-end:""-*-
 * GNU Mes --- Maxwell Equations of Software
 * Copyright © 2017,2018 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
 * Copyright © 2021 Paul Dersey <pdersey@gmail.com>
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

#include <stdlib.h>

void
qswap (void* a, void* b, int size)
{
  char buffer[size];
  memcpy (buffer, a, size);
  memcpy (a, b, size);
  memcpy (b, buffer, size);
}

void
_qsort (void *base, size_t size, size_t left, size_t right,
        int (*compare)(void const*, void const*))
{
  char *pbase = base;
  int mid = (left + right) / 2;
  if (left >= right)
    return;

  void *vl = pbase + (left * size);
  void *vr = pbase + (mid * size);
  qswap (vl, vr, size);

  int last = left;
  for (int i = left + 1; i <= right; i++)
    {
      void *vt = pbase + (i * size);
      if ((*compare)(vl, vt) > 0)
        {
          ++last;
          void *v3 = pbase + (last * size);
          qswap (vt, v3, size);
        }
    }
  void *v3 = pbase + (last * size);
  qswap (vl, v3, size);
  _qsort (base, size, left, last - 1, compare);
  _qsort (base, size, last + 1, right, compare);
}

void
qsort (void *base, size_t count, size_t size,
       int (*compare)(void const*, void const*))
{
  _qsort (base, size, 0, count, compare);
}
