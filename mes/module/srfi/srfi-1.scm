;;; -*-scheme-*-

;;; GNU Mes --- Maxwell Equations of Software
;;; Copyright © 2021 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
;;; Copyright © 2022 Timothy Sample <samplet@ngyro.com>
;;;
;;; This file is part of GNU Mes.
;;;
;;; GNU Mes is free software; you can redistribute it and/or modify it
;;; under the terms of the GNU General Public License as published by
;;; the Free Software Foundation; either version 3 of the License, or (at
;;; your option) any later version.
;;;
;;; GNU Mes is distributed in the hope that it will be useful, but
;;; WITHOUT ANY WARRANTY; without even the implied warranty of
;;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;;; GNU General Public License for more details.
;;;
;;; You should have received a copy of the GNU General Public License
;;; along with GNU Mes.  If not, see <http://www.gnu.org/licenses/>.

;;; Commentary:

;;; SRFI 1: List Library.  This module provides procedures for
;;; constructing, searching, and manipulating lists.

;;; Code:

(define-module (srfi srfi-1)
  #:re-export (every
               find
               filter
               append-map
               filter-map
               fold
               fold-right
               unfold
               remove
               reverse!
               mes:member
               srfi-1:member
               member
               mes:iota
               srfi-1:iota
               iota
               delete-duplicates
               any
               any1
               every
               every1
               list-index
               lset-union
               lset-intersection
               lset-difference
               reverse!
               take-while))
