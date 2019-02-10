/* -*-comment-start: "//";comment-end:""-*-
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

char *
_getcwd (char *buffer, size_t size)
{
  int r = _sys_call2 (SYS_getcwd, (long)buffer, (long)size);
  if (r >= 0)
    return buffer;
  return 0;
}

char *
getcwd (char *buffer, size_t size)
{
  static char buf[PATH_MAX];
  if (buffer)
    return _getcwd (buffer, size);
  return _getcwd (buf, PATH_MAX);
}
