#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "data_handler.h"
#include "common.h"


void init_data(data_table *table)
{
	table->counter = 0;
	table->data = NULL;
}

void allocate_table(data_table *table, unsigned int n)
{
	/*add a block of memory the size of n bytes */
	table->counter+=n;

	if (table->data == NULL)
	{
		table->data = (unsigned char*) malloc(n*sizeof(unsigned char));
		if(table->data == NULL)
		{
			printf("Failed to allocate memory. Exiting program\n");
			exit(1);
		}
	}
	else /*if not empty allocate more memory using realloc */
	{
		table->data = (unsigned char*)realloc(table->data,table->counter*sizeof(unsigned char));

	}
}

void write_to_cell(data_table *table, unsigned char x, unsigned int cell)
{
	if(cell < table->counter)
	{
		*(table->data + cell) = x;
	}
	else
	{
		printf("Out of range");
	}

}

void free_data_table(data_table *table){

		free(table->data);
		init_data(table);

}

void add_db(data_table  *table, int * numbers, int n)
{
	int i,k;
	int data_size;

	data_size = n;

	/*allocate n bytes to the memory and advance the table size counter*/
	allocate_table(table, data_size);

	for(i = 0, k = table->counter-data_size ; i < n ; i++,k++)
	{
		*(table->data + k) = numbers[i];
	}

}

void add_dh(data_table  *table, int * numbers, int n)
{
	int i,k;
	int data_size;

	data_size = 2*n;

	/*allocate n bytes to the memory and advance the table size counter*/
	allocate_table(table, data_size);

	for(i = 0, k = table->counter-data_size ; i < n ; i++,k+=2)
	{
		*(table->data + k) = (numbers[i] & 0xFF);
		*(table->data + k + 1) = ((numbers[i] >> 8) & 0xFF);
	}
}

void add_dw(data_table  *table, int * numbers, int n)
{
	int i,k;
	int data_size;

	data_size = 4*n;

	/*allocate n bytes to the memory and advance the table size counter*/
	allocate_table(table, data_size);

	for(i = 0, k = table->counter-data_size ; i < n ; i++,k+=4)
	{
		*(table->data + k) = (numbers[i] & 0xFF);
		*(table->data + k + 1) = ((numbers[i] >> 8) & 0xFF);
		*(table->data + k + 2) = ((numbers[i] >> 16) & 0xFF);
		*(table->data + k + 3) = ((numbers[i] >> 24) & 0xFF);
	}

}

void add_asciz(data_table  *table, int * numbers, int n)
{

	int i,k;
	int data_size;

	data_size = n + 1;

	/*allocate n bytes to the memory and advance the table size counter*/
	allocate_table(table, data_size);

	for(i = 0, k = table->counter-data_size ; i < n ; i++,k++)
	{
		*(table->data + k) = numbers[i];
	}

	/*add end of string char*/
	*(table->data + k) = '\0';
}



void add_arithm_cmd(data_table *table, instruction_type type, int _rs, int _rt, int _rd)
{
	unsigned int temp = 0,funct,rs,rt,rd,opcode;
	int i,k;

	/*allocate four bytes to the memory*/
	allocate_table(table,4);

	/*translate the parameters to the temp variable*/
	opcode = 0;

	switch(type)
	{
	case ADD:
		funct = 1;
		break;
	case SUB:
		funct = 2;
		break;
	case AND:
		funct = 3;
		break;
	case OR:
		funct = 4;
		break;
	case NOR:
		funct = 5;
		break;
	default:
		funct = 1;
		break;
	}

	rs = _rs;
	rt = _rt;
	rd = _rd;

	temp |= (funct << 6);
	temp |= (rd  << 11);
	temp |= (rt << 16);
	temp |= (rs << 21);
	temp |= (opcode << 26);

	/*copy temp into the data table*/
	for(i = 0, k = table->counter-4 ; i < 4 ; i++,k++)
	{
		*(table->data + k) = ((temp >> (i*8)) & 0xFF);
	}

}

void add_move_cmd(data_table *table, instruction_type type, int _rs, int _rd)
{
	unsigned int temp = 0,funct,rs,rt = 0,rd,opcode;
	int i,k;

	/*allocate four bytes to the memory*/
	allocate_table(table,4);

	/*translate the parameters to the temp variable*/
	opcode = 1;

	switch(type)
	{
	case MOVE:
		funct = 1;
		break;
	case MVHI:
		funct = 2;
		break;
	case MVLO:
		funct = 3;
		break;
	default:
		funct = 1;
		break;
	}

	rs = _rs;
	rd = _rd;

	temp |= (funct << 6);
	temp |= (rd  << 11);
	temp |= (rt << 16);
	temp |= (rs << 21);
	temp |= (opcode << 26);

	/*copy temp into the data table*/
	for(i = 0, k = table->counter-4 ; i < 4 ; i++,k++)
	{
		*(table->data + k) = ((temp >> (i*8)) & 0xFF);
	}

}

void add_arithm_i_cmd(data_table *table, instruction_type type, int _rs, int _rt, int _immed)
{
	unsigned int temp = 0, opcode, rs,rt, immed;
	int i,k;

	/*allocate four bytes to the memory*/
	allocate_table(table,4);

	/*translate the parameters to the temp variable*/
	switch(type)
	{
		case ADDI:
			opcode = 10;
			break;
		case SUBI:
			opcode = 11;
			break;
		case ANDI:
			opcode = 12;
			break;
		case ORI:
			opcode = 13;
			break;
		case NORI:
			opcode = 14;
			break;
		default:
			opcode = 10;
			break;
	}

	rs = _rs;
	rt = _rt;
	immed = _immed;

	temp|= (immed & 0xFFFF);
	temp|=(rt << 16);
	temp|=(rs << 21);
	temp|=(opcode << 26);

	/*copy temp into the data table*/
	for(i = 0, k = table->counter-4 ; i < 4 ; i++,k++)
	{
		*(table->data + k) = ((temp >> (i*8)) & 0xFF);
	}

}


void add_branch_cmd(data_table *table, instruction_type type, int _rs, int _rt)
{
	unsigned int temp = 0, opcode, rs,rt, immed;
	int i,k;

	/*allocate four bytes to the memory*/
	allocate_table(table,4);

	/*translate the parameters to the temp variable*/
	switch(type)
	{
	case BEQ:
		opcode = 16;
		break;
	case BNE:
		opcode = 15;
		break;
	case BLT:
		opcode = 17;
		break;
	case BGT:
		opcode = 18;
		break;
	default:
		opcode = 15;
		break;
	}

	rs = _rs;
	rt = _rt;
	immed = 0;

	temp|=immed;
	temp|=(rt << 16);
	temp|=(rs << 21);
	temp|=(opcode << 26);

	/*copy temp into the data table*/
	for(i = 0, k = table->counter-4 ; i < 4 ; i++,k++)
	{
		*(table->data + k) = ((temp >> (i*8)) & 0xFF);
	}

}


void add_store_cmd(data_table * table, instruction_type type, int _rs,  int _rt, int _immed)
{
	unsigned int temp = 0, opcode, rs,rt, immed;
	int i,k;

	/*allocate four bytes to the memory*/
	allocate_table(table,4);

	/*translate the parameters to the temp variable*/
	switch(type)
	{
	case LB:
		opcode = 19;
		break;
	case SB:
		opcode = 20;
		break;
	case LW:
		opcode = 21;
		break;
	case SW:
		opcode = 22;
		break;
	case LH:
		opcode = 23;
		break;
	case SH:
		opcode = 24;
		break;
	default:
		opcode = 19;
		break;
	}

	rs = _rs;
	rt = _rt;
	immed = _immed;

	temp|= (immed & 0xFFFF);
	temp|=(rt << 16);
	temp|=(rs << 21);
	temp|=(opcode << 26);

	/*copy temp into the data table*/
	for(i = 0, k = table->counter-4 ; i < 4 ; i++,k++)
	{
		*(table->data + k) = ((temp >> (i*8)) & 0xFF);
	}

}

void add_j_cmd(data_table * table, instruction_type type, int _rs, char * label)
{
	unsigned int temp = 0, opcode, reg, address;
	int i,k;

	/*allocate four bytes to the memory*/
	allocate_table(table,4);

	/*translate the parameters to the temp variable*/
	switch(type)
	{
	case JMP:
		opcode = 30;
		break;
	case LA:
		opcode = 31;
		break;
	case CALL:
		opcode = 32;
		break;
	default:
		opcode = 30;
		break;
	}

 if(strlen(label)==0)
 {
	 reg = 1;
	 address = _rs;
 }
 else /*do not assign a value to the label at this stage*/
 {
	 reg = 0;
	 address = 0;
 }

 temp = address;
 temp |= (reg << 25);
 temp |= (opcode << 26);

 /*copy temp into the data table*/
 for(i = 0, k = table->counter-4 ; i < 4 ; i++,k++)
 {
	 *(table->data + k) = ((temp >> (i*8)) & 0xFF);
 }

}


void add_stop_cmd(data_table * table)
{
	unsigned int temp = 0, opcode = 63;
	int i,k;

	/*allocate four bytes to the memory*/
	allocate_table(table,4);

	temp|=(opcode << 26);

	 /*copy temp into the data table*/
	 for(i = 0, k = table->counter-4 ; i < 4 ; i++,k++)
	 {
		 *(table->data + k) = ((temp >> (i*8)) & 0xFF);
	 }
}

void update_j_cmd(data_table * table, int ic, unsigned int value )
{
	unsigned int temp = 0;

	/*copy the value of the data field into temp*/

	temp = *(table->data + ic*4);

	temp|= value;

	*(table->data + ic*4) = (temp & 0xFF);
	*(table->data + ic*4 + 1) = ((temp >> 8) & 0xFF);
	*(table->data + ic*4 + 2) = ((temp >> 16) & 0xFF);

}



int update_branch_cmd(data_table * table, int ic, unsigned int value , unsigned int curr_address)
{
	unsigned int temp = 0;
	int jump_distance = 0;
	unsigned int immed;

	jump_distance = value - curr_address;
	immed = ((value - curr_address) & 0xFFFF);

	if (jump_distance> 32767 || jump_distance < - 32768)
	{
		return 0;
	}

	/*copy the value of the data field into temp*/

	temp = *(table->data + ic*4);

	temp|= immed;

	*(table->data + ic*4) = (temp & 0xFF);
	*(table->data + ic*4 + 1) = ((temp >> 8) & 0xFF);

	return 1;
}


void export_data_tables(char * file_name, data_table *inst_table, data_table *data_table )
{
	FILE *fd;
	char file_exp_name[255]="";
	int add_counter = 0;

	strncpy(file_exp_name, file_name, strlen(file_name) - 3);
	strcat(file_exp_name, ".ob");

	fd = fopen(file_exp_name, "w");

	/*export the contents of the data and inst tables to the file*/
	if (fd != NULL)
	{
		char temp[100] = "";
		char num_str[10] = "";


		/*first write the instruction and data size*/
		strcat(temp,"     "); /*add five spaces*/
		num2str(inst_table->counter,num_str);
		strcat(temp,num_str);
		strcat(temp," ");
		num2str(data_table->counter,num_str);
		strcat(temp,num_str);
		strcat(temp,"\n");

		fputs(temp,fd);

		/*write the hexa data in the inst_table*/
		while( add_counter < inst_table->counter)
		{
			int j = 0;

			strcpy(temp,"");
			num2str_padded(BASE_ADD + add_counter,num_str);
			strcat(temp,num_str);

			for(j = 0 ; j < 4 ; j++)
			{
				strcat(temp, " ");
				num2hex(*(inst_table->data + add_counter + j),num_str);
				strcat(temp,num_str);
			}

			strcat(temp,"\n");
			fputs(temp,fd);
			add_counter+=4;
		}

		/*write the hexa data in the data_table*/
		while( add_counter < inst_table->counter + data_table->counter)
		{
			int j = 0;

			strcpy(temp,"");
			num2str_padded(BASE_ADD + add_counter,num_str);
			strcat(temp,num_str);

			for(j = 0 ; j < 4 && (add_counter + j) < inst_table->counter + data_table->counter ; j++)
			{
				strcat(temp, " ");
				num2hex(*(data_table->data + add_counter - inst_table->counter + j),num_str);
				strcat(temp,num_str);
			}

			strcat(temp,"\n");
			fputs(temp,fd);
			add_counter+=4;
		}

		/*close file*/
		fclose(fd);

	}


}



void print_data_table(data_table  *table)
{
	int i;

	for (i = 0; i < table->counter ; i++)
	{
		printf("data at loc %d: %d\n",i, *(table->data + i));

	}

	printf("Overall buffer size %d\n",table->counter);

}


