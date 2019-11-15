(define-module (boo)
  #:use-module (ijs-9 receive)
  #:export (ZEE-MODULE
            bah))

(define *************************en-deze-dan************************ 42)
(define (*************************of-deze************************) 42)
(define bah "Bah!")
(display "*ZZZZEEE* module boo\n")
'ZZZEEE-MODULE
(define (ZEE-MODULE)
  (display "ZEE-MODULE\n"))

;; (module-define! (guile:current-module) 'ZEE-MODULE (lambda _ "HAHA\n"))
;; (module-define! (guile:current-module) 'bah "burp")

;; (define (make-fluid x)
;;   (display "HAHA\n"))
33
