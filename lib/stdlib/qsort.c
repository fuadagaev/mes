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
#include <string.h>

#if 0
void
qswap (void *a, void *b, size_t size)
{
  char *pa = a;
  char *pb = b;
  do
    {
      char tmp = *pa;
      *pa++ = *pb;
      *pb++ = tmp;
    } while (--size > 0);
}
#else
void
qswap (void *a, void *b, int size)
{
  char buffer[size];
  memcpy (buffer, a, size);
  memcpy (a, b, size);
  memcpy (b, buffer, size);
}
#endif

size_t
qpart (char *base, size_t count, size_t size,
       int (*compare) (void const *, void const *))
{
  void *p1 = base + count * size;
  size_t i = 0;
  for (size_t j = 0; j < count; j++)
    {
      char *p2 = base + j * size;
      if (p1 == p2)
        i++;
      else if (compare (p2, p1) < 0)
        {
          char *p1 = base + i * size;
          qswap (p1, p2, size);
          i++;
        }
    }

  char *p2 = base + i * size;
  if (p1 != p2 && compare (p1, p2) < 0)
    qswap (p1, p2, size);
  return i;
}

void
qsort (void *base, size_t count, size_t size,
       int (*compare) (void const *, void const *))
{
  if (count <= 1)
    return;
  size_t p = qpart (base, count - 1, size, compare);
  qsort (base, p, size, compare);
  char *pbase = base;
  char *p1 = pbase + p * size;
  size_t c1 = count - p;
  qsort (p1, c1, size, compare);
}
