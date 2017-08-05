; Clayton Brutus
; Midterm
; CS380 - Programming Languages

#lang eopl

; 1a.
(define-datatype polynomial polynomial?
  (single-poly
   (n integer?)
   (e natural?))
  (multi-poly
   (n integer?)
   (e natural?)
   (poly polynomial?)))

; 1b.
(define construct-poly
  (lambda (polylst)
    (if (null? polylst)
        (single-poly 0 0)
        (multi-poly (car (car polylst)) (car (cdr (car polylst))) (construct-poly (cdr polylst))))))

(define natural?
  (lambda (n)
    (if (integer? n)
        (if (>= n 0)
            #t
            #f)
         #f)))

; 2.
(define degree
  (lambda (poly)
    (degree-aux poly 0)))

(define degree-aux
  (lambda (poly mx)
    (cases polynomial poly
      (single-poly (n e)
                   (if (> e mx)
                       e
                       mx))
      (multi-poly (n e nxtpoly)
                  (if (> e mx)
                      (degree-aux nxtpoly e)
                      (degree-aux nxtpoly mx))))))

; 3.
(define eval-poly
  (lambda (poly x)
    (cases polynomial poly
      (single-poly (n e)
                   (* n (expt x e)))
      (multi-poly (n e nxtpoly)
                  (+ (* n (expt x e)) (eval-poly nxtpoly x))))))

; 4.
(define add-poly
  (lambda (poly1 poly2)
    (cases polynomial poly1
      (single-poly (n e)
                   (multi-poly n e poly2))
      (multi-poly (n e nxtpoly)
                  (add-poly (single-poly n e) (add-poly nxtpoly poly2))))))

; 5.
(define add-constant-poly
  (lambda (P)
    (lambda (P2)
      (add-poly P P2))))

; 6.
(define AddN
  (lambda (N)
    (lambda (x)
      (+ x N))))
