#! /bin/sh

# GNU Mes --- Maxwell Equations of Software
# Copyright © 2018 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
#
# This file is part of GNU Mes.
#
# GNU Mes is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or (at
# your option) any later version.
#
# GNU Mes is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Mes.  If not, see <http://www.gnu.org/licenses/>.

set -e

. ${srcdest}build-aux/config.sh
. ${srcdest}build-aux/trace.sh

# native
sh ${srcdest}build-aux/snarf.sh

ARCHDIR=1 NOLINK=1 sh ${srcdest}build-aux/cc.sh lib/libmes
sh ${srcdest}build-aux/cc.sh src/mes
cp src/mes.gcc-out src/mes
