/*File name :	[buffer.h]
Compiler :		[MS Visual Studio 2015]
Author :		[Jagmandeep Singh, ID# ]
Course :		CST 8152 – Compilers, Lab Section : [13]
Assignment :	[ 1 ]
Date :			[29-9-2017]
Professor :		Sv.Ranev
Purpose :		[This file is for defining macros,type defination and function prototypes ]
Function list :	RT_FAIL1
				RT_FAIL2
				LOAD_FAIL
				SET_R_FLAG
				RESET_R_FLAG
				FIXED
				ADDITIVE
				MULTIPLICATIVE
*/

#ifndef BUFFER_H_
#define BUFFER_H_

/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */

							/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

														   /* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

														   /* constant definitions */
														   /* You may add your own constant definitions here */
/***************************************************************************************************************************************************
*DEFINING MACROS
***************************************************************************************************************************************************/

#define RT_FAIL1 -1         /* fail return value */
#define RT_FAIL2 -2         /* fail return value */
#define LOAD_FAIL -2       /* load fail error */
#define SET_R_FLAG 1       /* realloc flag set value */
#define RESET_R_FLAG 0		/* resets flag */
#define FIXED 0				/* set mode to fixed*/
#define ADDITIVE 1			/* set mode to additive*/
#define MULTIPLICATIVE -1	/* set mode to multiplicative*/
														  
/***************************************************************************************************************************************************
*TYPE DEFINATION
***************************************************************************************************************************************************/
typedef struct BufferDescriptor {
	char *cb_head;   /* pointer to the beginning of character array (character buffer) */
	short capacity;    /* current dynamic memory size (in bytes) allocated to character buffer */
	short addc_offset;  /* the offset (in chars) to the add-character location */
	short getc_offset;  /* the offset (in chars) to the get-character location */
	short markc_offset; /* the offset (in chars) to the mark location */
	char  inc_factor; /* character array increment factor */
	char  r_flag;     /* reallocation flag */
	char  mode;       /* operational mode indicator*/
	int   eob;       /* end-of-buffer flag */
} Buffer, *pBuffer;


/***************************************************************************************************************************************************
*FUNCTION PROTOTYPES
***************************************************************************************************************************************************/
Buffer *b_allocate(short init_capacity, char inc_factor, char o_mode);
pBuffer b_addc(pBuffer const pBD, char symbol);
int b_clear(Buffer *const pBD);
void b_free(Buffer *const pBD);
int b_isfull(Buffer *const pBD);
short b_limit(Buffer *const pBD);
short b_capacity(Buffer *const pBD);
short b_mark(Buffer *const pBD, short mark);
int b_mode(Buffer *const pBD);
size_t b_incfactor(Buffer *const pBD);
int b_load(FILE *const fi, Buffer *const pBD);
int b_isempty(Buffer *const pBD);
int b_eob(Buffer *const pBD);
char b_getc(Buffer *const pBD);
int b_print(Buffer *const pBD);
Buffer *b_compact(Buffer *const pBD, char symbol);
char b_rflag(Buffer *const pBD);
short b_retract(Buffer *const pBD);
short b_reset(Buffer *const pBD);
short b_getcoffset(Buffer *const pBD);
int b_rewind(Buffer *const pBD);
char *b_location(Buffer *const pBD, short loc_offset);
#endif
