A Tiger Compiler
----------------


## What is Tiger?
Written by **Andrew W.Appel**, the author of [***Tiger Book***][tiger-book], the Tiger language is a small language with nested functions, records values with implicit pointers, arraya, integer and string variables, and a few simple structured control constructs.

For more details, see the **Appendix** section of that book.


## Intention
The intention of this project is to write a compiler as a matter of interest. To solidate my own knownledge of both C and C++, i decide to use C to write the front end of this compiler and use C++ to finish the back end.

## Roadmap
This compiler project simply works through steps in tiger book, so there are mainly 11 steps:

1.	Lexical Analysis
2.	Parsing
3.	Abstract Syntax
4.	Semantic Analysis
5.	Activation Records
6.	Translation to Intermediate Code
7.	Basic Blocks and Traces
8.	Instruction Selection
9.	Liveness Analysis
10.	Register Allocation
11.	Putting It All Together


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


## Project Guide
If any, yun can use following guide to do some tasks:

*	To **make**, type `make` in a terminal under the right directory.
*	To **run**, type `make run` in a terminal under the right directory.
*	To **clean**, type `make clean` in a terminal under the right directory.
*	To **show coverage**, type `make show-coverage` in a terminal under the right directory.
*	To **re-test**, type `make retest` in a terminal under the right directory.

## Project Status
 Done with first four steps, currently in **Activation Records**.


[tiger-book]:	http://www.amazon.com/Modern-Compiler-Implementation-Basic-Techniques/dp/0521583896
