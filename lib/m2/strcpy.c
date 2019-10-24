/* -*-comment-start: "
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

#include <string.h>

char *
strcpy (char *dest, char *src)
{
  /* eputs ("\nstrcpy: src="); */
  /* eputs (src); */
  /* eputs ("\n"); */
  char *p = dest;
  char *orig = dest;

  /* eputs ("dest="); */
  /* eputs (dest); */
  /* eputs ("\n"); */

  /* eputs ("c:" ); */
  while (0 != src[0])
    {
      /* eputc (src[0]); */
      /* eputs (" "); */
      dest[0] = src[0];
      dest = dest + 1;
      src = src + 1;
    }
  dest[0] = 0;
  /* eputs ("\n   => orig="); */
  /* eputs (orig); */
  /* eputs ("\n"); */

  return p;
}
