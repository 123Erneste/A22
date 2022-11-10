/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2022
* Author: Svillen Ranev - Paulo Sousa
* Professors: Paulo Sousa
************************************************************
*/

/*
************************************************************
* File name: scanner.h
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A22, A32.
* Date: Jul 01 2022
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1  /* Value for run-time error */

/* TO_DO: Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/*  0: Error token */
	MNID_T,		/*  1: Method name identifier token (start: &) */
	STR_T,		/*  2: String literal token */
	LPR_T,		/*  3: Left parenthesis token */
	RPR_T,		/*  4: Right parenthesis token */
	SMC_T,		/*  5: Semi colon token */
	RBR_T,		/*  6: Right brace token */
	KW_T,		/*  7: Keyword token */
	ASS_T,		/*  8: Assignment operator (=) */
	RTE_T,		/*  9: Run-time error token */
	INL_T,		/* 10: Integer token */
	FL_T,		/* 11: Float token */
	SEOF_T,		/* 13: Source end-of-file token */
	VAR_T,      /* 14: Variables */
	ART_T,      /* 15: Any arithmietic exp */\
	RLO_T,      /* 16: Any relational exp */
	LGO_T       /* 17: Any logical exp */
};

/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT, OP_EGT, OP_ELT } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* TO_DO: Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	jer_intg codeType;      /* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	jer_intg intValue;						/* integer literal attribute (value) */
	jer_intg keywordIndex;					/* keyword index in the keyword table */
	jer_intg contentString;				/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	jer_real floatValue;					/* floating-point literal attribute (value) */
	jer_char idLexeme[VID_LEN + 1];		/* variable identifier token attribute */
	jer_char errLexeme[ERR_LEN + 1];		/* error token attribite */
} TokenAttribute;

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	jer_byte flags;			/* Flags information */
	union {
		jer_intg intValue;				/* Integer value */
		jer_real floatValue;			/* Float value */
		jer_char* stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	jer_intg code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFF

/*  Special case tokens processed separately one by one in the token-driven part of the scanner
 *  '=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' , ';',
 *  white space, #comment\n , ',' , ';' , '-' , '+' , '*' , '/', # ,
 *  .&., .|. , .!. , SEOF.
 */

 /* TO_DO: Define lexeme FIXED classes */
 /* These constants will be used on nextClass */
#define CHRCOL2 '_'
#define CHRCOL3 '('
#define CHRCOL4 '\"'
#define CHRCOL5 '.'

/* These constants will be used on VID / MID function */
#define MNIDPREFIX '('

/* TO_DO: Error states and illegal state */
#define FS		100		/* Illegal state / Final state */
#define ESWR	10		/* Error state with retract */
#define ESNR	11		/* Error state with no retract */

 /* TO_DO: State transition table definition */
#define TABLE_COLUMNS 7

/* DONE: Transition table - type of states defined in separate table */
static jer_intg transitionTable[][TABLE_COLUMNS] = {
	/*         [A-z], [0-9],    _,    (,    ", 	   .,    SEOF,           other               */
	/*         [A-z], [0-9],    _,    (,    ", 	   .,    SEOF,           other               */
	/*          L(0),  D(1), U(2), M(3), Q(4),  P(5),    E(6),            O(7)               */
	/* S00 */ {     1,    6, ESNR, ESNR,    4,  ESNR,	 /*ESWR,*/     ESNR}, /* NOAS        */
	/* S01 */ {     1,    1,    1,    2,    3,     3,	 /*ESWR,*/ 	      3}, /* NOAS        */
	/* S02 */ {    FS,   FS,   FS,   FS,   FS,    FS,	 /*FS,*/         FS}, /* ASNR (MNID) */
	/* S03 */ {    FS,   FS,   FS,   FS,   FS,    FS,	 /*FS,*/         FS}, /* ASWR (KEY)  */
	/* S04 */ {     4,    4,    4,    4,    5,     4,	 /*ESWR*/         4}, /* NOAS        */
	/* S05 */ {    FS,   FS,   FS,   FS,   FS,    FS,	  /*FS,*/        FS}, /* ASNR (SL)   */
	/* S06 */ {     7,    6,    7 ,    7,   7,     8,	 /*ESWR*/        7}, /* NOAS        */
	/* S07 */ {    FS,   FS,   FS,   FS,   FS,    FS,	 /*FS,*/          FS}, /* ASWR (IL)   */
	/* S08 */ {  ESNR,    9, ESNR, ESNR, ESNR,  ESNR,	 /*ESWR*/      ESNR}, /* NOAS        */
	/* S09 */ {    10,    9,   10,   10,   10,    10,	 /*ESWR*/        10}, /* NOAS        */
	/* S10 */ {    FS,   FS,   FS,   FS,   FS,    FS,	 /*FS,*/         FS}, /* ASNR (FL)   */
	/* S11 */ {    FS,   FS,   FS,   FS,   FS,    FS,	 /*FS,*/         FS}, /* ASNR (ES)   */
	/* S12 */ {    FS,   FS,   FS,   FS,   FS,    FS,	 /*FS,*/         FS}, /* ASWR (ER)   */

};

/* Define accepting states types */
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

/* DONE: Define list of acceptable states */
static jer_intg stateType[] =
{
	NOFS, /* 00 */
	NOFS, /* 01 */
	FSNR, /* 02 (MID) - Methods */
	FSWR, /* 03 (KEY) */
	NOFS, /* 04 */
	FSNR, /* 05 (SL) */
	NOFS, /* 06 */
	FSWR, /* 07 (IL) */
	NOFS, /* 08 */
	NOFS, /* 09 */
	FSNR, /* 10 (FL) */
	FSNR, /* 11 (Err1 - no retract) */
	FSWR  /* 12 (Err2 - retract) */
};

/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
jer_intg startScanner(ReaderPointer psc_buf);
static jer_intg nextClass(jer_char c);			/* character class function */
static jer_intg nextState(jer_intg, jer_char);		/* state machine function */

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* TO_DO: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(jer_char* lexeme);

/* Declare accepting states functions */
Token funcSL(jer_char lexeme[]);
Token funcID(jer_char lexeme[]);
Token funcKEY(jer_char lexeme[]);
Token funcIL(jer_char lexeme[]);
Token funcFL(jer_char lexeme[]);
Token funcErr(jer_char lexeme[]);

/*
 * Accepting function (action) callback table (array) definition
 * If you do not want to use the typedef, the equvalent declaration is:
 */

 /* DONE: Define final state table */
static PTR_ACCFUN finalStateTable[] = {
	NULL,		/* -    [00] */
	NULL,		/* -    [01] */
	funcID,		/* MNID	[02] */
	funcKEY,	/* KEY  [03] */
	NULL,		/* -    [04] */
	funcSL,		/* SL   [05] */
	NULL,		/* -    [06] */
	funcIL,		/* IL   [07] */
	NULL,		/* -    [08] */
	NULL,		/* -    [09] */
	funcFL,		/* FL   [10] */
	funcErr,	/* ERR1 [11] */
	funcErr		/* ERR2 [12] */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO: Define the number of Keywords from the language */
#define KWT_SIZE  31

/* TO_DO: Define the list of keywords */
static jer_char* keywordTable[KWT_SIZE] = {
	"break",
	"return",
	"false",
	"true",
	"null",
	"local",
	"global",
	"and",
	"or",
	"continue",
	"def",
	"del",
	"if",
	"elseif",
	"finally",
	"for",
	"while",
	"in",
	"pass",
	"with",
	"yield",
	"try",
	"expect",
	"as",
	"class",
	"raise",
	"int",
	"char",
	"bool",
	"float",
	"string"
};

/* NEW SECTION: About indentation */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */

#define INDENT '\t'  /* Tabulation */

 /* TO_DO: Should be used if no symbol table is implemented */
typedef struct languageAttributes {
	jer_char indentationCharType;
	jer_intg indentationCurrentPos;
	/* TO_DO: Include any extra attribute to be used in your scanner (OPTIONAL and FREE) */
} LanguageAttributes;

#endif
