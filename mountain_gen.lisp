(defpackage #:mountain_gen
  (:use :common-lisp :alexandria :trivia :iterate :defclass-std)
  (:export :prova-fun :main :*grid* :grad :fade :perlin))

(in-package #:mountain_gen)

(defconstant +grid-height+ 520)
(defconstant +grid-width+ 520)

(defconstant +zoom+ 0.05)
(defconstant +octaves+ 4)
(defconstant +initial-amplitude+ 1.0)
(defconstant +initial-frequency+ 1.0)
(defconstant +lacunarity+ 2.0)
(defconstant +gain+ 0.5)

; Perlin Noise
(defconstant +permutation+
  (let* ((arr-n (alexandria:iota 256))
         (arr-n-shuffled (shuffle arr-n)))
    (progn
      (append arr-n-shuffled arr-n-shuffled))))

(defun grad (hash x y)
  (let ((h (logand hash)))
    (match h
      (0 (+ x 0))
      (1 (+ (* x (- 0 1) 0)))
      (2 (+ 0 y))
      (3 (- 0 y)))))

(defun fade (x)
  (let ((five (* 6 (expt x 5)))
        (four (* 15 (expt x 4)))
        (three (* 10 (expt x 3))))
    (+ (- five four) three)))

(defun perlin (x y)
  (let* (
         (xi (logand (truncate x) 255))
         (yi (logand (truncate y) 255))
         (xf (- x (truncate x)))
         (yf (- y (truncate y)))
         (u (fade xf))
         (v (fade yf))
         (aa (nth (+ (nth xi +permutation+) yi) +permutation+))
         (ab (nth (+ (nth xi +permutation+) yi 1) +permutation+))
         (ba (nth (+ (nth (+ xi 1) +permutation+) yi) +permutation+))
         (bb (nth (+ (nth (+ xi 1) +permutation+) yi 1) +permutation+))
         (dot_aa (grad aa xf yf))
         (dot_ba (grad ba (- xf 1) yf))
         (dot_ab (grad ab xf (- yf 1)))
         (dot_bb (grad bb (- xf 1) (- yf 1)))
         (x1 (lerp u dot_aa dot_ba))
         (x2 (lerp u dot_ab dot_bb)))
    (lerp x1 x2 v)))

(defstruct point
  (x 0.0 :type float)
  (y 0.0 :type float)
  (z 0.0 :type float))

(defparameter *grid*
  (make-array `(,+grid-height+ ,+grid-width+) :initial-element (make-point)))

(defun prova-fun ()
  (print "Hello, World"))

(defun main ()
  ;(nth 1 +permutation+))
  (perlin 2.3 6.7))

