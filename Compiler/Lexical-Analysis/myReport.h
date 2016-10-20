#ifndef MYREPORT_H
#define MYREPORT_H

/**
 *	@file	myReport.h
 *	@author	lgxZJ@outlook.com
 *	@date	5/16/2016
 *
 *	@brief	Report module file.
 *
 *			This file contains functions about error reporting used
 *			in lexical analysing.
 *
 *	@note	These functions should be and only be used in lexical
 *			analysing.
 *	@warning	The test cases may not be enough, if some bugs
 *				were found, please contact me!
 */

#include "stdbool.h"

/**	@cond	*/

/**
 *	@brief	A struct used to keep track of the position where lexical
 *			analysis is currently processing.
 */
typedef struct
{
	int	line;	/**<	line num	*/
	int	column;	/**<	column num	*/
}PosInfo;

/**	@endcond	*/

/**
 *	@brief	Reset the tracked position.
 *	@return	This function has no return value.
 *
 *	@note	Since there is rare conditions to reset positions, this
 *			function should be carefully used.
 */
void	resetPos			();

/**
 *	@brief	Manually record the token position in lexical analysing.
 *
 *	@param	newLine		A boolean variable indicates whether there is
 *						a new line.
 *	@param	textLength	The processed token length.
 *	@return	void		This function has no return value.
 *	
 *	@remark	When \a newLine is \b true, the \a textLength argument is
 *			ignored.
 */
void 	recordTokenPos		(bool newLine, int textLength);

/**
 *	@brief	Get the current position info in lexical analysis.
 *
 *	@return	A @ref PosInfo variable containing the current position info.
 */
PosInfo getCurrentPosInfo	();

/**
 *	@brief	Output one token information to the standard output.
 *
 *	@param	tokenNum		The token whose information is outputed.
 *	@param	optionalValue	If the token represents a integer literal,
 *							this variable gives the corresponding integer
 *							value.
 *	@param	optionalText	A variable fetched through \b yytext() which is
 *							a commonly used function provided by \b lex.
 *							For more details, please do search.
 *
 *	@return	If the function succeeds, the return value is the outputed
 *			character count.
 *			If the function fails, the return value is -1.
 *
 *	@note	This function must be only used in this Chapter Because
 *			the tokens in used inside function are hard-coded.
 *	@sa		myTokens.h
 *	@sa		<a href="http://dinosaur.compilertools.net/lex/"> Lex</a>
 */
int 	outputReport		(int tokenNum, int optionalValue, char* optionalText);

/**
 *	@brief	An alternative to \b yyerror() used to output error information
 *			when an error is encountered on lexical analysis.
 *
 *	@return	On success, the total number of characters written is returned.
 *			On failsure, a negative number is returned.
 *
 *	@remark	Users can choose either \b yyerror() or this function to report
 *			error. By default, the report is outputed to the standard
 *			output.
 *	@sa		<a href="http://dinosaur.compilertools.net/lex/"> Lex</a>
 */
int 	errorReport			();

#endif
