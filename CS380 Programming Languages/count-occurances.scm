#lang eopl

(define co
  (lambda  (slst s)
    (if (null? slst) 0
        (if (list? (car slst) )
            (+ (co (car slst) s) (co (cdr slst) s))
            (if (equ? (car slst) s)
                (add 1 (co (cdr slst) s))
                (co (cdr slst) s))))))
