#ifndef PARSER
#define PARSER

typedef enum line_type{EMPTY, COMMENT, DIRECTION, INSTRUCTION} line_type;

typedef enum error_code{OK, BAD_DIRECT, BAD_INSTRUCTION, BAD_NUMBER, NUM_OVERRUN, BAD_STRING,
	EXTRA_COMMA, BAD_LABEL, MISSING_PARAM, BAD_REGISTER, EXTRANEOUS_TEXT,
	LABEL_REDIFINITION, UNDEFINED_LABEL, BAD_JUMP_ADDRESS} error_code;

typedef enum direction_type{DH, DW, DB, ASCIZ, ENTRY, EXTERN, FAIL} direction_type;

typedef enum instruction_type{ADD, SUB, AND, OR, NOR, MOVE, MVHI, MVLO, ADDI, SUBI,
ANDI, ORI, NORI, BNE, BEQ, BLT, BGT, LB, SB, LW, SW, LH, SH, JMP, LA, CALL, STOP, CMD_FAIL} instruction_type;

/*This function receives line and checks it's type
 * for further analyzing
 *
 * Parameters:
 * line - a pointer to the start of the line
 *
 * Returns:
 * line type - EMPTY, COMMENT, DIRECTION, INSTRUCTION
 * */
line_type check_type(char *line);


/*This function goes over the received string and decides what
 * type of a direction it is, and checks if it is a legal direction. Also
 * this function checks if there is a legal label.
 *
 * Parameters:
 * line -  a pointer to the direction string
 * type - a pointer to the enum that represents the direction type
 * label - a pointer to a string that represents the label
 * numbers -  a pointer to an array of numbers/ characters in a given direction
 * num_of_nums - a pointer to the number of read numbers
 *
 * Return-
 * error_code - returns the type of the error in a given direction, or ok
 * if there is no error.*/

error_code parse_direction(char * line, direction_type * type,
		char * label, int * numbers, int * num_of_nums);

/*This function goes over the received string and decides what
 * type of a instruction it is, and checks if it is a legal instruction. Also
 * this function checks if there is a legal label.
 *
 * Parameters:
 * line -  a pointer to the direction string
 * type - a pointer to the enum that represents the direction type
 * label - a pointer to a string that represents the label
 * val1 -  the value of first number if exists
 * val2 -  the value of second number if exists
 * val3 -  the value of third number if exists
 * label_cmd - the label value which appears as part of the instruction
 *
 * Return-
 * error_code - returns the type of the error in a given instruction, or ok
 * if there is no error.*/
error_code parse_instruction(char * line, instruction_type * type, char * label, int * val1, int * val2, int * val3, char * label_cmd);


/* This function receives the error type and the line where
 * the error appeared, and prints a message for the user.
 * */
void error_handler(error_code err, int index);

#endif


