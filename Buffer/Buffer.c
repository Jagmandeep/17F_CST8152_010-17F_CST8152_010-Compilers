/*File name :	[Buffer.c]
  Compiler :	[MS Visual Studio 2015]
  Author :		[Jagmandeep Singh, ID# 040]
  Course :		CST 8152 – Compilers, Lab Section : [13]
  Assignment :	[ 1 ]
  Date :		[29-9-2017]
  Professor :	Sv.Ranev
  Purpose :		[The purpose of this assignment is to implement a buffer that can operate in three different modes:"fixed-size", "additive 
				self-incrementing", "multiplicative self-incrementing" This file imlements all function prototypes declaired in buffer.h 
				file. ]
Function list : [ b_allocate(), 				b_addc(), 				b_clear(), 				b_free(),				b_isfull(), 				b_limit(), 				b_capacity(),				b_mark(), 				b_mode(), 				b_incfactor(), 				b_load(),				b_isempty(), 				b_eob(), 				b_getc(), 				b_print(),				b_compact(), 				b_rflag(), 				b_retract(),				b_reset(), 				b_getcoffset(), 				b_rewind(),				b_location() ]*/
#include "buffer.h"
/***************************************************************************************************************************************************
Purpose:		[ This function allocates memory for our buffer ]
Author :		[ Jagmandeep Singh ]
History / Versions : [ 1 , 19-9-2017 ]
Called functions : [ calloc(), malloc(), free(), b_free() ]
Parameters :	[ short, char, char ]
Return value :	[ Buffer * ]
Algorithm :		[ Using calloc() gives us advantage that it automatically initializes all components of buffer to 0 ]***************************************************************************************************************************************************/
Buffer *b_allocate(short init_capacity, char inc_factor, char o_mode) { 
	/* Check for valid init_capacity */
	if ( init_capacity > SHRT_MAX-1 || init_capacity < 0 ) {
		return NULL;
	}
	/*variable b allocates memory for buffer and initializes all its elements to 0 using calloc function */
	Buffer* b = (Buffer *)calloc(1, sizeof(Buffer));	
	/*check if the calloc function successfully allocated memory or not*/
	if (b == NULL) {
		return NULL;
	}
	b->cb_head = (char *)malloc( sizeof(char) * init_capacity);
	/*check if the malloc function successfully allocated memory or not*/
	if (b->cb_head == NULL) {
		free(b);
		return NULL;
	}
	/* Check to handle invalid entry of mode */
	if (o_mode != 'f' && o_mode != 'm' && o_mode != 'a') {
		return NULL;
	}
	/*Check if mode is fixed and size is 0 so nothing chan be changed in this case so we return*/
	else if (o_mode == 'f' && init_capacity == 0) {
		return NULL;
	}
	/*Check if mode is fixed or inc_factor is 0*/
	else if (o_mode == 'f' || inc_factor == 0) {
		b->mode = FIXED;
		b->inc_factor = 0;
	}
	/*Check for fixed mode and inc_factor other than 0*/
	else if (o_mode == 'f' && inc_factor != 0) {
		b->mode = FIXED;
		b->inc_factor = 0;
	}
	/*In additive mode  unsigned char is used because value of inc_factor goes upto 255 (signed char goes negative after 127)*/
	else if (o_mode == 'a' && ( (unsigned char)inc_factor ) >= 1 && ( (unsigned char)inc_factor ) <= 255) {
		b->mode = ADDITIVE;
		b->inc_factor = (unsigned char) inc_factor;
	}
	/*Check for multiplicative mode and valid range of inc_factor*/
	else if (o_mode == 'm' && inc_factor >= 1 && inc_factor <= 100) {
		b->mode = MULTIPLICATIVE;
		b->inc_factor = inc_factor;
	}
	/*If none of the condition stated above works than free the allocated memory (to avoid memory leaks)*/
	else {
		b_free(b);
		return NULL;
	}
	b->capacity = init_capacity;
	return b;
}
/***************************************************************************************************************************************************
Purpose:		[ This function adds characters into buffer. If memory of buffer is full it tries to increase capacity ]
Author :		[ Jagmandeep Singh ]
History / Versions : [ 1 , 20-9-2017 ]
Called functions : [ b_isfull(), sizeof(), realloc() ]
Parameters :	[ pBuffer, char ]
Return value :	[ pBuffer ]
Algorithm :		[ realloc() is used to change the capacity of buffer. Realloc would crater new 				  space in memory(if needed) and copy all elements to new location]***************************************************************************************************************************************************/
pBuffer b_addc(pBuffer const pBD, char symbol) {
	/*Sets r_flag to 0*/
	pBD->r_flag = RESET_R_FLAG;
	short availableSpace;	/*Available space in buffer would be calculated in function and stored in this variable*/
	short increment;		/*increment variable will store increment in multiplicative mode*/
	char *new = NULL;
	/*Variable newCap is created to calculte new capacity for buffer*/
	short newCap = 0;
	/*Check if mode is valid and buffer is not full */
	if ( (pBD->mode == 0 || pBD->mode == 1 || pBD->mode == -1) && b_isfull(pBD) == 0 ) {
		/*Adds character (symbol) in buffer at position addc_offset*/
		pBD->cb_head[pBD->addc_offset] = symbol;
		/*After adding character to buffer addc_offset is incremented by one*/
		pBD->addc_offset++;
		return pBD;
	}
	/*Check if buffer is full*/
	if (b_isfull(pBD) == 1) {
		/*check if mode is fixed*/
		if (pBD->mode == FIXED) {
			return NULL;
		}
		/*Check if mode is additive*/
		else if (pBD->mode == ADDITIVE) {
			/*Check if capacity is already SHRT_MAX-1*/
			if (pBD->capacity == (SHRT_MAX - 1)) {
				return NULL;
			}
			/*new capacity is calculated and stored in newCap variable*/
			newCap = pBD->capacity + ((unsigned char)pBD->inc_factor * sizeof(char));
			/*Check foe validity of newly calculated capacity*/
			if (newCap > 0) {

				if (newCap < (SHRT_MAX - 1)) {
					pBD->capacity = newCap;
				}
				else pBD->capacity = (SHRT_MAX - 1);
			}

			else return NULL;
		}
			
		/*Check if mode is Multiplicative*/
		else if (pBD->mode == MULTIPLICATIVE) {
			/*Check if buffer has already reached to its maximum capacity*/
			if (pBD->capacity >= (SHRT_MAX - 1)) {
				return NULL;
			}
			else {
				availableSpace = (SHRT_MAX - 1) - pBD->capacity;	/*Calculation to find available space in buffer*/
				increment = (availableSpace *(unsigned char) pBD->inc_factor) / 100;
				newCap = pBD->capacity + increment;
				/*Checks if new calculated capacity is not in valid range */
				if ( newCap >= (SHRT_MAX - 1) || newCap <= pBD->capacity) {
					pBD->capacity = SHRT_MAX - 1;
				}
				else {
					pBD->capacity = newCap;
				}
			}
		}
		/*Realloc function is called to alloc memory for buffer depending on capacity*/
		/*Realloc() would allocate memory and copy all evements to new location if needed*/
		new = (char *) realloc(pBD->cb_head, (sizeof(short) * pBD->capacity) ) ;
		/*Check if realloc failed to allocate memory*/
		if (new == NULL) {
			return NULL;
		}
		pBD->cb_head = new;	/*head of buffer would now point to newly allocated memory*/
		pBD->r_flag = SET_R_FLAG;	/*Location of buffer changed in memory so we set flag to 1*/
		pBD->cb_head[pBD->addc_offset] = symbol;	/*symbol is added in buffer at position addc_offset*/
		pBD->addc_offset++;	/*After adding element into buffer addc_offset is incremented by 1*/
	}
	return pBD;
}
/***************************************************************************************************************************************************
Purpose:		[ This function re-initializes buffer member so as it act as empty buffer keepinf its allocated memoty same ]
Author :		[ Jagmandeep Singh ]
History / Versions : [ 1 , 18-9-2017 ]
Called functions : [ ]
Parameters :	[ Buffer * ]
Return value :	[ int, (0,-1) ]
Algorithm :		[ ]***************************************************************************************************************************************************/
int b_clear(Buffer *const pBD) {
	/*
	Check if the pointer passed into this function is NULL or has some value;
	This might cause problem at runtime so to handle that this if condition is needed 
	*/
	if (pBD == NULL) {
		return RT_FAIL1;
	}
	pBD->addc_offset = 0;
	pBD->getc_offset = 0;
	pBD->r_flag = 0;
	pBD->markc_offset = 0;
	pBD->eob = 0;
	return 0;
}
/***************************************************************************************************************************************************
Purpose:		[ this function frees memory occupied by character]
Author :		[ Jagmandeep Singh ]
History / Versions : [ 1 , 18-9-2017 ]
Called functions : [ free() ]
Parameters :	[ Buffer * ]
Return value :	[ void ]
Algorithm :		[ ]***************************************************************************************************************************************************/
void b_free(Buffer *const pBD) {
	/*Checks for error at run time*/
	if (pBD == NULL) {
		return;
	}
	free(pBD->cb_head);
	free(pBD);
}
/***************************************************************************************************************************************************
Purpose:		[ This function checks if buffer is full or not ]
Author :		[ Jagmandeep Singh ]
History / Versions : [ 1 , 18-9-2017 ]
Called functions : [ ]
Parameters :	[ Buffer * ]
Return value :	[ int (-1,0,1) ]
Algorithm :		[ ]***************************************************************************************************************************************************/
int b_isfull(Buffer *const pBD) {
	/*Checks for error at run time*/
	if (pBD == NULL) {
		return RT_FAIL1;
	}
	/*Checks if addc_offset is equal to capacity of buffer */
	if(pBD->addc_offset == pBD->capacity) {
		return 1;
	}
	return 0;
}
/***************************************************************************************************************************************************
Purpose:		[ This function will return current limit of buffer ]
Author :		[ Jagmandeep Singh ]
History / Versions : [ 1 , 18-9-2017 ]
Called functions : [ ]
Parameters :	[ Buffer * ]
Return value :	[ short ]
Algorithm :		[ ]***************************************************************************************************************************************************/
short b_limit(Buffer *const pBD) {
	/*Checks for error at run time*/
	if (pBD == NULL) {
		return RT_FAIL1;
	}
	
	return pBD->addc_offset;	
}
/***************************************************************************************************************************************************
Purpose:		[ This function returns current capacity of the character buffer ]
Author :		[ Jagmandeep Singh ]
History / Versions : [ 1 , 18-9-2017 ]
Called functions : [ ]
Parameters :	[ Buffer * ]
Return value :	[ short ]
Algorithm :		[ ]***************************************************************************************************************************************************/
short b_capacity(Buffer *const pBD) {
	/*Checks for error at run time*/
	if (pBD == NULL) {
		return RT_FAIL1;
	}
	return pBD->capacity;
}
/***************************************************************************************************************************************************
Purpose:		[ This function sets value of markc_offset of buffer ]
Author :		[ Jagmandeep Singh ]
History / Versions : [ 1 , 18-9-2017 ]
Called functions : [ ]
Parameters :	[ Buffer *, short ]
Return value :	[ short ]
Algorithm :		[ ]***************************************************************************************************************************************************/
short b_mark(Buffer *const pBD, short mark) {
	/*Checks for error at run time*/
	if (pBD == NULL) {
		return RT_FAIL1;
	}
	/* Checks if mark value is negative or goes beyond the position where last char is added in buffer*/
	if (mark < 0 || mark > pBD->addc_offset) {
		return RT_FAIL1;
	}
	pBD->markc_offset = mark;
	return pBD->markc_offset;
}
/***************************************************************************************************************************************************
Purpose:		[This function returns value of mode ]
Author :		[ Jagmandeep Singh ]
History / Versions : [ 1 , 18-9-2017 ]
Called functions : [ ]
Parameters :	[ Buffer * ]
Return value :	[ int ]
Algorithm :		[ ]***************************************************************************************************************************************************/
int b_mode(Buffer *const pBD) {
	/*Checks for error at run time*/
	if (pBD == NULL) {
		return RT_FAIL1;
	}
	return pBD->mode;
}
/***************************************************************************************************************************************************
Purpose:		[ This function returns non-negative value of inc_factor ]
Author :		[ Jagmandeep Singh ]
History / Versions : [ 1 , 18-9-2017 ]
Called functions : [list of function(s) that are called by this function]
Parameters :	[for each formal parameter : type, specific range or values if applicable]
Return value :	[type, specific values if applicable]
Algorithm :		[ ]***************************************************************************************************************************************************/
size_t b_incfactor(Buffer *const pBD) {
	/*Checks for error at run time*/
	if (pBD == NULL) {
		return 256;
	}
	/*Checks if inc_factor is positive */
	else if( ((unsigned char)pBD->inc_factor) < 0) {
		return 256;
	}	
	return (unsigned char) pBD->inc_factor;
}
/***************************************************************************************************************************************************
Purpose:		[ This function reads file and adds it to buffer character by character ]
Author :		[ Jagmandeep Singh ]
History / Versions : [ 1 , 18-9-2017 ]
Called functions : [ feof() ]
Parameters :	[ FILE * , Buffer * ]
Return value :	[ int ]
Algorithm :		[ Loading is done till feof(end of file) but but not eof ]***************************************************************************************************************************************************/
int b_load(FILE *const fi, Buffer *const pBD) {
	int charsInBuff = 0;	/*counts number of elements added in array*/
	char toAdd = (char)fgetc(fi);			/*toAdd variable stores character to add in our buffer*/
	/*Checks for error at run time*/
	if (pBD == NULL) {
		return RT_FAIL1;
	}
	/*loop continues untill end of file is reached*/
	while (feof(fi) == 0) { 
		/*Checks if character is successfully added to buffer or not*/
		if (b_addc(pBD, toAdd) == NULL) {
			return LOAD_FAIL;
		}
		charsInBuff++;
		toAdd = (char) fgetc(fi);		
	}
	return charsInBuff;
}
/***************************************************************************************************************************************************
Purpose:		[ This function checks if Buffer is empty of not ]
Author :		[ Jagmandeep Singh ]
History / Versions :[ 1 , 18-9-2017 ]
Called functions : [ ]
Parameters :	[ Buffer * ]
Return value :	[ int, (0,1,-1) ]
Algorithm :		[ ]***************************************************************************************************************************************************/
int b_isempty(Buffer *const pBD) {
	/*Checks for error at run time*/
	if (pBD == NULL) {
		return RT_FAIL1;
	}

	/*addc_offse is position where next char is to be added in buffer so if it is 0, 
	  it indicates that buffer is empty */
	/* This if check if addc_offset is 0 */
	if (pBD->addc_offset == 0) {
		return 1;
	}
	
	return 0;
}
/***************************************************************************************************************************************************
Purpose:		[ This function returns eob to the calling function ]
Author :		[ Jagmandeep Singh ]
History / Versions : [ 1 , 18-9-2017 ]
Called functions : [ ]
Parameters :	[ Buffer * ]
Return value :	[ int ]
Algorithm :		[ ]***************************************************************************************************************************************************/
int b_eob(Buffer *const pBD) {
	/*Checks for error at run time*/
	if (pBD == NULL) {
		return RT_FAIL1;
	}
	return pBD->eob;
}
/***************************************************************************************************************************************************
Purpose:		[ This function returns character at position of getc_offset ]
Author :		[ Jagmandeep Singh ]
History / Versions : [ 1 , 18-9-2017 ]
Called functions : [ ]
Parameters :	[ Buffer * ]
Return value :	[ char ]
Algorithm :		[ ]***************************************************************************************************************************************************/
char b_getc(Buffer *const pBD) {
	/*Checks for runtime error*/
	if (pBD == NULL) {
		return RT_FAIL2;
	}
	/* Checks if getc_offset has reached to the end of buffer or not */
	else if (pBD->getc_offset == pBD->addc_offset) {
		pBD->eob = 1;
		return RT_FAIL1;
	}
	else {
		pBD->eob = 0;
	}
	pBD->getc_offset++;
	return pBD->cb_head[pBD->getc_offset-1];
}
/***************************************************************************************************************************************************
Purpose:		[ This function prints contents of Buffer character by character ]
Author :		[ Jagmandeep Singh ]
History / Versions : [ 1 , 18-9-2017 ]
Called functions : [ b_isempty(), b_eob() ]
Parameters :	[ Buffer * ]
Return value :	[ int ]
Algorithm :		[ ]***************************************************************************************************************************************************/
int b_print(Buffer *const pBD) {
	/* charsPrinted variable is used to count number of characters printed */
	short charsPrinted = 0;
	/* Checks for error at runtime */
	if (pBD == NULL) {
		return RT_FAIL1;
	}
	/*Checks if buffer is empty. If Buffer is empty there is nothing to print so it prints a message and returns */
	if ( b_isempty(pBD) == 1 ) {
		printf("Empty buffer\n");
		return 0;
	}
	/*Loop continues untill it reaches end of buffer*/
	/*addc_offset is location where next char is to be added in buffer so when loop 
	  reaches this position loop ends(Buffer has chars from bigning to addc_offset)*/
	while (b_eob(pBD) == 0 && charsPrinted != pBD->addc_offset) {

		printf("%c", b_getc(pBD) );	
		charsPrinted++;
	}
	printf("\n");
	return charsPrinted;
}
/***************************************************************************************************************************************************
Purpose:		[ This function changed Buffer capacity(expands or shrinks) ]
Author :		[ Jagmandeep Singh ]
History / Versions : [ 1 , 18-9-2017 ]
Called functions : [ realloc() ]
Parameters :	[ Buffer *, char ]
Return value :	[ Buffer ]
Algorithm :		[ realloc() is used to change the capacity of buffer. Realloc would crater new 				  space in memory(if needed) and copy all elements to new location]***************************************************************************************************************************************************/
Buffer *b_compact(Buffer *const pBD, char symbol) {
	short newCapacity = 0;
	char *new;
	/*To handle run time error*/
	if (pBD == NULL) {
		return NULL;
	}
	/*
	newCapacity variable is used to calculate new capacity of buffer
	*/
	newCapacity = pBD->addc_offset + 1;
	/* checks if new capacity is less than 0*/
	if (newCapacity < 0) {
		return NULL;
	}
	/*new is use to realloc for new size so that if realloc doesnot work we would still have our old buffer as it is */
	new = realloc(pBD->cb_head, newCapacity);
	/*To handle run time error*/
	if (new == NULL) {
		return NULL;
	}
	pBD->cb_head = new;
	/*r_flag is set to 1 because location of character array in memory has been changed*/
	pBD->r_flag = SET_R_FLAG;
	pBD->capacity = newCapacity;
	/*symbol is added in the buffer at the position og addc_offset*/
	pBD->cb_head[pBD->addc_offset] = symbol;
	/*Since we added a char in Buffer we need to increment our addc_offset*/
	pBD->addc_offset++;

	return pBD;
}
/***************************************************************************************************************************************************
Purpose:		[ This functionn returns buffer r_flag ]
Author :		[ Jagmandeep Singh ]
History / Versions : [ 1 , 18-9-2017 ]
Called functions : [  ]
Parameters :	[ Buffer * ]
Return value :	[ char ]
Algorithm :		[ ]***************************************************************************************************************************************************/
char b_rflag(Buffer *const pBD) {
	/*To handle run time error*/
	if (pBD == NULL) {
		return RT_FAIL1;
	}
	return pBD->r_flag;

}
/***************************************************************************************************************************************************
Purpose:		[ This function decrements getc_offset by 1 ]
Author :		[ Jagmandeep Singh ]
History / Versions : [ 1 , 18-9-2017 ]
Called functions : [ ]
Parameters :	[ Buffer * ]
Return value :	[ short ]
Algorithm :		[ ]***************************************************************************************************************************************************/
short b_retract(Buffer *const pBD) {
	/*To handle run time error*/
	if (pBD == NULL) {
		return RT_FAIL1;
	}
	pBD->getc_offset--;
	return (pBD->getc_offset);
}
/***************************************************************************************************************************************************
Purpose:		[ Resets the value of getc_offset equals to markc_offset ]
Author :		[ Jagmandeep Singh ]
History / Versions : [ 1 , 18-9-2017 ]
Called functions : [ ]
Parameters :	[ Buffer * ]
Return value :	[ short ]
Algorithm :		[ ]***************************************************************************************************************************************************/
short b_reset(Buffer *const pBD) {
	/*To handle run time error*/
	if (pBD == NULL) {
		return RT_FAIL1;
	}
	pBD->getc_offset = pBD->markc_offset;
		
	return pBD->getc_offset;

}
/***************************************************************************************************************************************************
Purpose:		[ Returns character at which getc_offset is pointing ]
Author :		[ Jagmandeep Singh ]
History / Versions : [ 1 , 18-9-2017 ]
Called functions : [ ]
Parameters :	[ Buffer * ]
Return value :	[ short ]
Algorithm :		[ ]***************************************************************************************************************************************************/
short b_getcoffset(Buffer *const pBD) {
	/*To handle run time error*/
	if (pBD == NULL) {
		return RT_FAIL1;
	}
	return pBD->getc_offset;
}
/***************************************************************************************************************************************************
Purpose:		[ Resets particular values of Buffer so that it can be read again ]
Author :		[ Jagmandeep Singh ]
History / Versions : [ 1 , 18-9-2017 ]
Called functions : [  ]
Parameters :	[ Buffer * ]
Return value :	[ int, 0 or -1 ]
Algorithm :		[ ]***************************************************************************************************************************************************/
int b_rewind(Buffer *const pBD) {
	/*To handle run time error*/
	if (pBD == NULL) {
		return RT_FAIL1;
	}
	pBD->getc_offset = 0;
	pBD->markc_offset = 0;
	return 0;
}
/***************************************************************************************************************************************************
Purpose:		[ This function malloc char pointer so as to store address of a character in buffer and
				  reurns that char pointer indicated by position loc_offset]
Author :		[ Jagmandeep Singh ]
History / Versions : [ 1 , 18-9-2017 ]
Called functions : [ malloc() ]
Parameters :	[Buffer *, short [] ]
Return value :	[ char * ]
Algorithm :		[ ]***************************************************************************************************************************************************/
char *b_location(Buffer *const pBD, short loc_offset) {
	/*
	Check if the pointer passed into this function is NULL or has some value;
	*/
	if (pBD == NULL) {
		return NULL;
	}
	/*
	Checks if loc_offset (distance from bigning of buffer to char whose address needs to be returned) is grater than
	addc_offset(in this case represents size of buffer, bigning to last index). If grater than its clear that loc_offset would 
	not point inside our buffer so function returns. 
	*/
	if (pBD->addc_offset < loc_offset) {
		return NULL;
	}
	char *temp = (char *)malloc(sizeof(char));
	/*
	char pointer (temp) is assignes with the address of element(char) in our buffer at possition specified by loc_offset.
	*/
	temp = &(pBD->cb_head[loc_offset]);
	return temp;
}
