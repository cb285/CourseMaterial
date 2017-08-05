#lang eopl
; 1.15
(define duple
  (lambda (n x)
    (if (eqv? n 0)
        '()
        (cons x (duple (- n 1) x)))))

; 1.16
(define invert
  (lambda (lst)
    (if (null? lst)
        '()
        (cons (list (cadr (car lst)) (car (car lst)))
              (invert (cdr (lst)))))))

; 1.17
(define down
  (lambda (lst)
    (if (null? lst)
        '()
        (cons (list (car lst))
              (down (cdr lst))))))
; 1.18
(define swapper
  (lambda (s1 s2 lst)
    (if (null? lst)
        '()
        (if (eqv? (car lst) s1)
            (cons s2 (swapper s1 s2 (cdr lst)))
            (if (eqv? (car lst) s2)
                (cons s1 (swapper s1 s2 (cdr lst)))
                (cons (car lst) (swapper s1 s2 (cdr lst))))))))

; 1.19
(define list-set
  (lambda (lst n x)
    (if (null? lst)
        '"list too short."
        (if (eqv? n 0)
            (cons x (cdr lst))
            (cons (car lst) (list-set (cdr lst) (- n 1) x))))))

; 1.20
(define co
  (lambda (s slist)
    (if (null? slist)
        '0
        (if (list? (car slist))
            (+ (co s (car slist)) (co s (cdr slist)))
            (if (eqv? (car slist) s)
                (+ 1 (co s (cdr slist)))
                (co s (cdr slist)))))))

; 1.21 product of 2 lists


; 1.22
(define filter-in
  (lambda (pred lst)
    (if (null? lst)
        '()
        (if (pred (car lst))
            (cons (car lst) (filter-in pred (cdr lst)))
            (filter-in pred (cdr lst))))))

; 1.23
(define list-index
  (lambda (pred lst)
    (list-index-helper pred lst 0)))

; Auxilery Function
(define list-index-aux
  (lambda (pred lst i)
    (if (null? lst)
        #f
        (if (pred (car lst))
            i
            (list-index-helper pred (cdr lst) (+ i 1))))))

; 1.36
(define number-elements
  (lambda (lst)
    (if (null? lst)
        '()
        (g (list 0 (car lst)) (number-elements (cdr lst))))))

(define g
  (lambda (t lst)
    (if (null? lst)
        (list t)
        (cons t (g (list (+ 1 (car (car lst))))
                      (car (cdr (car lst)))
              (cdr lst))))))
