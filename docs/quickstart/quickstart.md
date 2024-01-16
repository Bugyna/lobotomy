# Quickstart

## General

### Writing an expression
Lisp inspired syntax with parenthesis denoting the start `(` and end `)` of the expression
and uses polish notation
that means a valid expression looks like this
> `(function-call argument1 "this is text" (+ 2 2))`
this calls a function named `function-call` and passes a variable named `argument1`, a string literal with the value of `"this is text"` and an expression `(+ 2 2)`  
**Arguments get evaluated** so this expressions is transformed into (if we assume the value of `argument1` was 3)
> `(function-call 3 "this is text" 4)`
and after that function `function-call` gets called



### Valid names

An identifier has to start with alphabet characters or one of these special characters `<=?!$%^&*/|>+-` after that it can be followed by any of these aforementioned charcters or a number so basically anything goes and as such `+-=2` is a valid identifier


### Comments

Comments are denoted with two semicolons `;;` and everything after that untill the end of the line is a comment
There are no multiline comments
> ` ;; this is a comment `


## Data types

### Lists

Lists are like expression, but they get evaluated only partially. They are great for passing around chunks of data. They are denoted just like expression, but instead of parenthesis `()` we use square brackets `[]`.
> `[+ 1 2 3]`
>> `[+ 1 2 3]`


### Numbers

Numbers are your typical integers like `1` or perhaps even `33`.
> `2`
>> `2`

### Decimals

Decimals are the floating numbers like `1.213` or `33.002`.
> `3.14`
>> `3.140000`


### Strings

Strings are the text objects like `"word"` or `"This is a sentence."`.
> `"Dog"`
>> `"Dog"`
Strings can be multiline if you put `\` before the newline character.
> `"This is a multiline\  
> string"`
>> `"This is a multiline  
>>string"`


### Functions

This creates a function named hello-name which takes one parameter called `name` and outputs `hello, [value of name]` into the console
> `(fn hello-name (name) (print "hello, " name))`

Calling `hello-name` with value `Mortimer`
> `(hello-name "Mortimer")`
>> `print>> "hello, Mortimer"`



### Variables

To create a variable use the inbuilt function `let` which takes two parameters `name` and `value`
This creates a variable named `girl_boss` that holds a string `"value"`
> `(let girl_boss "value")`
>> `"value"`
You can also pass expressions into the `value` parameter
> `(let var (+ 2 2))`
>> `4`
***To change the value of a variable simply call***
> `(let girl_boss "new value")`
>> `"new value"`

Variables are saved inside scopes.
The default scope is the global one. Functions create their own scope when they are called
you can still access global variables inside of functions as long as they haven't been overwritten by definitions inside the functions.
If you create a variable inside the function which has the same name as a global variable and you try to get the value of this variable the local one (the one you just defined inside the function takes precedence)
In the following example we create a function `local-definition`
> `(let a 2)`
>> `2`
> `(fn local-definition (b) ( (let a (+ b 22)) (+ a 3)) )`  
> `(local-definition a)`
>> `27`
> `(print a)`
>> print>> `2`


## Control flow

### Conditionals

Conditionals are a basic control flow element allowing you to execute a piece of code based on a set condition.
In this example we check if `a` is bigger than `4` and if it isn't add `2` otherwise print `"can't add anymore, number too big"`
> `(let a 2)`  
> `(?`  
>	`&nbsp;&ensp;(< a 4) (let a (+ a 2))`  
>	`&nbsp;&ensp;(else) (print "can't add anymore, number too big")`  
>	`)`  
>> `4`
> `(?`  
>	`&nbsp;&ensp;(< a 4) (let a (+ a 2))`  
>	`&nbsp;&ensp;(else) (print "can't add anymore, number too big")`  
>	`)`  
>> `print>> can't add anymore, number too big`
If we do not add an `else` clause to our condition and none of the other conditions match our return value is #NIL.
> `(let a 8)`
> `(?`  
>	`&nbsp;&ensp;(< a 4) (let a (+ a 2))`  
>	`&nbsp;&ensp;(> a 10) (let a (- a 2))`  
>	`)`  
>> `#NIL`


### Loops

If you want to repeat the same code over and over under a certain condition.
> `(let a 2)`  
> `(loop (< a 10)`  
>	`&nbsp;&ensp;(let a (+ a 2))`  
>	`)`  
>> `10`
If you want to repeat the same code infinitely.
> `(let a 2)`  
> `(loop (1)`  
>	`&nbsp;&ensp;(let a (+ a 2))`  
>	`)`  
This doesn't return anything because it never ends.


