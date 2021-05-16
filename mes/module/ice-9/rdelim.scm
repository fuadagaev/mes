;;; GNU Mes --- Maxwell Equations of Software
;;; Copyright © 2016,2021 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
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

(define-module (ice-9 rdelim)
  #:export (read-line))

(define (read-line . rest)
  (let* ((port (if (pair? rest) (car rest) (current-input-port)))
         (handle-delim (if (and (pair? rest) (pair? (cdr rest))) (cadr rest) 'trim))
         (c (read-char port)))
    (if (eof-object? c) c
        (list->string
         (let loop ((c c))
           (if (or (eof-object? c) (eq? c #\newline)) (case handle-delim
                                                        ((trim) '())
                                                        ((concat) '(#\newline))
                                                        (else (error (format #f "not supported: handle-delim=~a" handle-delim))))
               (cons c (loop (read-char port)))))))))
