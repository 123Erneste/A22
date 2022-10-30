/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2022
* Author: Svillen Ranev - Paulo Sousa
* Professors: Paulo Sousa
************************************************************
 _________________________________
|                                 |
| ........ JER LANGUAGE ......... |
|     __    __    __    __        |
|    /  \  /  \  /  \  /  \       |
| __/  __\/  __\/  __\/  __\__    |
| _/  /__/  /__/  /__/  /_____|   |
|  \_/ \   / \   / \   / \  \___  |
|       \_/   \_/   \_/   \___o_> |
|                                 |
| .. ALGONQUIN COLLEGE - 2022F .. |
|_________________________________|

*/

/*
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 ? Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Sep 01 2022
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */


#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

 /*
 ***********************************************************
 * Function name: readerCreate
 * Purpose: Creates the buffer reader according to capacity, increment
	 factor and operational mode ('f', 'a', 'm')
 * Author: Svillen Ranev / Paulo Sousa
 * History/Versions: S22
 * Called functions: calloc(), malloc()
 * Parameters:
 *   size = initial capacity
 *   increment = increment factor
 *   mode = operational mode
 * Return value: bPointer (pointer to reader)
 * Algorithm: Allocation of memory according to initial (default) values.
 * TODO ......................................................
 *	- Adjust datatypes for your LANGUAGE.
 *   - Use defensive programming
 *	- Check boundary conditions
 *	- Check flags.
 *************************************************************
 */

ReaderPointer readerCreate(jer_intg size, jer_intg increment, jer_intg mode) {
	ReaderPointer readerPointer;
	/* TO_DO: Defensive programming */
	if (size < 0 || increment < 0 || mode < 0)
		return NULL;
	/* TO_DO: Adjust the values according to parameters */
	if (size <= 0)
		size = READER_DEFAULT_SIZE;
	if (increment <= 0 && mode != MODE_FIXED)
		increment = READER_DEFAULT_INCREMENT;
	if (mode != 'f' && mode != 'a' && mode != 'm')
		mode = MODE_FIXED;
	readerPointer = (ReaderPointer)calloc(1, sizeof(BufferReader));
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return NULL;

	readerPointer->content = (jer_char*)malloc(size);
	/* TO_DO: Defensive programming */
	if (!readerPointer->content)
		return NULL;
	/* TO_DO: Initialize the histogram */
	readerPointer->size = size;
	readerPointer->increment = increment;
	readerPointer->mode = mode;
	/* TO_DO: Initialize flags */
	readerPointer->flags = READER_DEFAULT_FLAG;
	/* TO_DO: The created flag must be signalized as EMP */
	readerPointer->flags &= SET_EMP;

	return readerPointer;
}


/*
***********************************************************
* Function name: readerAddChar
* Purpose: Adds a char to buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   ch = char to be added
* Return value:
*	readerPointer (pointer to Buffer Reader)
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

ReaderPointer readerAddChar(ReaderPointer const readerPointer, jer_char ch) {
	jer_char* tempReader = NULL;
	jer_intg newSize = 0;
	/* DONE: Defensive programming */
	if (ch < 0 || ch > 127)
		return NULL;
	if (!readerPointer)
		return NULL;
	/* DONE: Reset Realocation */
	readerPointer->flags &= RST_REL;
	/* TO_DO: Test the inclusion of chars */
	if (readerPointer->position.wrte * (jer_intg)sizeof(jer_char) <= readerPointer->size)
	{
		/* TO_DO: This buffer is NOT full */
	}
	else
	{
		/* DONE: Reset Full flag */
		switch (readerPointer->mode) {
		case MODE_FIXED:
			return NULL;
		case MODE_ADDIT:
			readerPointer->flags &= SET_REL;
			/* DONE: Adjust new size */
			newSize = readerPointer->size + readerPointer->increment;
			if (newSize < 0)
				return NULL;
			break;
		case MODE_MULTI:
			readerPointer->flags &= SET_REL;
			/* DONE: Adjust new size */
			newSize = readerPointer->size * readerPointer->increment;
			if (newSize < 0)
				return NULL;
			break;
		default:
			return NULL;
		}
		/* DONE: New reader allocation */
		tempReader = realloc(readerPointer->content, newSize);
		/* TO_DO: Defensive programming */
		if (!tempReader)
			return NULL;
		/* TO_DO: Check Relocation */
		if (tempReader != readerPointer->content)
			readerPointer->flags &= CHK_REL;
		readerPointer->content = tempReader;
		readerPointer->size = newSize;
	}
	/* TO_DO: Add the char */
	if (readerPointer->position.wrte * (jer_intg)sizeof(jer_char) == readerPointer->size)
		readerPointer->flags |= SET_FUL;
	readerPointer->content[readerPointer->position.wrte++] = ch;
	/* TO_DO: Updates histogram */
	/*readerPointer->histogram[ch]++;*/
	jer_intg length = sizeof(readerPointer->histogram) / sizeof(readerPointer->histogram[0]);
	jer_intg n = 0;
	jer_intg count = 0;
	jer_intg check = 0;
	jer_intg i = 0;



	while (n < length && readerPointer->histogram[n] != 0)
	{
		if (readerPointer->histogram[n] == ch)
		{
			check = 1;
		}
		else
		{
			count++;
		}
		n++;
	}
	i = 0;
	if (check == 0)
	{
		while (i <= count)
		{
			if (readerPointer->histogram[i] == 0)
			{
				readerPointer->histogram[i] = ch;
			}
			i++;
		}
	}
	return readerPointer;
}

/*
***********************************************************
* Function name: readerClear
* Purpose: Clears the buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
jer_boln readerClear(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return JER_FALSE;
	readerPointer->position.wrte = 0;
	//memset(readerPointer->content, 0, sizeof(BufferReader))
	/* TO_DO: Adjust flags original */
	readerPointer->flags |= SET_EMP;
	return JER_TRUE;
}

/*
***********************************************************
* Function name: readerFree
* Purpose: Releases the buffer address
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
jer_boln readerFree(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return JER_FALSE;

	/* TO_DO: Free pointers */
	free(readerPointer->content);
	free(readerPointer);

	return JER_TRUE;
}

/*
***********************************************************
* Function name: readerIsFull
* Purpose: Checks if buffer reader is full
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
********************f*****************************************
*/
jer_boln readerIsFull(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return JER_FALSE;
	/* TO_DO: Check flag if buffer is FUL */
	if (readerPointer->flags & CHK_FUL)
		return JER_TRUE;
	return NULL;
}


/*
***********************************************************
* Function name: readerIsEmpty
* Purpose: Checks if buffer reader is empty.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
jer_boln readerIsEmpty(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return JER_FALSE;
	/* TO_DO: Check flag if buffer is EMP */
	if (readerPointer->flags & CHK_EMP)
		return JER_TRUE;

	return JER_FALSE;
}

/*
***********************************************************
* Function name: readerSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
jer_boln readerSetMark(ReaderPointer const readerPointer, jer_intg mark) {
	/* TO_DO: Defensive programming */
	if (!readerPointer || mark < 0 || mark > readerPointer->position.wrte)
		return JER_FALSE;
	/* TO_DO: Adjust mark */
	readerPointer->position.mark = mark;
	return JER_TRUE;
}


/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
jer_intg readerPrint(ReaderPointer const readerPointer) {
	jer_intg cont = 0;
	jer_char c;
	/* TO_DO: Defensive programming (including invalid chars) */
	if (!readerPointer)
		return READER_ERROR;
	c = readerGetChar(readerPointer);
	if (c < 0 || c>127)
		return READER_ERROR;
	/* TO_DO: Check flag if buffer EOB has achieved */
	//if(readerPointer == CHK_END)
	while (cont < readerPointer->position.wrte) {
		cont++;
		printf("%c", c);
		c = readerGetChar(readerPointer);
	}
	return cont;
}

/*
***********************************************************
* Function name: readerLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   fileDescriptor = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
jer_intg readerLoad(ReaderPointer const readerPointer, FILE* const fileDescriptor) {
	jer_intg size = 0;
	jer_char c;
	/* TO_DO: Defensive programming */
	c = (jer_char)fgetc(fileDescriptor);
	while (!feof(fileDescriptor)) {



		if (!readerAddChar(readerPointer, c)) {
			ungetc(c, fileDescriptor);
			return READER_ERROR;
		}
		c = (char)fgetc(fileDescriptor);
		size++;
	}
	/* TO_DO: Defensive programming */
	return size;
}


/*
***********************************************************
* Function name: readerRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
jer_boln readerRecover(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
	{
		return JER_FALSE;
	}
	/* TO_DO: Recover positions */
	readerPointer->position.mark = NULL;
	readerPointer->position.read = NULL;

	return JER_TRUE;
}


/*
***********************************************************
* Function name: readerRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
jer_boln readerRetract(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
	{
		return JER_FALSE;
	}
	/* TO_DO: Retract (return 1 pos read) */
	while (readerPointer->content != NULL) {
		readerPointer;
	}
	return JER_TRUE;
}


/*
***********************************************************
* Function name: readerRestore
* Purpose: Resets the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
jer_boln readerRestore(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
	{
		return JER_FALSE;
	}
	/* TO_DO: Restore positions (read/mark) */
	readerPointer->position.read;
	readerPointer->position.mark;

	return JER_TRUE;
}



/*
***********************************************************
* Function name: readerGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
jer_char readerGetChar(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
	{
		return NULL;
	}

	/* TO_DO: Check condition to read/wrte */
	/* TO_DO: Set EOB flag */
	/* TO_DO: Reset EOB flag */
	if (readerPointer->position.read == readerPointer->position.wrte)
	{
		readerPointer->flags &= SET_END;
		return READER_TERMINATOR;
	}
	else
	{
		readerPointer->flags &= RST_END;
		return readerPointer->content[readerPointer->position.read++];;
	}
}


/*
***********************************************************
* Function name: readerGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
jer_char* readerGetContent(ReaderPointer const readerPointer, jer_intg pos) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
	{
		return NULL;
	}
	/* TO_DO: Return content (string) */
	return readerPointer->content;
}



/*
***********************************************************
* Function name: readerGetPosRead
* Purpose: Returns the value of getCPosition.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The read position offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
jer_intg readerGetPosRead(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
	{
		return READER_ERROR;
	}
	/* TO_DO: Return read */
	return readerPointer->position.read;
}


/*
***********************************************************
* Function name: readerGetPosWrte
* Purpose: Returns the position of char to be added
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Write position
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
jer_intg readerGetPosWrte(ReaderPointer const readerPointer) {
	/* DONE: Defensive programming */
	if (!readerPointer)
		return READER_ERROR;
	/* DONE: Return wrte */
	return readerPointer->position.wrte;
}


/*
***********************************************************
* Function name: readerGetPosMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Mark position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
jer_intg readerGetPosMark(ReaderPointer const readerPointer) {
	/* DONE: Defensive programming */
	if (!readerPointer)
	{
		return READER_ERROR;
	}
	/* DONE: Return mark */
	return readerPointer->position.mark;
}


/*
***********************************************************
* Function name: readerGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
jer_intg readerGetSize(ReaderPointer const readerPointer) {
	/* DONE: Defensive programming */
	if (!readerPointer)
	{
		return READER_ERROR;
	}
	/* TO_DO: Return size */
	return readerPointer->size;
}

/*
***********************************************************
* Function name: readerGetInc
* Purpose: Returns the buffer increment.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The Buffer increment.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
jer_intg readerGetInc(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer->increment == 0)
	{
		return READER_DEFAULT_INCREMENT;
	}
	/* TO_DO: Return increment */
	return readerPointer->increment;
}

/*
***********************************************************
* Function name: readerGetMode
* Purpose: Returns the operational mode
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Operational mode.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
jer_intg readerGetMode(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
	{
		return READER_ERROR;
	}
	/* TO_DO: Return mode */
	if (readerPointer->mode == 'f') {
		return	MODE_FIXED;
	}
	else if (readerPointer->mode == 'a') {
		return	MODE_ADDIT;
	}
	else if (readerPointer->mode == 'm') {
		return	MODE_MULTI;
	}
	return READER_ERROR;
}


/*
***********************************************************
* Function name: readerGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Flags from Buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
jer_byte readerGetFlags(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
	{
		return READER_ERROR;
	}
	/* TO_DO: Return flags */
	return readerPointer->flags;
}


/*
***********************************************************
* Function name: readerShowStat
* Purpose: Returns the number of chars used.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
jer_intg readerShowStat(ReaderPointer const readerPointer)
{
	/* TO_DO: Defensive programming */
	if (!readerPointer)

	{
		return READER_ERROR;
	}
	/* TO_DO: Updates the histogram */
	jer_intg length = sizeof(readerPointer->histogram) / sizeof(readerPointer->histogram[0]);
	jer_intg n = 0;
	jer_intg count = 0;
	while (n < length && readerPointer->histogram[n] != 0)
	{
		count++;
		n++;

	}
	return count;
}