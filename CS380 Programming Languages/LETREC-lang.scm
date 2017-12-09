; Clayton Brutus
; CS380
; Dr. Robert Morse
; letrec interpreter implimentation
; with addons: add-exp, mult-exp, div-exp
; examples of letrec usage: double and sum.

#lang eopl

; environment datatype definition
(define-datatype environment environment?
  (empty-env)
  (extend-env
   (var symbol?)
   (val expval?)
   (saved-env environment?))
  (extend-env-rec
   (p-name symbol?)
   (b-var symbol?)
   (body expression?)
   (env environment?)))

; function: apply-env
(define apply-env
  (lambda (env search-var)
    (cases environment env
      (empty-env () #f)
      (extend-env (var val saved-env) 
        (if (eq? var search-var) val
            (apply-env saved-env search-var)))
      (extend-env-rec (p-name b-var p-body saved-env)
                      (if (eqv? search-var p-name)
                          (proc-val (procedure b-var p-body env))
                          (apply-env saved-env search-var))))))

; datatype: program
(define-datatype program program?
  (a-program
   (exp1 expression?)))

; datatype: expression
(define-datatype expression expression?
  (const-exp
   (num number?))
  (diff-exp
   (exp1 expression?)
   (exp2 expression?))
  (add-exp
   (exp1 expression?)
   (exp2 expression?))
  (mult-exp
   (exp1 expression?)
   (exp2 expression?))
  (div-exp
   (exp1 expression?)
   (exp2 expression?))
  (zero?-exp
   (exp1 expression?))
  (if-exp
   (exp1 expression?)
   (exp2 expression?)
   (exp3 expression?))
  (var-exp
   (var symbol?))
  (let-exp
   (var symbol?)
   (exp1 expression?)
   (body expression?))
  (proc-exp
   (var symbol?)
   (body expression?))
  (call-exp
   (rator expression?)
   (rand expression?))
  (letrec-exp
   (p-name symbol?)
   (b-var symbol?)
   (p-body expression?)
   (letrec-body expression?)))

; datatype: expval
(define-datatype expval expval?
  (num-val
   (num number?))
  (bool-val
   (bool boolean?))
  (proc-val
   (proc proc?)))

; function: expval->num
(define expval->num
  (lambda (val)
    (cases expval val
      (num-val (num) num)
      (else ('report-extractor-error)))))

; function: expval->bool
(define expval->bool
  (lambda (val)
    (cases expval val
      (bool-val (bool) bool)
      (else ('report-extractor-error)))))

; function: expval->proc
(define expval->proc
  (lambda (val)
    (cases expval val
      (proc-val (proc)
                proc)
      (else 'invalid-procedure))))

; function: value-of-program
(define value-of-program
  (lambda (pgm)
    (cases program pgm
      (a-program (exp1)
                 (value-of exp1 init-env)))))

; function: value-of
(define value-of
  (lambda (exp env)
    (cases expression exp
      (const-exp (num)
                 (num-val num))
      (var-exp (var) (apply-env env var))
      (diff-exp (exp1 exp2)
                (let ((val1 (value-of exp1 env))
                      (val2 (value-of exp2 env)))
                  (let ((num1 (expval->num val1))
                        (num2 (expval->num val2)))
                    (num-val (- num1 num2)))))
      (zero?-exp (exp1)
                 (let ((val1 (value-of exp1 env)))
                   (let ((num1 (expval->num val1)))
                     (if (zero? num1)
                         (bool-val #t)
                         (bool-val #f)))))
      (if-exp (exp1 exp2 exp3)
              (let ((val1 (value-of exp1 env)))
                (if (expval->bool val1)
                    (value-of exp2 env)
                    (value-of exp3 env))))
      (let-exp (var exp1 body)
               (let ((val1 (value-of exp1 env)))
                 (value-of body
                           (extend-env var val1 env))))
      (proc-exp (var body)
                (proc-val (procedure var body env)))
      (call-exp (rator rand)
                (let ((proc (expval->proc (value-of rator env)))
                      (arg (value-of rand env)))
                  (apply-procedure proc arg)))
      (letrec-exp (proc-name bound-var proc-body letrec-body)
                  (value-of letrec-body (extend-env-rec proc-name bound-var proc-body env)))
      (add-exp (exp1 exp2)
               (value-of (diff-exp exp1 (diff-exp (const-exp 0) exp2)) env))
      (mult-exp (exp1 exp2)
                (let ((val1 (value-of exp1 env))
                      (val2 (value-of exp2 env)))
                  (let ((num1 (expval->num val1))
                        (num2 (expval->num val2)))
                    (num-val (* num1 num2)))))
      (div-exp (exp1 exp2)
                (let ((val1 (value-of exp1 env))
                      (val2 (value-of exp2 env)))
                  (let ((num1 (expval->num val1))
                        (num2 (expval->num val2)))
                    (num-val (/ num1 num2))))))))

; define default environment (init-env)
(define init-env
    (extend-env 'i (num-val 1)
                (extend-env 'v (num-val 5)
                            (extend-env 'x (num-val 10)
                                        (empty-env)))))

; datatype: proc
(define-datatype proc proc?
  (procedure
   (var symbol?)
   (body expression?)
   (saved-env environment?)))

; function: apply-procedure
(define apply-procedure
  (lambda (proc1 val)
    (cases proc proc1
      (procedure (var body saved-env)
                 (value-of body (extend-env var val saved-env))))))

; multiplies by 2
(define double-test
  (letrec-exp 'double 'x (if-exp (zero?-exp (var-exp 'x))
                                 (const-exp 0)
                                 (diff-exp (call-exp (var-exp 'double)
                                                     (diff-exp (var-exp 'x)
                                                               (const-exp 1)))
                                           (const-exp -2)))
              (call-exp (var-exp 'double) (const-exp 3))))

; sums x with all numbers < x.
(define sum-test
  (letrec-exp 'sum 'x (if-exp (zero?-exp (var-exp 'x))
                              (const-exp 0)
                              (diff-exp (call-exp (var-exp 'sum)
                                                  (diff-exp (var-exp 'x) (const-exp 1)))
                                        (diff-exp (const-exp 0) (var-exp 'x))))
              (call-exp (var-exp 'sum) (const-exp 4))))
