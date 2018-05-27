A Tiger Compiler  ![build-status-img]
----------------


## What is Tiger?
Written by **Andrew W.Appel**, the author of [***Tiger Book***][tiger-book], the Tiger language is a small language with nested functions, records values with implicit pointers, arraya, integer and string variables, and a few simple structured control constructs.

For more details, see the **Appendix** section of that book.

## Basic Release
There is a basic release of this compiler under `Compiler` folder. This is a basic release because only the basic functionalities of Tiger is guaranteed.

### To build the compiler
1.  `make`.

Then the generated **`./Compiler/builds/tiger-compiler`** is just the final compiler you want.

### To compile a test file
1.  `cd Compiler`.
2.  `./builds/tiger-compiler ./testcases/testXXX.tig`.

The generated executable file will be under the `testcases` folder with name `testXXX`.

### To see the unit-test coverage result
First make sure that you have installed **firefox**, because the makefile uses it.

1.  `cd Compiler/Coverage-Report`.
2.  `make`.


## Folder Categories
*	`Compiler` : The real project folder, every file in this folder is sorted into different categories.
*	`Chapter xxx` : Each folder named with convention corresponding to one chapter in tiger book.
*	`doc` : The documentation folder.
*	`testcases` : Folders containing source files written in tiger language, which is intended for tests.
*	`tests` or `xxx_Test` : Folders containing unit tests.
*	`bin` : Folders containing object files.
*	`coverage` : Folders containing code coverage reports.
*	`example` : Folders containing examples.


## Project Needed
*	**`Lex`**, a computer program that generates lexical analyzers.
*	**`Yacc`**, a LALR parser generator.
*	**`CUnit`**, a unit test framework for C.
*	**`LCOV`**,  a graphical front-end for GCC's coverage testing tool gcov.
*	**`GCC`**, the GNU Compiler Collection.
*	**`Doxygen`**, a documentation generator.
* **`CppUnit`**.
* **`32bit and 64bit C libraries`**.
* **`GnuMake`**.
* **`G++`**.

## Todo
See `Compiler/TODO`.


[tiger-book]:		http://www.amazon.com/Modern-Compiler-Implementation-Basic-Techniques/dp/0521583896
[build-status-img]:	https://travis-ci.org/lgxZJ/Tiger-Compiler.svg?branch=master
