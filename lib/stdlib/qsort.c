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


/**
 * Assuming precondition (P) that `end - begin >= 2`, this function reorders the elements
 * of range [begin, end) and returns a pointer `ret` such that the following
 * postconditions hold:
 *   - (Q1): `ret > begin`
 *   - (Q2): `ret < end`
 * and, for some value `p` in [begin, end):
 *   - (Q3): all values in [begin, ret) are lower than or equal to `p`
 *   - (Q4): all values in [ret, end) are greater than or equal to `p`
 */
char *
qpart (char *low, char *high, size_t size,
       int (*compare) (void const *, void const *))
{
  char *pivot = (low + (high - low)/2);

  // Loop invariants, all trivially verified at the start of the loop:
  //   - (A): values strictly to the left of `low` are lower than or equal to `pivot`
  //   - (B): there is at least one value at or to the right of `low` that is greater
  //     than or equal to `pivot`
  //   - (C): values at or to the right of `high` are greater than or equal to `pivot`
  //   - (D): there is at least one value strictly to the left of `high` that is lower
  //     than or equal to `pivot`
  //   - (E): `low <= high`
  //
  // The loop terminates because `high - low` decreases strictly at each execution of
  // the body (obvious).
  while (1)
    {
      // This loop terminates because of (B).
      int c = compare (low, pivot);
      while (c < 0)
        low += size;

      // Here, we have
      //   - (1): `*low >= pivot`
      //   - (2): `low <= high` because of (E) and (C)
      //   - properties (A) and (B) still hold because `low` has only moved
      //     past values strictly less than `pivot`

      // This loop terminates because of (D).
      do {
        --high;
        int c = compare (pivot, high);
      } while (c < 0);

      // Here, we have
      //   - (3): `*high <= pivot`
      //   - (4): by (C) which held before this loop, elements strictly to the
      //     right of `high` are known to be greater than or equal to `pivot`
      //     (but now (C) may not hold anymore)

      if (low >= high)
        {
          // Due to (1), (A) and (4), (Q3) and (Q4) are established with `pivot`
          // as `p`.
          // Clearly, (B) proves Q2.
          // See the rest of the answer below for a proof of (Q1).
          // This correctly finishes the qpart.
          return low;
        }

      // We have `low < high` and we swap...
      qswap (low, high, size);

      // ...and now,
      //   - by (1) and (4), invariant (C) is re-established
      //   - by (1), invariant (D) is re-established
      //   - (5): by (3), `*low <= pivot`

      ++low;
      // (A) already held before this increment. Thus, because of (5), (A)
      // still holds. Additionally, by (1), after the swap, (B) is
      // re-established. Finally, (E) is obvious.
    }
}

void
_qsort (char *low, char *high, size_t size,
       int (*compare) (void const *, void const *))
{
  // Trivial base case...
  if (low - high < size)
    return;

  // ...therefore pre-condition (P) of `qpart` is satisfied.
  char *p = qpart (low, high, size, compare);

  // Thanks to postconditions (Q1) and (Q2) of `qpart`, the ranges
  // [low, p) and [p, high) are non-empty, therefore the size of the ranges
  // passed to the recursive calls below is strictly lower than the size of
  // [low, high) in this call. Therefore the base case is eventually reached
  // and the algorithm terminates.

  // Thanks to postconditions (Q3) and (Q4) of `qpart`, and by induction
  // on the size of [low, high), the recursive calls below sort their
  // respective argument ranges and [low, high) is sorted as a result.
  _qsort (low, p, size, compare);
  _qsort (p, high, size, compare);
}

void
qsort (void *base, size_t count, size_t size,
       int (*compare) (void const *, void const *))
{
  char *high = base + count * size;
  _qsort (base, high, size, compare);
}
