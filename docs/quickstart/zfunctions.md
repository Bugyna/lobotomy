# Available functions

## Numerical operations

### +

> `(+ ...)`
- takes unlimited number of arguments and adds them together


### -

> `(- ...)`
- takes unlimited number of arguments and substracts them


### *

> `(* ...)`
- takes unlimited number of arguments and multiplies them


### /
> `(/ ...)`
- takes unlimited number of arguments and divides them

### %
> `(/ a b)`
- takes 2 values and returns `a` mod `b`
- if `b` is `0` the program will crash with `floating point exception`


## Binary

### &
> `(& ...)`
- takes unlimited number of arguments and does a logical **and** on them


### |
> `(| ...)`
- takes unlimited number of arguments and does a logical **or** on them

### xor
> `(xor ...)`
- takes unlimited number of arguments and does a logical **xor** on them


## Control flow

### ?
> `(? (cond-expr) (execute something))`
> `(? (other-cond-expr) (execute something-else))`
- takes pairs of expressions, one condition expression and one expression that gets executed if the condition is true.

### loop
> `(loop (cond-expr) (execute something))`
- takes a pair of expressions, one condition expression and one expression that gets executed as long as the condition is true.


## Comparisons



### <
> `(< a b)`
- takes a pair of numbers and compares if `a` is less than `b`

### >
> `(> a b)`
- takes a pair of numbers and compares if `a` is greater than `b`

### <=
> `(<= a b)`
- takes a pair of numbers and compares if `a` is less than or equal to `b`

### >=
> `(>= a b)`
- takes a pair of numbers and compares if `a` is greater than or equal to `b`

### =
> `(= a b)`
- takes a pair of values(can be string or list) and compares if `a` is equal to `b`


## IO

### print
> `(print ...)`
- takes an unlimited number of values and prints them to the console


### input
> `(input ["conditional text to print"])`
- takes optional string value and prints it to the console, then waits for input from the console


## General

### exit
> `(exit)`
- exits the execution of the whole program

### help
> `(help)`
- prints all named objects stored in the global scope

### type
> `(type val)`
- returns the string name of the type of `val`


### let
> `(let var val)`
- stores the val under the name of var in the current scope

### fn
> `(fn fn-name ([args]) (expr))`
- Creates a function named `fn-name` taking args

> `(load name)`
- Loads a `.so` C binary file


## Random

### random-num
> `(random-num [num])`
- Returns a random number, takes an optional argument num that sets the max value that can be returned




