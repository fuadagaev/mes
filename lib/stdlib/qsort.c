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

void
swap (void* a, void* b, int size)
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
    void *vt, *v3;
    int i;
    int last;
    int mid = (left + right) / 2;
    if (left >= right)
        return;
  
    void* vl = (char*)(base + (left * size));
    void* vr = (char*)(base + (mid * size));
    swap (vl, vr, size);
    last = left;
    for (i = left + 1; i <= right; i++)
      {
        vt = (char*)(base + (i * size));
        if ((*compare)(vl, vt) > 0)
          {
            ++last;
            v3 = (char*)(base + (last * size));
            swap (vt, v3, size);
        }
    }
    v3 = (char*)(base + (last * size));
    swap (vl, v3, size);
    _qsort (base, size, left, last - 1, compare);
    _qsort (base, size, last + 1, right, compare);
}

void
qsort (void *base, size_t count, size_t size,
       int (*compare)(void const*, void const*))
{
  _qsort (base, size, 0, count, compare);
}
