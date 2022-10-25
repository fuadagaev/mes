/* -*-comment-start: "//";comment-end:""-*-
 * GNU Mes --- Maxwell Equations of Software
 * Copyright © 2017,2018 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
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

size_t
partition (void *base, size_t low, size_t high, size_t size,
           int (*compare) (void const *, void const *))
{
  // select the rightmost element as pivot
  void *pivot = base + high * size;

  // pointer for greater element
  size_t i = (low - 1);

  // traverse each element of the array
  // compare them with the pivot
  for (int j = low; j < high; j++)
    {
      int c = compare (base + j * size, pivot);
      if (c < 0)
        {
          // swap element at i with element at j
          void *p1 = base + i * size;
          void *p2 = base + j * size;
          qswap (p1, p2, size);

          // if element smaller than pivot is found
          // swap it with the greater element pointed by i
          i++;
        }
    }

  // swap the pivot element with the greater element at i
  void *p1 = base + (i + 1) * size;
  void *p2 = base + high * size;
  qswap (p1, p2, size);

  // return the partition point
  return (i + 1);
}

void
_qsort (void *base, size_t low, size_t high, size_t size,
        int (*compare) (void const *, void const *))
{
  if (low < high)
    {
      // find the pivot element such that
      // elements smaller than pivot are on left of pivot
      // elements greater than pivot are on right of pivot
      int pi = partition (base, low, high, size, compare);

      // recursive call on the left of pivot
      _qsort (base, low, pi - 1, size, compare);

      // recursive call on the right of pivot
      _qsort (base, pi + 1, high, size, compare);
    }
}

void
qsort (void *base, size_t count, size_t size,
       int (*compare) (void const *, void const *))
{
  _qsort (base, 0, count, size, compare);
}
