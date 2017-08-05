; Clayton Brutus
; CS380 Final Exam Problem 16
#lang eopl

; a.
(define-datatype bintree bintree?
  (empty-tree)
  (leaf)
  (branch
   (tree1 bintree?)
   (tree2 bintree?)))

; b.
(define height
  (lambda (T)
    (cases bintree T
      (empty-tree ()
       -1)
      (leaf ()
       1)
      (branch (t1 t2)
              (+ 1 (max (height t1) (height t2))))
      (else
       'error))))

; c.
(define num-leaves
  (lambda (T)
    (cases bintree T
      (empty-tree ()
                  0)
      (leaf ()
            1)
      (branch (t1 t2)
              (+ (num-leaves t1) (num-leaves t2))))))

; d.
(define bt-parser
  (lambda (t)
    (if (null? t)
        'error
        (cond
          ((eqv? t 'empty-tree)
           (empty-tree))
          ((eqv? t 'leaf)
           (leaf))
          ((and (list? t) (null? (cdr t)))
           (bt-parser (car t)))
          ((list? t)
           (branch (bt-parser (car t)) (bt-parser (cdr t))))))))
