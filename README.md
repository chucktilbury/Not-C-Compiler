# Not-C-Compiler
LLVM based compiler for a language that is similar to a subset of C

This subset of C does not include:
* typedef
* assignment operators other than ```=```
* switch/case
* for() loops
* numerical types other than int, uint, and float (all 64 bits)
* the ```->``` operator. Uses the ```.``` instead.
* A preprocessor. Comments and includes are handled by the parser or scanner as approprate. Macros are not supported.
* The ```include``` keyword is replaced with the ```import``` keyword, with the implications being that a namespace is being brought into the current scope.
* Storage classes.

Features that are different from C:
* All blocks must be inclosed with ```{}```, including if/else.
* The else clause uses an expression as ```else(1==1){}``` instead of ```else if(1==1){}```
* A blank expression is taken to be true such as ```while(){}``` is equivalent to ```while(1){}```
* The type name of ```string``` is supported. Formatting a string is a part of the language, similar to Python.
* Boolean values are supported.
* Nested function definitions are legal.
* Iterators are implemented similar to Ruby with the ```yield``` keyword.
* ```choose/item``` replaces switch/case. Syntax looks like ```choose(expr){item(const){} ... default{}}```. A const can also be a string.

Features that are planned but not supported yet.
* When a function is declated inside a struct, the scope of the other items defined in the struct will appear as global or local to that function.
* Data structures can be concatenated to make a larger structure. Name conflicts are handled with the ```.``` operator.
* Function decorators, similar to Python.
* Lists and dictionaries, similar to Python.
* Operator overloading for structures. 
* Exception handling.
