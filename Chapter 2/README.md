# A Lexical Analyzer for Tiger Language (Chapter 2 in Tiger book) 
-----------------------------------------------------------------
This program uses [Lex][lex_link] to generate a lexical analyzer.


## Files
### README
*	**README.md**

### Lex files
*	**myTiger.lex** : The lex file to generate a lexical analyzer.

### Headers
*	**myTokens.h** : The header file containing tokens.
*	**makeMemory.h** : Header file.
*	**stringEscape.h** : Header file.
*	**myReport.h** : Header file.

### Sources
*	**makeMemory.c** : Functions about memory allocating.
*	**stringEscape.c** : Functions about escape characters' translation.
*	**myReport.c** : Functions about lexical analysis reporting.
*	**myDriver.c** : The scaffold to call the lexical analyzer.

### Others
*	**doxygen_doc.conf** : The configure file to generate doxygen documentation.
*	**makefile** : The makefile to build executables or objects.


## Folders
### Documentation
*	**doc** : The documentation folder.

### Tests
*	**makeMemory_Test** : Tests of **makeMemory.c**.
*	**stringEscape_Test** : Tests of **stringEscape.c**.
*	**testcases** : Files written in ***Tiger Language*** which can be tested.


## Program Usage

*		With no arguments : Analyze all files in **testcases** folder.
*		With One filename argument : Analyze this file.


## Note:
**testcase** is a folder containing all simple test cases. File *'string.tig'*
is manually written for string testing; Other files are simple source file
 written in tiger language in spite of correctness or incorrectness.


[lex_link]:http://dinosaur.compilertools.net/lex/