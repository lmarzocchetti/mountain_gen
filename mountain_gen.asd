(require "asdf")

(ql:quickload :alexandria)
(ql:quickload :trivia)
(ql:quickload :iterate)
(ql:quickload :defclass-std)

(asdf:defsystem "mountain_gen"
  :depends-on ("alexandria" "trivia" "iterate" "defclass-std")
  :components ((:file "mountain_gen"))
  :build-operation "program-op"
  :build-pathname "mountain_gen"
  :entry-point "mountain_gen::main")
