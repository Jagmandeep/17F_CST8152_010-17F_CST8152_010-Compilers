
/*File name :	[table.h]
Compiler :		[MS Visual Studio 2015]
Author :		[Jagmandeep Singh, ID#]
Course :		CST 8152 – Compilers, Lab Section : [13]
Assignment :	[ 1 ]
Date :			[30-11-2017]
Professor :		Sv.Ranev
Purpose :		[Transition Table and function declarations necessary for the scanner implementation
				 as required for CST8152 - Assignment #2.]
Function list : [ st_table(),
				as_table(),
				aa_func02();
				aa_func03(),
				aa_func05(),
				aa_func08(),
				aa_func11(),
				aa_func12(),
				kw_table()
*/
#ifndef  TABLE_H_
#define  TABLE_H_ 

#ifndef BUFFER_H_
#include "buffer.h"
#endif

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*   Source end-of-file (SEOF) sentinel symbol
*    '\0' or only one of the folowing constants: 255, 0xFF , EOF
*/

/*  Single-lexeme tokens processed separately one by one
*  in the token-driven part of the scanner
*  '=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' ,
*       space
*  !!comment , ',' , '"' , ';' , '-' , '+' , '*' , '/', '#' ,
*  .AND., .OR. , SEOF, 'wrong symbol',
*/


#define IS -1    /* Inavalid state */
#define ES 12 /* Error state */
#define ER 13 /* Error retract */
#define MAX 32767 
/* State transition table definition */

/* NUMBER of COLUMNS  */

#define TABLE_COLUMNS 9
/*transition table - type of states defined in separate table */
int  st_table[][TABLE_COLUMNS] = {
	/* State 0 */{ 1 , 1 , 1 , 1 , 6 , 4 , ES , ES , ES },
	/* State 1 */{ 1 , 1 , 1 , 1 , 1 , 1 , 2 , 3 , 2 },
	/* State 2 */{ IS , IS , IS , IS , IS , IS , IS , IS , IS },
	/* State 3 */{ IS , IS , IS , IS , IS , IS , IS , IS , IS },
	/* State 4 */{ ES , ES , ES , ES , 4 , 4 , 7 , 5 , 5 },
	/* State 5 */{ IS , IS , IS , IS , IS , IS , IS , IS , IS },
	/* State 6 */{ 9 , ES , ES , ES , 5 , ES , 7 , ER , 5 },
	/* State 7 */{ 8 , 8 , 8 , 8 , 7 , 7 , 8 , 8 , 8 },
	/* State 8 */{ IS , IS , IS , IS , IS , IS , IS , IS , IS },
	/* State 9 */{ ER , ER , 10 , ER , 10 , 10 , ER , ER , ER },
	/* State 10 */{ ES , ES , 10 , ES , 10 , 10 , 11 , 11 , 11 },
	/* State 11 */{ IS , IS , IS , IS , IS , IS , IS , IS , IS },
	/* State 12 */{ IS , IS , IS , IS , IS , IS , IS , IS , IS },
	/* State 13 */{ IS , IS , IS , IS , IS , IS , IS , IS , IS },
};
/* Accepting state table definition
REPLACE *N1*, *N2*, and *N3* WITH YOUR NUMBERS
*/
#define ASWR     21  /* accepting state with retract */
#define ASNR     22  /* accepting state with no retract */
#define NOAS     23  /* not accepting state */

int as_table[] = { NOAS , NOAS , ASWR , ASNR , NOAS , ASWR , NOAS , NOAS , ASWR , NOAS , NOAS , ASWR , ASNR , ASWR };

/* Accepting action function declarations

FOR EACH OF YOUR ACCEPTING STATES YOU MUST PROVIDE
ONE FUNCTION PROTOTYPE.THEY ALL RETURN Token AND TAKE
ONE ARGUMENT : A string REPRESENTING A TOKEN LEXEME.
*/

Token aa_func02(char *lexeme);		/*AVID*/ 
Token aa_func03(char *lexeme);		/*SVID*/
Token aa_func05(char *lexeme);		/*DIL*/
Token aa_func08(char *lexeme);		/*FPL*/
Token aa_func11(char *lexeme);		/*HIL*/
Token aa_func12(char *lexeme);		/*ES*/
/*Token aa_func13(char *lexeme);*/		/*ER*/

									/* defining a new type: pointer to function (of one char * argument)
									returning Token
									*/

typedef Token(*PTR_AAF)(char *lexeme);

/* Accepting function (action) callback table (array) definition */
/* If you do not want to use the typedef, the equvalent declaration is:
* Token (*aa_table[])(char lexeme[]) = {
*/

PTR_AAF aa_table[] = {

	NULL , NULL , aa_func02 , aa_func03 , NULL , aa_func05 , NULL , NULL , aa_func08 , NULL , NULL , aa_func11 , aa_func12 ,aa_func12

	/*HERE YOU MUST PROVIDE AN INITIALIZATION FOR AN ARRAY OF POINTERS
	TO ACCEPTING FUNCTIONS.THE ARRAY HAS THE SAME SIZE AS as_table[].
	YOU MUST INITIALIZE THE ARRAY ELEMENTS WITH THE CORRESPONDING
	ACCEPTING FUNCTIONS(FOR THE STATES MARKED AS ACCEPTING IN as_table[]).
	THE REST OF THE ELEMENTS MUST BE SET TO NULL.*/

};

/* Keyword lookup table (.AND. and .OR. are not keywords) */

#define KWT_SIZE  10

char * kw_table[KWT_SIZE] =
{
	"ELSE",
	"FALSE",
	"IF",
	"PLATYPUS",
	"READ",
	"REPEAT",
	"THEN",
	"TRUE",
	"WHILE",
	"WRITE"
};

#endif
