/* -*-comment-start: "//";comment-end:""-*-
 * GNU Mes --- Maxwell Equations of Software
 * Copyright © 202 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
 * Rich Felker © 2011 <dalias@aerifal.cx>
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

#if 1
static void
qswap (char *a, char *b, size_t size)
{
  while (size-- > 0)
    {
      char tmp = *a;
      *a++ = *b;
      *b++ = tmp;
    }
}
#elif 0
static void
qswap (void *a, void *b, int size)
{
  char buffer[128];
  memcpy (buffer, a, size);
  memcpy (a, b, size);
  memcpy (b, buffer, size);
}
#else
#define MIN(a, b) ((a)<(b) ? (a) : (b))
static void
qswap (char *a, char *b, size_t size)
{
  char tmp[256];
  size_t i;
  while (size)
    {
      i = MIN (sizeof (tmp), size);
      memcpy (tmp, a, i);
      memcpy (a, b, i);
      memcpy (b, tmp, i);
      a += i;
      b += i;
      size -= i;
    }
}
#undef MIN
#endif

static void
sift (char *base, size_t low, size_t high, size_t size,
      int (*compare)(const void *, const void *))
{
  while (2 * low <= high)
    {
      size_t max = 2 * low;
      if (max < high
          && compare (base + max * size, base + (max + 1) * size) < 0)
        max++;
      if (max
          && compare (base + low * size, base + max * size) < 0)
        {
          qswap (base + low * size, base + max * size, size);
          low = max;
        }
      else
        break;
    }
}

void
qsort (void *base, size_t count, size_t size,
       int (*compare)(const void *, const void *))
{
  if (count <= 1)
    return;

  char *pbase = base;
  for (size_t i = (count + 1) / 2; i; i--)
    sift (pbase, i - 1, count - 1, size, compare);
  for (size_t i = count - 1; i; i--)
    {
      qswap (pbase, pbase + i * size, size);
      sift (pbase, 0, i - 1, size, compare);
    }
}
