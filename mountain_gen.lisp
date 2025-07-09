(defpackage #:mountain_gen
  (:use :common-lisp :alexandria :trivia :iterate :defclass-std)
  (:export :prova-fun :main :*grid*))

(in-package #:mountain_gen)

(defconstant *grid-height* 520)
(defconstant *grid-width* 520)

(defconstant +zoom+ 0.05)
(defconstant +octaves+ 4)
(defconstant +initial-amplitude+ 1.0)
(defconstant +initial-frequency+ 1.0)
(defconstant +lacunarity+ 2.0)
(defconstant +gain+ 0.5)
p
(defstruct point
  (x 0.0 :type float)
  (y 0.0 :type float)
  (z 0.0 :type float))

(defparameter *grid*
  (make-array `(,*grid-height* ,*grid-width*) :initial-element (make-point)))

(defun prova-fun ()
  (print "Hello, World"))

(defun main ()
  (prova-fun))
