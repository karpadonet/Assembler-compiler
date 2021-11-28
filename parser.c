
#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"


/*This function checks if the given stop command is legal and parses it
 *
 * Parameters -
 * line- a pointer to the instruction line
 * */
error_code stop_cmd_parse(char * line);


/*This function checks if the given store command is legal and parses it
 *
 * Parameters -
 * line- a pointer to the instruction line
 * val1 - a pointer to the first operand (rt)
 * label - a string which includes the j label. if no label exists the label will be empty.
 * */
error_code j_cmd_parse(char * line, int * val1, char * label);

/*This function checks if the given store command is legal and parses it
 *
 * Parameters -
 * line- a pointer to the instruction line
 * val1 - a pointer to the first operand (rt)
 * val2 - a pointer to the second operand (immed)
 * val3 - a pointer to the third operand (rs)
 * */
error_code store_cmd_parse(char * line, int * val1, int * val2, int * val3);


/*This function checks if the given branch command is legal and parses it
 *
 * Parameters -
 * line- a pointer to the instruction line
 * val1 - a pointer to the first operand (r1)
 * val2 - a pointer to the second operand (r2)
 * label - the jump label
 * */
error_code branch_parse(char * line, int * val1, int * val2, char * label);

/*This function checks if the given i_arithmetic instruction is in the right mode,
 * and we store the operands.
 *
 * Parameters -
 * line- a pointer to the instruction line
 * val1 - a pointer to the first operand (rt)
 * val2 - a pointer to the second operand (immed)
 * val3 - a pointer to the third operand (rs)
 * */
error_code arithm_i_parse(char * line, int * val1, int * val2, int * val3);

/*This function checks if the given arithmetic instruction is in the right mode,
 * and we store the operands.
 *
 * Parameters -
 * line- a pointer to the instruction line
 * val1 - a pointer to the first operand
 * val2 - a pointer to the second operand
 * val3 - a pointer to the third operand
 * */
error_code arithm_parse(char * line, int * val1, int * val2, int * val3);

/*The function move_parse retrieves the operands given to a move command
 *
 * Parameters -
 * line- a pointer to the instruction line
 * val1 - a pointer to the first operand
 * val2 - a pointer to the second operand
 * */
error_code move_parse(char *line, int *val1, int * val2);


/* Function check_direct checks which direction is at the beginning of
 * the string line.
 *
 * Parameters:
 * line - the given string
 *
 * Returns:
 * The direction type
 */

direction_type check_direct(char * line);


/* This function checks which command is at the beginning of
 * the string line.
 *
 * Parameters:
 * line - the given string
 *
 * Returns:
 * The command type
 */
instruction_type check_instruction(char * line);


/* Function check_reserved_word checks whether the given string is equal
 * to a reserved command word.
 *
 * Parameters:
 * str - the given string
 *
 * Returns:
 * 0 - if str is not a reserved work, 1 - else
 */
int check_reserved_word(char * str);


/*This function receives a line of direction and checks it for a  legal label.
 *
 * Parameters-
 * line -  a pointer to the direction string.
 * label - a pointer to fill the label string.
 *
 * Return-
 * returns -1 for no label 1 for legal label and 0 for an illegal label.
 * */
int check_label_at_start(char *line, char *label);

/*This function checks if there is a legal label at the end,
 * after entry or extern direction.
 *
 * parameters -
 * line- a pointer to the start of the line
 * label - a pointer to the start of the label
 *
 * returns -1 for no label 1 for legal label and 0 for an illegal label.
 * */
int check_label_at_end(char *line, char *label);

/*function check_num checks that the given number is valid
 *
 * Parameters:
 * dir - direction type
 * num - the given number
 *
 * Returns:
 * 0 - if the number is not valid, 1 - if the number is valid
 */
int check_num(direction_type dir, int num);


/**********************************************************************************
 *																				  *
 * 							function implementation								  *
 *																				  *
 **********************************************************************************/

/*function check_num checks that the given number is valid
 *
 * Parameters:
 * dir - direction type
 * num - the given number
 *
 * Returns:
 * 0 - if the number is not valid, 1 - if the number is valid
 */
int check_num(direction_type dir, int num)
{
	/* check that the given number is not too big*/
	if (dir == DB && (num > MAX_DB || num < MIN_DB))
			return 0;

	if (dir == DH && (num > MAX_DH || num < MIN_DH))
			return 0;

	if (dir == DW && (num > MAX_DW || num < MIN_DW))
			return 0;

	return 1;

}


error_code stop_cmd_parse(char * line)
{
	remove_spaces(line);

	if (strlen(line) > 0)
	{
		return EXTRANEOUS_TEXT;
	}

	return OK;
}

error_code j_cmd_parse(char * line, int * val1, char * label)
{

	/*the parameter is either a register or a label*/

	remove_spaces(line);

	/*check if the first char is $ following a number*/
	if(line[0]=='$')
	{
		int num = 0;

		remove_chars(line,1);

		if(str2num(line,&num))
		{
			*val1 = num;
		}
		else return BAD_REGISTER;

		if (num < 0 || num > 31)
		{
			return BAD_REGISTER;
		}

	}
	else /*this is probably a label*/
	{
		int res;

		res = check_label_at_end(line, label);

		if(res == -1)
		{
			return MISSING_PARAM;
		}
		else if (res == 0)
		{
			return BAD_LABEL;
		}

	}

	/*check if there's any additional text*/
	remove_spaces(line);

	if(strlen(line) > 0)
	{
		return EXTRANEOUS_TEXT;
	}


	return OK;
}


error_code store_cmd_parse(char * line, int * val1, int * val2, int * val3)
{
	/*same implementation as arithm_i_parse*/
	return arithm_i_parse(line, val1, val2, val3);

}


error_code branch_parse(char * line, int * val1, int * val2, char * label)
{
	int res = 0, comma_res, num = 0, len, count = 0;
	int res_vals[2] = {0};

	remove_spaces(line);
	len = strlen(line);

	while(len && count < 2)
	{
		/*checking if this is a register*/
		if(line[0] != '$')
		{
			return BAD_REGISTER;
		}

		/*removing the $*/
		remove_chars(line,1);

		/*read the register number*/
		res = str2num(line, &num);
		if (!res)
			return BAD_REGISTER;

		/*check the register is in the range*/
		if(num < 0 || num > 31)
		{
			return BAD_REGISTER;
		}

		res_vals[count] = num;

		/*remove spaces and comma*/
		remove_spaces(line);
		comma_res = check_commas(line);

		if (comma_res > 1)
			return EXTRA_COMMA;
		else
			comma_res = 0;

		len = strlen(line);
		count++;
	}

	/*copy return values*/
	*val1 = res_vals[0];
	*val2 = res_vals[1];

	/*handle the label at the end*/
	res = check_label_at_end(line, label);

	 if (res == -1)
		 return MISSING_PARAM;
	 else if (res == 0)
		 return BAD_LABEL;

	 /*check if there is extra text in the end of the command*/
	 remove_spaces(line);
	 if (strlen(line)>0)
		 return EXTRANEOUS_TEXT;

	return OK;
}


error_code arithm_i_parse(char * line, int * val1, int * val2, int * val3)
{
	int res = 0, comma_res, num = 0, len, count = 0;
	int res_vals[3] = {0};

	remove_spaces(line);
	len = strlen(line);

	while(len && count < 3)
	{
		if(count!=1)
		{
			/*checking if this is a register*/
			if(line[0] != '$')
			{
				return BAD_REGISTER;
			}

			/*removing the $*/
			remove_chars(line,1);
		}

		/*read the register number*/
		res = str2num(line, &num);
		if (!res)
		{
			if(count !=1)
				return BAD_REGISTER;
			else
				return BAD_NUMBER;
		}


		if (count !=1)
		{
			/*check the register is in the range*/
			if(num < 0 || num > 31)
			{
				return BAD_REGISTER;
			}
		}
		else
		{
			if (num < MIN_DW || num > MAX_DW)
				return BAD_NUMBER;
		}


		res_vals[count] = num;

		/*remove spaces and comma*/
		if(count!=2)
		{
			remove_spaces(line);
			comma_res = check_commas(line);
		}

		if (comma_res > 1)
			return EXTRA_COMMA;
		else
			comma_res = 0;

		len = strlen(line);
		count++;
	}

	if(strlen(line) == 0 && count < 3)
		return MISSING_PARAM;

	if(strlen(line) > 0 && count >=3)
		return EXTRANEOUS_TEXT;

	/*copy return values*/
	*val1 = res_vals[0];
	*val2 = res_vals[1];
	*val3 = res_vals[2];

	return OK;

	return OK;
}


error_code move_parse(char *line, int *val1, int * val2)
{
	int res = 0, comma_res, num = 0, len, count = 0;
	int res_vals[2] = {0};

	remove_spaces(line);
	len = strlen(line);

	while(len && count < 2)
	{
		/*checking if this is a register*/
		if(line[0] != '$')
		{
			return BAD_REGISTER;
		}

		/*removing the $*/
		remove_chars(line,1);

		/*read the register number*/
		res = str2num(line, &num);
		if (!res)
			return BAD_REGISTER;

		/*check the register is in the range*/
		if(num < 0 || num > 31)
		{
			return BAD_REGISTER;
		}

		res_vals[count] = num;

		/*remove spaces and comma*/
		if(count!=1)
		{
			remove_spaces(line);
			comma_res = check_commas(line);
		}

		if (comma_res > 1)
			return EXTRA_COMMA;
		else
			comma_res = 0;

		len = strlen(line);
		count++;
	}

	if(strlen(line) == 0 && count < 2)
		return MISSING_PARAM;

	if(strlen(line) > 0 && count >=2)
		return EXTRANEOUS_TEXT;

	/*copy return values*/
	*val1 = res_vals[0];
	*val2 = res_vals[1];

	return OK;


}


error_code arithm_parse(char * line, int * val1, int * val2, int * val3)
{
	int res = 0, comma_res, num = 0, len, count = 0;
	int res_vals[3] = {0};

	remove_spaces(line);
	len = strlen(line);

	while(len && count < 3)
	{
		/*checking if this is a register*/
		if(line[0] != '$')
		{
			return BAD_REGISTER;
		}

		/*removing the $*/
		remove_chars(line,1);

		/*read the register number*/
		res = str2num(line, &num);
		if (!res)
			return BAD_REGISTER;

		/*check the register is in the range*/
		if(num < 0 || num > 31)
		{
			return BAD_REGISTER;
		}

		res_vals[count] = num;

		/*remove spaces and comma*/
		if(count!=2)
		{
			remove_spaces(line);
			comma_res = check_commas(line);
		}

		if (comma_res > 1)
			return EXTRA_COMMA;
		else
			comma_res = 0;

		len = strlen(line);
		count++;
	}

	if(strlen(line) == 0 && count < 3)
		return MISSING_PARAM;

	if(strlen(line) > 0 && count >=3)
		return EXTRANEOUS_TEXT;

	/*copy return values*/
	*val1 = res_vals[0];
	*val2 = res_vals[1];
	*val3 = res_vals[2];

	return OK;
}



direction_type check_direct(char * line)
{
	if(!strncmp(line,".dw",3))
	{
		/*remove 3 chars from line*/
		remove_chars(line,3);
		return DW;

	}
	else if(!strncmp(line,".dh",3))
	{
		/*remove 3 chars from line*/
		remove_chars(line,3);
		return DH;

	}
	else if(!strncmp(line,".db",3))
	{
		/*remove 3 chars from line*/
		remove_chars(line,3);
		return DB;

	}
	else if(!strncmp(line,".asciz",6))
	{
		/*remove 3 chars from line*/
		remove_chars(line,6);
		return ASCIZ;

	}
	else if(!strncmp(line,".entry",6))
	{
		/*remove 3 chars from line*/
		remove_chars(line,6);
		return ENTRY;

	}
	else if(!strncmp(line,".extern",7))
	{
		/*remove 3 chars from line*/
		remove_chars(line,7);
		return EXTERN;

	}
	else
	{
		return FAIL;
	}


}


instruction_type check_instruction(char * line)
{
	if(!strncmp(line,"add",3))
	{
		if (!strncmp(line,"addi",4))
		{
			/*remove 4 chars from line*/
			remove_chars(line,4);
			return ADDI;
		}
		else
		{
			/*remove 3 chars from line*/
			remove_chars(line,3);
			return ADD;
		}

	}
	else if(!strncmp(line,"sub",3))
	{


		if(!strncmp(line,"subi",4))
		{
			/*remove 4 chars from line*/
			remove_chars(line,4);
			return SUBI;
		}
		else
		{
			/*remove 3 chars from line*/
			remove_chars(line,3);
			return SUB;
		}

	}
	else if(!strncmp(line,"and",3))
	{
		if(!strncmp(line,"andi",4))
		{
			/*remove 4 chars from line*/
			remove_chars(line,4);
			return ANDI;

		}
		else
		{
			/*remove 3 chars from line*/
			remove_chars(line,3);
			return AND;

		}

	}
	else if(!strncmp(line,"or",2))
	{

		if(!strncmp(line,"ori",3))
		{
			/*remove 3 chars from line*/
			remove_chars(line,3);
			return ORI;

		}
		else
		{
			/*remove 2 chars from line*/
			remove_chars(line,2);
			return OR;
		}

	}
	else if(!strncmp(line,"nor",3))
	{
		if(!strncmp(line,"nori",4))
		{
			/*remove 4 chars from line*/
			remove_chars(line,4);
			return NORI;

		}
		else
		{
			/*remove 3 chars from line*/
			remove_chars(line,3);
			return NOR;
		}

	}
	else if(!strncmp(line,"move",4))
	{
		/*remove 4 chars from line*/
		remove_chars(line,4);
		return MOVE;

	}
	else if(!strncmp(line,"mvhi",4))
	{
		/*remove 4 chars from line*/
		remove_chars(line,4);
		return MVHI;

	}
	else if(!strncmp(line,"mvlo",4))
	{
		/*remove 4 chars from line*/
		remove_chars(line,4);
		return MVLO;

	}
	else if(!strncmp(line,"bne",3))
	{
		/*remove 3 chars from line*/
		remove_chars(line,3);
		return BNE;

	}
	else if(!strncmp(line,"beq",3))
	{
		/*remove 3 chars from line*/
		remove_chars(line,3);
		return BEQ;

	}
	else if(!strncmp(line,"blt",3))
	{
		/*remove 3 chars from line*/
		remove_chars(line,3);
		return BLT;

	}
	else if(!strncmp(line,"bgt",3))
	{
		/*remove 3 chars from line*/
		remove_chars(line,3);
		return BGT;

	}
	else if(!strncmp(line,"lb",2))
	{
		/*remove 2 chars from line*/
		remove_chars(line,2);
		return LB;

	}
	else if(!strncmp(line,"sb",2))
	{
		/*remove 2 chars from line*/
		remove_chars(line,2);
		return SB;

	}
	else if(!strncmp(line,"lw",2))
	{
		/*remove 2 chars from line*/
		remove_chars(line,2);
		return LW;

	}
	else if(!strncmp(line,"sw",2))
	{
		/*remove 2 chars from line*/
		remove_chars(line,2);
		return SW;

	}
	else if(!strncmp(line,"lh",2))
	{
		/*remove 2 chars from line*/
		remove_chars(line,2);
		return LH;

	}
	else if(!strncmp(line,"sh",2))
	{
		/*remove 2 chars from line*/
		remove_chars(line,2);
		return SH;

	}
	else if(!strncmp(line,"jmp",3))
	{
		/*remove 3 chars from line*/
		remove_chars(line,3);
		return JMP;

	}
	else if(!strncmp(line,"la",2))
	{
		/*remove 2 chars from line*/
		remove_chars(line,2);
		return LA;

	}
	else if(!strncmp(line,"call",4))
	{
		/*remove 3 chars from line*/
		remove_chars(line,4);
		return CALL;

	}
	else if(!strncmp(line,"stop",4))
	{
		/*remove 4 chars from line*/
		remove_chars(line,4);
		return STOP;
	}
	else
	{
		return CMD_FAIL;
	}

}


int check_reserved_word(char * str)
{
	if ((strlen(str) == 2) && (!strncmp(str,"dh",2) || !strncmp(str, "dw",2) ||
			!strncmp(str,"db",2) || !strncmp(str,"or",2) || !strncmp(str,"lb",2) ||
			!strncmp(str,"sb",2) || !strncmp(str,"lw",2) || !strncmp(str,"sw",2) ||
			!strncmp(str,"lh",2) || !strncmp(str,"sh",2) || !strncmp(str,"la",2)))
		return 1;

	if ((strlen(str) == 3) && (!strncmp(str,"add",3) || !strncmp(str,"sub",3) ||
			!strncmp(str,"and",3) || !strncmp(str, "nor",3) || !strncmp(str,"ori",3) ||
			!strncmp(str,"bne",3) || !strncmp(str,"beq",3) || !strncmp(str,"blt",3) ||
			!strncmp(str,"bgt",3) || !strncmp(str,"jmp",3)))
		return 1;

	if((strlen(str) == 4) && (!strncmp(str,"move",4) || !strncmp(str, "mvhi",4) ||
			!strncmp(str,"mvlo",4) || !strncmp(str,"addi",4) || !strncmp(str,"subi",4) ||
			!strncmp(str,"andi",4) || !strncmp(str,"nori",4) || !strncmp(str,"call",4) ||
			!strncmp(str,"stop",4)))
		return 1;

	if ((strlen(str) == 5) && (!strncmp(str,"asciz",5) || !strncmp(str,"entry",5)))
		return 1;

	if((strlen(str) == 6) && !strncmp(str,"extern",6))
		return 1;


	return 0;
}


int check_label_at_start(char *line, char *label)
{
	int result;
	int i=0;

	/*Removing the spaces at the beginning of the direction string*/
	remove_spaces(line);
	/*checks if there are ":" in the given direction string*/
	result = check_two_dots(line);

		if(result == 1)
		{
			int j = 0;
			int k = 0;
			/*check if first char in line is a letter*/
			if ( (line[0] >='A' && line[0] <='Z') ||
					(line[0]>='a' && line[0] <='z'))
			{
				while(line[i] != ':')
				{
					/*check that each next char is either a letter or a number*/
					if ( ( line[i] >='A' && line[i] <='Z') ||
							(line[i] >= 'a' && line[i]<='z') ||
							(line[i] >='0' && line[i] <='9'))
					{
						label[i] = line[i];
						i++;
					}
					else return 0;
				}
				label[i] = '\0';

				j = 0;
				k = 0;

				while(line[j] != ':')
				{
					j++;
				}

				k = j+ 2;

				while(line[k] != '\0')
				{
					line[k - (j +2)] = line[k];
					k++;
				}

					line[k - (j+2)] = '\0';

				return 1;
			}
			else return 0;

		}
		else
		{
			return -1;
		}



}


int check_label_at_end(char *line, char *label)
{
	int i=0;

	/*Removing the spaces at the beginning of the direction string*/
	remove_spaces(line);

	/*no label*/
	if (strlen(line) == 0)
		return -1;

			/*check if first char in line is a letter*/
			if ( (line[0] >='A' && line[0] <='Z') ||
					(line[0]>='a' && line[0] <='z'))
			{
				while(line[i] != '\0' && line[i]!=' ' && line[i]!='\t')
				{
					/*check that each next char is either a letter or a number*/
					if ( ( line[i] >='A' && line[i] <='Z') ||
							(line[i] >= 'a' && line[i]<='z') ||
							(line[i] >='0' && line[i] <='9'))
					{
						label[i] = line[i];
						i++;
					}
					else return 0;
				}
				label[i] = '\0';

				remove_chars(line,i);

				return 1;

		}
		else
		{
			return 0;
		}



}


line_type check_type(char *line){
	char temp[MAXLEN + 1];
	strcpy(temp, line);
	remove_spaces(temp);

	/*check the given type*/
	if(strlen(temp)==0)/*empty line*/
	{
		return EMPTY;
	}
	else if (temp[0] ==';') /*comment line*/
	{
		return COMMENT;
	}
	else if (check_dot(temp))
	{
		/*if there is a dot in the text it is probably a direction*/
		return DIRECTION;
	}
	else
	{
		/*this is probably an instruction*/
		return INSTRUCTION;
	}


	return 0;
}


error_code parse_direction(char * line, direction_type * type,
		char * label, int * numbers, int * num_of_nums)
{
	int label_res;

	/*init the label*/
	label[0] = '\0';

	/*check if there is a label at the beginning */
	label_res = check_label_at_start(line, label);

	if (label_res == 0)
		return BAD_LABEL;

	/*check that the label is not a reserved word*/
	if(check_reserved_word(label))
		return BAD_LABEL;

	/*remove spaces again*/
	remove_spaces(line);

	/*if the format of the direction command is correct next char should be .*/
	if(line[0] != '.')
	{
		return BAD_DIRECT;
	}

	/*check the type of direction*/
	*type = check_direct(line);
	remove_spaces(line);

	/*continue parsing rules for direction according to the type of direction*/
		if(*type == FAIL)
		{
			return BAD_DIRECT;
		}
		else if (*type == DH || *type == DW || *type == DB)
		{
		/*parse according to expected DH,DW, DB format*/
		int res, comma_res = 0, num = 0, len = 0, count = 0;


		res = str2num(line, &num);
		remove_spaces(line);
		len = strlen(line);

		if(!check_num(*type, num))
			return NUM_OVERRUN;

		while(len)

		{
			if (!res)
				return BAD_NUMBER;

			/* check that the given number is not too big*/
			if(!check_num(*type, num))
				return NUM_OVERRUN;


			numbers[count] = num;

			/*remove the spaces and commas after the number*/
			remove_spaces(line);
			comma_res = check_commas(line);

			if (comma_res > 1)
				return EXTRA_COMMA;
			else
				comma_res = 0;

			remove_spaces(line);
			res = str2num(line, &num);
			count++;
			len = strlen(line);

		}

		/*add last number*/
		if (!res)
			return BAD_NUMBER;

		if(!check_num(*type, num))
			return NUM_OVERRUN;

		numbers[count] = num;
		count++;

		if (count == 0) /*if no numbers were read*/
			return MISSING_PARAM;

		if(comma_res > 0)
			return EXTRA_COMMA;

		*num_of_nums = count;


	}
	else if ( *type == ASCIZ)
	{
		/*parse according to asciz format*/

		/*checking if there is a string*/
		int i = 1;
		int count = 0;
		int ind_end = -1;

		/*first removing the spaces*/
		remove_spaces(line);

 		if(line[0] != '"')
		{
			return BAD_STRING;
		}

		/*if the string is empty*/
		if(line[0] == '"' && line[1] == '"')
		{
			numbers[0] = '\0';
		}

		while(line[i] != '\0' && line[i]!=' ' && line[i]!='\t')
		{
			if(line[i] != '"')
			{

				numbers[count] = line[i];
				count++;
				i++;
			}
			else
			{
				ind_end = i;
				i++;
				numbers[ind_end] = '\0';
				break;
			}
		}
		*num_of_nums = count;

		remove_chars(line,i);
		remove_spaces(line);

		if(strlen(line)>0)
			return EXTRANEOUS_TEXT;


		if(ind_end == -1)
		{
			return BAD_STRING;
		}

	}

	else /*parse according to extern or entry format*/
	{
		int res;
		/*first removing the spaces*/
		remove_spaces(line);

		/*check if there is a legal label after the direction
		 * returns 1 for legal label, otherwise returns 0*/
		res = check_label_at_end(line,label);
		if (res == 0)
		{
			return BAD_LABEL;
		}else if(res == -1)
		{
			return MISSING_PARAM;
		}

		/*check that the label is not a reserved word*/
		if(check_reserved_word(label))
		{
			return BAD_LABEL;
	    }


	}


	/*remove spaces */
	remove_spaces(line);

	/*check if the string which is left is not empty*/


	return OK;
}

error_code parse_instruction(char * line, instruction_type * type, char * label, int * val1, int * val2, int * val3, char * label_cmd)
{
	int label_res;

	/*init the labels*/
	label[0] = '\0';
	label_cmd[0] ='\0';

	/*check if there is a label at the beginning */
	label_res = check_label_at_start(line, label);

	if (label_res == 0)
		return BAD_LABEL;

	/*check that the label is not a reserved word*/
	if(check_reserved_word(label))
		return BAD_LABEL;

	/*remove spaces again*/
	remove_spaces(line);

	*type = check_instruction(line);

	if(*type == CMD_FAIL)
	{
		return BAD_INSTRUCTION;
	}
	/*in this type of direction supposed to be 3 operands, all 3 are registers*/
	else if(*type == ADD || *type == SUB || *type == AND || *type == OR || *type == NOR)
	{
		 return arithm_parse(line, val1,val2,val3);

	}
	else if (*type == MOVE || *type == MVHI || *type == MVLO) 	/*2 operands and both of them are registers*/
	{
		return move_parse(line, val1, val2);
	}
	else if (*type == ADDI || *type == SUBI || *type == ANDI || *type == ORI || *type == NORI) /*3 operands, 2 registers and one constant*/
	{
		return arithm_i_parse(line, val1,val2,val3);
	}
	/*2 operands, both of them registers and a label*/
	else if (*type == BEQ || *type == BNE || *type == BLT || *type == BGT)
	{
		return branch_parse(line, val1, val2, label_cmd);
	}
	/* 2 operands that are registers and a constant*/
	else if (*type == LB || *type == SB || *type == LW || *type == SW || *type == LH || *type == SH)
	{
		return store_cmd_parse(line, val1,val2,val3);
	}
	/*operand register or a label*/
	else if(*type == JMP || *type == LA || *type == CALL)
	{
		error_code res;

		res = j_cmd_parse(line, val1, label_cmd);

		if (res == OK)
		{
			if (*type == LA || *type == CALL)
			{
				if (strlen(label_cmd)==0)
				{
					return BAD_LABEL;
				}
			}
		}

		return res;
	}
	else if(*type == STOP)
	{
		return stop_cmd_parse(line);
	}
	else
	{
		return BAD_INSTRUCTION;
	}


	return 0;

}

void error_handler(error_code err, int index)
{
	switch (err)
	{
    case BAD_DIRECT:
    	printf("Line %d : illegal direction\n", index);
    	break;

    case BAD_NUMBER:
    	printf("Line %d : illegal number\n", index);
    	break;

    case NUM_OVERRUN:
    	printf("Line %d : error in number overrun\n", index);
    	break;

   	case BAD_STRING:
   		printf("Line %d : illegal string\n", index);
   		break;

   	case EXTRA_COMMA:
   		printf("Line %d : extra comma\n", index);
   		break;

   	case BAD_LABEL:
   		printf("Line %d : illegal label\n", index);
   		break;

   	case MISSING_PARAM:
   		printf("Line %d : a missing parameter\n", index);
   		break;

   	case BAD_REGISTER:
   		printf("Line %d : bad register id\n", index);
   		break;

   	case EXTRANEOUS_TEXT:
   		printf("Line %d : extraneous text\n", index);
   		break;

   	case LABEL_REDIFINITION:
   		printf("Line %d : label redifinition\n", index);
   		break;

   	case UNDEFINED_LABEL:
   		printf("Line %d : undefined label\n", index);
   		break;

   	case BAD_JUMP_ADDRESS:
   		printf("Line %d : jump value exceeds allowed distance\n", index);
   		break;

   	case BAD_INSTRUCTION:
   		printf("Line %d : undefined instruction\n", index);
   		break;

   	default:
   		break;

	}

}




