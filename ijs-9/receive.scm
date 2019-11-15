(define-module (ijs-9 receive)
  #:export (ijs-receive))

;;(define-public (ijs-receive) (display "ijs\n"))

(module-define! (guile:current-module) 'ijs-receive (display "ijs\n"))
