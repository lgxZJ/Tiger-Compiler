# Abstract Syntax Tree (Chapter 4 in Tiger book)
------------------------------------------------
This program simply parses Tiger source files and prints abstract syntax expressions into separate result files in `.txt` format.

One file named `SingleParseResult.txt` is generated when program was ran with a filename.

A set of files inside **outputTxts** folder is generated when program was ran with no argument.


## Files
### README
*	**README.md**


### Lex files
*	**myTiger.lex** : The lex file to generate a lexical analyzer.


### Yacc files
*	**myTiger.y** : The yacc file to generate a syntax parser.


### Headers
See documentation.

*	**lexString.h**
*	**makeMemory.h**
*	**myAbstractSyntax.h**
*	**myPrintAbstractSyntax.h**
*	**myReport.h**
*	**mySymbol.h**
*	**myTable.h**


### Sources
See documentation.

*	**lexString.c** : 
*	**makeMemory.c** : 
*	**myAbstractSyntax.c** : 
*	**myPrintAbstractSyntax.c** : 
*	**myReport.c** : 
*	**mySymbol.c** : 
*	**myTable.c** : 


### Others
*	**doxygen_doc.conf** : Configure file to be used by doxygen to generate documentation.
*	**SingleParseResult.txt** : The result file genreated when the program runs with one single filename argument.
*	**Makefile** : The make file to build programs.


## Folders
### Documentation
*	**doc** : See index.html


### Tests
*	**lexString_Test**
*	**makeMemory_Test**
*	**myAbstractSyntax_Test**
*	**myPrintAbstractSyntax_Test**
*	**myReport_Test**
*	**mySymbol_Test**
*	**myTable_Test**
*	**stringEscape_Test**
*	**testcases** : Folder containing source files written in Tiger language which are to be tested.

### Program results
*	**outputTxts** : Folder containing files generated when the program runs with no arguments.

## Usage guide
### Run
*	To parse one file, run with the command format `./*.out filename`.
*	To parse files inside **testcases** folder, run with the command format `./*out`


### Make
*	To make, type `make`.
*	To clean, type `make clean`.