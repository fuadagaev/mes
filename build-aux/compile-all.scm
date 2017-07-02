;;; GNU Guix --- Functional package management for GNU
;;; Copyright © 2016 Taylan Ulrich Bayırlı/Kammer <taylanbayirli@gmail.com>
;;; Copyright © 2016, 2017 Ludovic Courtès <ludo@gnu.org>
;;;
;;; This file is part of GNU Guix.
;;;
;;; GNU Guix is free software; you can redistribute it and/or modify it
;;; under the terms of the GNU General Public License as published by
;;; the Free Software Foundation; either version 3 of the License, or (at
;;; your option) any later version.
;;;
;;; GNU Guix is distributed in the hope that it will be useful, but
;;; WITHOUT ANY WARRANTY; without even the implied warranty of
;;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;;; GNU General Public License for more details.
;;;
;;; You should have received a copy of the GNU General Public License
;;; along with GNU Guix.  If not, see <http://www.gnu.org/licenses/>.

(use-modules (system base target)
             (system base message)
             (ice-9 match)
             (ice-9 threads))

(define (mkdir-p dir)
  "Create directory DIR and all its ancestors."
  (define absolute?
    (string-prefix? "/" dir))

  (define not-slash
    (char-set-complement (char-set #\/)))

  (let loop ((components (string-tokenize dir not-slash))
             (root       (if absolute?
                             ""
                             ".")))
    (match components
      ((head tail ...)
       (let ((path (string-append root "/" head)))
         (catch 'system-error
           (lambda ()
             (mkdir path)
             (loop tail path))
           (lambda args
             (if (= EEXIST (system-error-errno args))
                 (loop tail path)
                 (apply throw args))))))
      (() #t))))

(define warnings
  '(unsupported-warning format unbound-variable arity-mismatch))

(define host (getenv "host"))

(define srcdir (getenv "srcdir"))

(define (relative-file file)
  (if (string-prefix? (string-append srcdir "/") file)
      (string-drop file (+ 1 (string-length srcdir)))
      file))

(define (file-mtime<? f1 f2)
  (< (stat:mtime (stat f1))
     (stat:mtime (stat f2))))

(define (scm->go file)
  (let* ((relative (relative-file file))
         (without-extension (string-drop-right relative 4)))
    (string-append without-extension ".go")))

(define (scm->mes file)
  (let ((base (string-drop-right file 4)))
    (string-append base ".mes")))

(define (file-needs-compilation? file)
  (let ((go (scm->go file)))
    (or (not (file-exists? go))
        (file-mtime<? go file)
        (let ((mes (scm->mes file))) ; FIXME: try to respect (include-from-path ".mes")
          (and (file-exists? mes)
               (file-mtime<? go mes))))))

(define (file->module file)
  (let* ((relative (relative-file file))
         (module-path (string-drop-right relative 4)))
    (map string->symbol
         (string-split module-path #\/))))

;;; To work around <http://bugs.gnu.org/15602> (FIXME), we want to load all
;;; files to be compiled first.  We do this via resolve-interface so that the
;;; top-level of each file (module) is only executed once.
(define (load-module-file file)
  (let ((module (file->module file)))
    (format #t "  LOAD     ~a~%" module)
    (resolve-interface module)))

(cond-expand
 (guile-2.2 (use-modules (language tree-il optimize)
                         (language cps optimize)))
 (else #f))

(define %default-optimizations
  ;; Default optimization options (equivalent to -O2 on Guile 2.2).
  (cond-expand
   (guile-2.2 (append (tree-il-default-optimization-options)
                      (cps-default-optimization-options)))
    (else '())))

(define %lightweight-optimizations
  ;; Lightweight optimizations (like -O0, but with partial evaluation).
  (let loop ((opts %default-optimizations)
             (result '()))
    (match opts
      (() (reverse result))
      ((#:partial-eval? _ rest ...)
       (loop rest `(#t #:partial-eval? ,@result)))
      ((kw _ rest ...)
       (loop rest `(#f ,kw ,@result))))))

(define (optimization-options file)
  (if (string-contains file "gnu/packages/")
      %lightweight-optimizations                  ;build faster
      '()))

(define (compile-file* file output-mutex)
  (let ((go (scm->go file)))
    (with-mutex output-mutex
      (format #t "  GUILEC   ~a~%" go)
      (force-output))
    (mkdir-p (dirname go))
    (with-fluids ((*current-warning-prefix* ""))
      (with-target host
        (lambda ()
          (compile-file file
                        #:output-file go
                        #:opts `(#:warnings ,warnings
                                 ,@(optimization-options file))))))))

;; Install a SIGINT handler to give unwind handlers in 'compile-file' an
;; opportunity to run upon SIGINT and to remove temporary output files.
(sigaction SIGINT
  (lambda args
    (exit 1)))

(match (command-line)
  ((_ . files)
   (let ((files (filter file-needs-compilation? files)))
     (for-each load-module-file files)
     (let ((mutex (make-mutex)))
       ;; Make sure compilation related modules are loaded before starting to
       ;; compile files in parallel.
       (compile #f)
       (par-for-each (lambda (file)
                       (compile-file* file mutex))
                     files)))))

;;; Local Variables:
;;; eval: (put 'with-target 'scheme-indent-function 1)
;;; End:
