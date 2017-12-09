; Clayton Brutus
; Exercise 2.31
; CS380 Programming Languages
; 9/20/16

#lang eopl

(define-datatype prefix-exp prefix-exp?
  (const-exp
   (num integer?))
  (diff-exp
   (operand1 prefix-exp?)
   (operand2 prefix-exp?)))


(define parse
  (lambda (pl)
    (if (null? pl) 'error
        (car (parse-aux pl)))))

(define parse-aux
  (lambda (pl)
    (if (null? (cdr pl))
        (list (const-exp (car pl) '()))
        (if (integer? (car pl))
            (list (const-exp (car pl)) (cdr pl))
            (list (diff-exp (car (parse-aux (cdr pl)))
                            (car (parse-aux (cadr (parse-aux (cdr pl)
