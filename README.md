\# lobotomy
programming langauge
syntax inspired by lisp


comments
;; a comment

debug comments
show the comment text during evaluation
\# a debug comment


syntax
uses polish notation
so for arithmetics and functions and all else you do
(hi-bestie argument "aaa") -> calls function named hi-bestie and passes variables named argument and a literal string "aaa"


valid names
an identifier has to start with alphabet characters or <=@?!$%^&*/|>+-' and can be followed by number
so basically anything goes and as such '+-=2' is a valid identifier


functions
(fn hello-name (name) (print "hello, " name)) -> creates a function named hello-name which takes one parameter
to call
(hello-name "girlie") -> outputs "hello, girlie"



variables
to create a variable
(let girl_boss "value") -> creates a variable named girl_boss that holds a string "value"

to change the value of a variable simply call
(let girl_boss "new value")

variables are saved inside scopes
the default scope is the global one and functions create their own scope when called
you can still access global variables inside of functions as long as they haven't been overwritten (named the same way as the funciton parameters)


