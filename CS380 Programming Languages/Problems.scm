; problems 3.6 - 3.8

#lang eopl

; environment implementation
(define empty-env
  (lambda ()
    (lambda (search-var)
      (report-no-binding-found search-var))))

(define extend-env
    (lambda (saved-var saved-val saved-env)
      (lambda (search-var)
        (if (eqv? search-var saved-var)
            saved-val
            (apply-env saved-env search-var)))))

(define apply-env
  (lambda (env search-var)
    (env search-var)))

(define report-no-binding-found
  (lambda (search-var)
    (eopl:error 'apply-env "No binding for ~s" search-var)))

(define report-invalid-env
  (lambda (env)
    (eopl:error 'apply-env "Bad environment: ~s" env)))
; end environment implementation

; LET language implementation
(define-datatype program program?
  (a-program
   (exp1 expression?)))

(define identifier? symbol?)

(define-datatype expression expression?
  (const-exp
   (num number?))
  (diff-exp 
   (exp1 expression?)
   (exp2 expression?))
  (minus-exp
   (exp1 expression?))
  (add-exp
   (n1 number?)
   (n2 number?))
  (mult-exp
   (exp1 expression?)
   (exp2 expression?))
  (div-exp
   (exp1 expression?)
   (exp2 expression?))
  (eqv?-exp
   (exp1 expression?)
   (exp2 expression?))
  (greater?-exp
   (exp1 expression?)
   (exp2 expression?))
  (less?-exp
   (exp1 expression?)
   (exp2 expression?))
  (zero?-exp
   (exp1 expression?))
  (if-exp
   (exp1 expression?)
   (exp2 expression?)
   (exp3 expression?))
  (var-exp
   (var identifier?))
  (let-exp
   (var identifier?)
   (exp1 expression?)
   (body expression?)))

(define init-env
  (lambda ()
    (extend-env
     'i (num-val 1)
     (extend-env
      'v (num-val 5)
      (extend-env
       'x (num-val 10)
       (empty-env))))))

(define-datatype expval expval?
  (num-val
   (num number?))
  (bool-val
   (bool boolean?)))

(define expval->num
  (lambda (val)
    (cases expval val
      (num-val (num) num)
      (else (report-expval-extractor-error 'num val)))))

(define expval->bool
  (lambda (val)
    (cases expval val
      (bool-val (bool) bool)
      (else (report-expval-extractor-error 'bool val)))))

(define report-expval-extractor-error
  (lambda (type val)
    (eopl:error 'exp-val "Error extracting ~s from ~s" type val)))

(define run
  (lambda (pgm)
          (value-of-program (pgm))))

(define value-of-program
  (lambda (pgm)
    (cases program pgm
      (a-program (exp1)
                 (value-of exp1 (init-env))))))

(define value-of
  (lambda (exp env)
    (cases expression exp
      (const-exp (num) (num-val num))
      (var-exp (var) (apply-env env var))
      (diff-exp (exp1 exp2)
                (let ((val1 (value-of exp1 env))
                      (val2 (value-of exp2 env)))
                  (let ((num1 (expval->num val1))
                        (num2 (expval->num val2)))
                    (num-val
                     (- num1 num2)))))
      (minus-exp (exp1)
                 (let (val1 (value-of exp1 env)))
                 (num-val (- 0 (expval->num val1))))
      (add-exp (exp1 exp2)
               (let ((val1 (value-of exp1 env))
                     (val2 (value-of exp2 env))))
               (let ((num1 (expval->num val1))
                     (num2 (expval->num val2))))
               (num-val (+ num1 num2)))
      (mult-exp (exp1 exp2)
                (let ((val1 (value-of exp1 env))
                     (val2 (value-of exp2 env))))
               (let ((num1 (expval->num val1))
                     (num2 (expval->num val2))))
               (num-val (* num1 num2)))
      (div-exp (exp1 exp2)
               (let ((val1 (value-of exp1 env))
                     (val2 (value-of exp2 env))))
               (let ((num1 (expval->num val1))
                     (num2 (expval->num val2))))
               (num-val (/ num1 num2)))
      (eqv?-exp (exp1 exp2)
                (let ((val1 (value-of exp1 env))
                     (val2 (value-of exp2 env))))
               (let ((num1 (expval->num val1))
                     (num2 (expval->num val2))))
               (bool-val (eqv? num1 num2)))
      (greater?-exp (exp1 exp2)
                    (let ((val1 (value-of exp1 env))
                     (val2 (value-of exp2 env))))
               (let ((num1 (expval->num val1))
                     (num2 (expval->num val2))))
               (bool-val (positive? (- num1 num2))))
      (less?-exp (exp1 exp2)
                 (let ((val1 (value-of exp1 env))
                     (val2 (value-of exp2 env))))
               (let ((num1 (expval->num val1))
                     (num2 (expval->num val2))))
               (bool-val (positive? (- num2 num1))))
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
                           (extend-env var val1 env)))))))
