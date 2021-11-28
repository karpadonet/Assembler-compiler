#include <stdio.h>
#include <string.h>
#include "runner.h"
#include "parser.h"
#include "symbol.h"
#include "data_handler.h"
#include "extern_entry.h"

/*globals*/
symbol_table table; /*label table*/
data_table data;
data_table inst; /*instruction table*/

/*file ok flag - determines whether the program can compile*/
int is_file_ok;

/*function add direction to table adds an array to data to the global data table
 *
 *Parameters:
 *dir - the type of the direction
 *numbers - an array of numbers
 *num_of_num - the number of given numbers
 */
void add_direction_to_table(direction_type dir,int * numbers, int num_of_nums);

/*This function reads characters from the file to a string
 * until it reaches the end of line.
 *
 * Parameters:
 * fd - file descriptor
 * str_res - the result char array
 *
 * Returns:
 * 1 - The data was read successfully
 * 0 - the line input was too long (over 80 characters)
 * -1 - Read until EOF
 * */
int read_line(FILE *fd, char *str_res);


/*The function adds the given direction to the table for the first run.
 *
 * Parameters:
 * type - instruction type
 * val1
 * val2
 * val3
 * cmd_label
 */
void add_instruction_to_table_first(instruction_type type,
		int val1, int val2, int val3, char * cmd_label);


/*This function adds a label to the symbol table.
 * The label is added in the following cases, if the label is new, or if it already
 * exists and the attribute combination allows it.
 *
 * Allowed attribute combinations: data + entry, code + entry, data, code, entry, external
 *
 *
 * Paremeters:
 * char * label
 * type - 1 for instruction, 0 for direction
 * inst_type - the instruction type
 * dir_type - the direction type
 *
 * Returns:
 * 1 - if the label was added succefully, 0 -  otherwise
 *
 */
int add_label_to_table(char * label, int type,
		direction_type dir_type, instruction_type inst_type);




/**********************************************************************************
 *																				  *
 * 							function implementation								  *
 *																				  *
 **********************************************************************************/


void add_instruction_to_table_first(instruction_type type,
		int val1, int val2, int val3, char * cmd_label)
{
	/*check instruction type and add the instruction to the inst table accordingly*/

	/*add arithmetic instruction*/
	if (type == ADD || type == SUB || type == AND || type == OR || type == NOR )
	{
		add_arithm_cmd(&inst, type, val1, val2, val3);
	}/*add move instruction*/
	else if ( type == MOVE || type == MVHI || type == MVLO )
	{
		add_move_cmd(&inst, type, val1, val2);
	}/*add arithmetic instruction with immediate value*/
	else if (type == ADDI || type == SUBI || type == ANDI || type == ORI || type == NORI )
	{
		add_arithm_i_cmd(&inst, type, val1, val3, val2);
	}/* add branch instruction */
	else if (type == BEQ || type == BNE || type == BLT || type == BGT)
	{
		add_branch_cmd(&inst,type, val1, val2);
	} /*add store instruction*/
	else if (type == LB || type == SB || type == LW || type == SW || type == LH || type == SH )
	{
		add_store_cmd(&inst, type, val1, val3, val2);
	} /* add type j instruction*/
	else if ( type == JMP || type == LA || type == CALL)
	{
		add_j_cmd(&inst, type, val1, cmd_label);
	}
	else /*add a stop command*/
	{
		add_stop_cmd(&inst);
	}




}


int add_label_to_table(char * label, int type,
		direction_type dir_type, instruction_type inst_type)
{
	if (strlen(label)==0) /*of the label is empty stop running*/
	{
		return 1;
	}
	else
	{
		if (type) /*if this is an instruction label*/
		{
			int check;

			/*first check if the label already exists*/
			check = check_label(&table, label);

			if (check) /*see if the label can be added*/
			{
				unsigned char attribute;

				attribute = get_attribute(&table, label);

				if ((attribute & 8) && !(attribute & 1) && !(attribute & 2) && !(attribute & 4))
				{
					add_attribute(&table, label, 0);
					update_symbol_value(&table, label, BASE_ADDRESS + inst.counter); /*add instruction address*/
				}
				else return 0;

			}
			else /*add a new label to the table*/
			{
				add_symbol(&table, label, BASE_ADDRESS + inst.counter , 1,0,0,0);
			}

		}
		else /*this is a direction label*/
		{
			int check;

			/*first check if the label already exists*/
			check = check_label(&table, label);

			if (check)
			{
				unsigned char attribute;

				attribute = get_attribute(&table, label);


				if (dir_type == ENTRY && (attribute & 2 ) && !(attribute & 8 ))
				{
					add_attribute(&table, label, 3); /*add entry attribute*/
				}
				else if (dir_type == ENTRY && (attribute & 1) && !(attribute & 8 ) )
				{
					add_attribute(&table, label, 8);
				}
				else if (((dir_type == DH) || (dir_type == DB) || (dir_type == DW) ||
						(dir_type == ASCIZ)) && (attribute & 8 ) && !(attribute & 2 ) && !(attribute & 1) &&
						!(attribute & 4))
				{
					add_attribute(&table, label, 1);
					update_symbol_value(&table, label, data.counter); /*add data address*/
				}
				else return 0;

			}
			else /*if label does not exist add it to the table*/
			{
				if (dir_type == ENTRY)
				{
					add_symbol(&table, label,data.counter,0,0,0,1);
				}
				else if (dir_type == EXTERN)
				{
					add_symbol(&table, label,data.counter,0,0,1,0);
				}
				else /*db,dw,dh, asciz */
				{
					add_symbol(&table,label, data.counter,0,1,0,0);
				}

			}


		}

	}


	return 1;
}

void add_direction_to_table(direction_type dir,int * numbers, int num_of_nums)
{
	if (dir == DB)
	{
		add_db(&data, numbers, num_of_nums);
	}
	else if (dir == DH)
	{
		add_dh(&data, numbers, num_of_nums);
	}
	else if (dir == DW)
	{
		add_dw(&data, numbers, num_of_nums);
	}
	else if (dir == ASCIZ)
	{
		add_asciz(&data, numbers, num_of_nums);
	}
}



int read_line(FILE *fd, char *str_res)
{
	int c;
	int count = 0;

	/*read first char*/
	c  = getc(fd);

	while(c !=EOF && c!='\n'){

		if (count < MAXLEN)
			str_res[count] = c;

		count++;
		c  = getc(fd);

	}

	/*add end of string char to str_res*/
	if (count < MAXLEN)
		str_res[count] = '\0';
	else
		str_res[MAXLEN] = '\0';

	if(c == EOF)
	{
		return -1;
	}
	else if(count > MAXLEN){
		return 0;
	}

	return 1;
}


void first_run(FILE *fd){

	int result;
	int line_counter = 1;
	char line[MAXLEN + 1];

	/*init data structures*/
	init_symbol_table(&table);
	init_data(&data);
	init_data(&inst);

	/*init is file ok*/
	is_file_ok = 1;

	/*loop over file*/
	result = read_line(fd, line);

	while(result != -1)
	{
		if(result == 0)
		{
			printf("Line %d: length is over 80\n", line_counter);
			is_file_ok = 0;
		}
		else
		{
			line_type type;
			type = check_type(line);/*sending the correct line to check it's type*/

			if (type == DIRECTION)
			{
				/*parse direction here*/
				error_code res_dir_parser;
				direction_type dir;
				char label[MAXLEN];
				int nums[MAXLEN];
				int num_of_nums;

				res_dir_parser = parse_direction(line,&dir,label, nums, &num_of_nums);

				if(res_dir_parser != OK)
				{
					error_handler(res_dir_parser,line_counter);
					is_file_ok = 0;
				}
				else
				{

					/*try to add the label to the symbol table*/
					if(!add_label_to_table(label, 0, dir, 0))
					{
						is_file_ok = 0;
						error_handler(LABEL_REDIFINITION,line_counter);
					}

					/*add the direction to the data table*/
					add_direction_to_table(dir,nums,num_of_nums);
				}


			}
			else if (type == INSTRUCTION)
			{
				/*parse instruction here*/
				error_code res_inst_parser;
				instruction_type type;
				char label[MAXLEN];
				char cmd_label[MAXLEN];
				int val1, val2, val3;

				res_inst_parser = parse_instruction(line, &type,label,&val1,&val2,&val3, cmd_label);

				if (res_inst_parser !=OK) /*handle errors*/
				{
					error_handler(res_inst_parser,line_counter);
					is_file_ok = 0;
				}
				else
				{
					/*add the instruction to the instruction table*/
					if (is_file_ok)/*only add instruction if the file can compile*/
					{

						/*try to add the label to the symbol table*/
						if(!add_label_to_table(label, 1, 0, type))
						{
							is_file_ok = 0;
							error_handler(LABEL_REDIFINITION,line_counter);
						}

						/*add the instruction to the data instruction table*/
						add_instruction_to_table_first(type, val1, val2, val3, cmd_label);
					}

				}

			}
		}

		result = read_line(fd, line);
		line_counter++;
	}


	update_symbol_table(&table, BASE_ADDRESS + inst.counter);
}

void second_run(FILE *fd, char * file_name)
{
	/*define database for extern and entry labels*/
	extern_entry_table ext, ent;

	init_extern_entry_table(&ext);
	init_extern_entry_table(&ent);

	/*go to the beginning of the file*/
	fseek(fd, 0, SEEK_SET);

	/*go over the code again (only if it can compile)*/
	/*during the second run no error checking is done - except for label definition*/
	if(is_file_ok)
	{

		/*loop over file*/
		int result;
		int line_counter = 1;
		char line[MAXLEN + 1];
		int ic = 0, dc = 0;

		result = read_line(fd, line);


		while(result!=-1)
		{
			line_type type;

			type = check_type(line);

			if (type == DIRECTION)
			{
				/*parse direction here*/
				direction_type dir_type;
				char label[MAXLEN];
				int nums[MAXLEN];
				int num_of_nums;

				parse_direction(line,&dir_type,label, nums, &num_of_nums);

				if (dir_type == DB || dir_type == DH || dir_type == DW || dir_type == ASCIZ)
				{
					/*if this is a label definition which is entry add it to the ent table*/
					if(strlen(label)>0)
					{
						unsigned char lbl_attribute = get_attribute(&table, label);

						if(lbl_attribute & 8)
						{
							add_extern_entry_label(&ent, label, BASE_ADDRESS + inst.counter + dc);
						}
					}
				}

				/*progress data counter*/
				if(dir_type == DB)
				{
					dc+=num_of_nums;
				}
				else if(dir_type == DH)
				{
					dc+=num_of_nums*2;
				}
				else if(dir_type == DW)
				{
					dc+=num_of_nums*4;
				}
				else if(dir_type == ASCIZ)
				{
					dc+=num_of_nums + 1;
				}

			}
			else if (type == INSTRUCTION)
			{
				instruction_type inst_type;
				char label[MAXLEN];
				char cmd_label[MAXLEN];
				int val1, val2, val3;

				parse_instruction(line, &inst_type,label,&val1,&val2,&val3, cmd_label);

				/*if this is a label definition which is entry add it to the ent table*/
				if(strlen(label)>0)
				{
					unsigned char lbl_attribute = get_attribute(&table, label);

					if(lbl_attribute & 8)
					{
						add_extern_entry_label(&ent, label, BASE_ADDRESS + ic*4);
					}
				}

				if (strlen(cmd_label)>0)
				{
					/*check if the cmd_label is defined in the symbol table*/
					if(check_label(&table, cmd_label))
					{
						unsigned int lbl_address = 0;
						unsigned char lbl_attribute = 0;

						lbl_address = get_symbol_address(&table, cmd_label);
						lbl_attribute = get_attribute(&table, cmd_label);


						if (inst_type == BEQ || inst_type == BNE || inst_type == BLT || inst_type == BGT )
						{
							if(!update_branch_cmd(&inst, ic, lbl_address, BASE_ADDRESS + ic*4))
							{
								is_file_ok = 0;
								error_handler(UNDEFINED_LABEL,line_counter);
							}
						}
						else if (inst_type == JMP || inst_type == LA || inst_type == CALL)
						{
							update_j_cmd(&inst, ic, lbl_address);
						}

						/*if the label is external add this insturction's address to ext table*/
						if (lbl_attribute & 4)
						{
							/*add this label item to extern table*/
							add_extern_entry_label(&ext, cmd_label, BASE_ADDRESS + ic*4);
						}

					}
					else /*trying to use an undefined label*/
					{
						is_file_ok = 0;
						error_handler(UNDEFINED_LABEL,line_counter);
					}
				}

				ic++;

			}

			result = read_line(fd, line);
			line_counter++;
		}

	}


	if(is_file_ok)
	{

		if (ext.table_len > 0 )
		{
			export_extern_entry_table(file_name, F_EXTERN, &ext);
		}

		if (ent.table_len > 0 )
		{
			export_extern_entry_table(file_name, F_ENTRY, &ent);
		}

		export_data_tables(file_name, &inst, &data );
	}

	/*release all dynamically allocated memory*/
	free_symbol_table(&table);
	free_data_table(&data);
	free_data_table(&inst);
	release_extern_entry_table(&ext);
	release_extern_entry_table(&ent);

	/*reset globals*/
	is_file_ok = 1;

}

