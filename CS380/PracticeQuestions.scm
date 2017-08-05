; Practice Programming Questions September 23, 2016
#lang eopl

; 1.
; Given a list of integers, write a function that returns the sum of these integers.
; We define the sum of the empty list ot be 0.

(define sum
  (lambda (lst)
    (if (null? lst)
        0
        (+ (car lst) (sum (cdr lst))))))

; 2.
; Given a list of integers lst, write a function that returns a list with entries length(lst) down to 1.
; For example:
; Given: (10 20 30 40 50)
; Returns: (5 4 3 2 1)

(define length-list
  (lambda (lst)
    (if (null? lst)
        '()
        (cons (length lst 0) (length-list (cdr lst))))))

(define length
  (lambda (lst n)
    (if (null? lst)
        n
        (length (cdr lst) (+ n 1)))))

; 3.
; Consider the grammer tht models sumtraction in Polish notation:
; plist := (pexp)
; pexp  := Int
;       := - pexp pexp

; 3a.
; Create the abstract syntax tree for plist and pexp using the define-datatype macro.
(define-datatype plist plist?
  (pexp-plist
   (pexp pexp?)))

(define-datatype pexp pexp?
  (const-exp
   (n integer?))
  (diff-exp
   (exp1 pexp?)
   (exp2 pexp?)))

; 3b.
; Using the constructors from the define-datatype of part (a) write an expression that creates
; the AST for (- - - 5 3 - 2 2 10)
(define 3bexample
  (pexp-plist (diff-exp
               (diff-exp
                (diff-exp (const-exp 5) (const-exp 3))
                (diff-exp (const-exp 2) (const-exp 2)))
               (const-exp 10))))

; 3c.
; Write a value-of procedure for plist. This procedule takes one parameter: the AST of some plist and returns the value.
; For example, the value of the plist in part (b) is -8
(define value-of-plist
  (lambda (plst)
    (cases plist plst
      (pexp-plist (exp)
        (value-of-pexp exp)))))

(define value-of-pexp
  (lambda (exp)
    (cases pexp exp
      (const-exp (num)
                 num)
      (diff-exp (exp1 exp2)
                (- (value-of-pexp exp1) (value-of-pexp exp2))))))

; Practice Questions October 17, 2016

; 1.
; Implement (duple n x) that returns a list containing n copies of x.
; For example:
; (duple 2 3) => (3 3)
(define duple
  (lambda (n x)
    (if (zero? n)
        '()
        (cons x (duple (- n 1) x)))))

; 2.
; Implement (every? pred lst) that returns #f if any element of lst
; fails to satisfy pred and return #t otherwise
;
; For example:
; (every? number? '(a b 3)) => #f
; (every? number? '(1 2 3)) => #t
(define every?
  (lambda (pred lst)
    (if (null? lst)
        #t
        (if (pred (car lst))
              (every? pred (cdr lst))
              #f))))

; 3.
; Here is a recursive definition of a binary tree whose leafs are integers and
; internal nodes are symbols - the variants are given names after the production rule:
;
; bintree := <integer>
;            leaf-node (int)
;         := <symbol> <bintree> <bintree>
;            interior-node (sym lefttree righttree)

; 3a.
; Represent the binary tree using define-datatype
(define-datatype bintree bintree?
  (leaf-node
   (n integer?))
  (interior-node
   (sym symbol?)
   (lefttree bintree?)
   (righttree bintree?)))

; 3b.
; Implement a bintree-to-list procedure for the binary tree.
; So that (bintree-to-list << a b 3 5 4 >>) => (a (b 3 5) 4)
(define bintree-to-list
  (lambda (tree)
    (cases bintree tree
      (leaf-node (n)
                 n)
      (interior-node (sym lefttree righttree)
                     (list sym (bintree-to-list lefttree) (bintree-to-list righttree))))))

(define bintree-example
  (interior-node 'a (interior-node 'b (leaf-node 3) (leaf-node 5)) (leaf-node 4)))
