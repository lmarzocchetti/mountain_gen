(pushnew (uiop:getcwd) ql:*local-project-directories*)
(ql:quickload :mountain_gen)
(asdf:make :mountain_gen)
