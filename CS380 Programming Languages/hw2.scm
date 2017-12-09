#lang eopl
; 2.5
(define empty-env
  (lambda () '()))

(define extend-env
  (lambda (var val env)
    (cons (list var val) env)))

(define apply-env
  (lambda (env search-var)
    (cond
      ((null? (car env))
       'no-binding-found)
      ((list? (car env))
       (let ((saved-var (car (car env)))
             (saved-val (car (cdr (car env))))
             (saved-env (cdr env)))
         (if (eqv? search-var saved-var)
             saved-val
             (apply-env saved-env search-var))))
    (else
     'invalid-env))))

(define extend-env*
  (lambda (vars vals env)
    (null? vars
           '()
           (null? vals
                  '()
                  (extend-env* (cdr vars) (cdr vals) (extend-env (car vars) (car vals) env))))))

(define e
  (extend-env 'd 6
              (extend-env 'y 8
                          (extend-env 'x 7
                                      (extend-env 'y 14
                                                  (empty-env))))))
