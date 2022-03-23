;;; -*-scheme-*-

;;; GNU Mes --- Maxwell Equations of Software
;;; Copyright © 2016,2017,2018,2019,2021 Jan (janneke) Nieuwenhuizen <janneke@gnu.org>
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

;;; Code:

(define-module (mes guile)
  #:use-module (srfi srfi-16)
  #:export (drain-input
            with-input-from-string
            with-input-from-file
            with-output-to-file
            with-error-to-file
            with-output-to-port
            core:open-input-file
            open-input-file
            dirname
            file-exists?
            set-port-encoding!))

(define (drain-input port) (read-string))

(define (with-input-from-string string thunk)
  (let ((prev (set-current-input-port (open-input-string string)))
        (r (thunk)))
    (set-current-input-port prev)
    r))

(define (with-input-from-file file thunk)
  (let ((port (open-input-file file)))
    (warn "opened" file "=>" port)
    (if (= port -1)
        (error 'no-such-file file)
        (let* ((save (current-input-port))
               (foo (warn 'poort (set-current-input-port port)))
               (r (thunk)))
          (set-current-input-port save)
          r))))

(define (with-output-to-file file thunk)
  (let ((port (open-output-file file)))
    (if (= port -1)
        (error 'cannot-open file)
        (let* ((save (current-output-port))
               (foo (set-current-output-port port))
               (r (thunk)))
          (set-current-output-port save)
          r))))

(define (with-error-to-file file thunk)
  (let ((port (open-output-file file)))
    (if (= port -1)
        (error 'cannot-open file)
        (let* ((save (current-error-port))
               (foo (set-current-error-port port))
               (r (thunk)))
          (set-current-error-port save)
          r))))

(define (with-output-to-port port thunk)
  (let* ((save (current-output-port))
         (foo (set-current-output-port port))
         (r (thunk)))
    (set-current-output-port save)
    r))

(define core:open-input-file open-input-file)
(define (open-input-file file)
  (let ((port (core:open-input-file file))
        (debug (and=> (getenv "MES_DEBUG") string->number)))
    (when (and debug (> debug 1))
      (core:display-error (string-append "open-input-file: `" file "'"))
      (when (> debug 3)
        (core:display-error " port=")
        (core:display-error port))
      (core:display-error "\n"))
    port))

(define (dirname file-name)
  (let* ((lst (string-split file-name #\/))
         (lst (filter (negate string-null?) lst)))
    (if (null? lst) (if (string-prefix? "/" file-name) "/" ".")
        (let ((dir (string-join (list-head lst (1- (length lst))) "/")))
          (if (string-prefix? "/" file-name) (string-append "/" dir)
              (if (string-null? dir) "."
                  dir))))))

(define (file-exists? o)
  (access? o R_OK))

(define (set-port-encoding! port encoding) #t)
