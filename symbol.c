#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symbol.h"


void init_symbol_table(symbol_table *table)
{

	table->symb = NULL;
	table->num_of_symbols = 0;
}

void add_symbol(symbol_table * table, char * label,  unsigned int value, int code, int data,
		int external, int entry)
{
	symbol * row;
	unsigned int val;

	/*add a new element in position num_of_symbols+1 */
	table->num_of_symbols+=1;

	/*if the data structure is empty, table->symb = NULL, allocate memory using malloc*/
	if (table->symb == NULL)
	{
		table->symb = (symbol*) malloc(sizeof(symbol));

		if(table->symb == NULL)
		{
			printf("Failed to allocate memory. Exiting program\n");
			exit(1);
		}
	}
	else /*if not empty allocate more memory using realloc */
	{
		table->symb = (symbol*)realloc(table->symb,table->num_of_symbols*sizeof(symbol));

	}

	/*calc value*/
	if (external || entry)
		val = 0;
	else
		val = value;

	/*update the data in the newly allocated row*/
	row = table->symb;
	strcpy((row + table->num_of_symbols - 1 )->label,label);
	(row + table->num_of_symbols - 1 )->value = val;
	(row + table->num_of_symbols - 1 )->attributes = code;
	(row + table->num_of_symbols - 1 )->attributes |= (data << 1);
	(row + table->num_of_symbols - 1 )->attributes |= (external << 2);
	(row + table->num_of_symbols - 1 )->attributes |= (entry << 3);


}

int check_label(symbol_table *table, char *label)
{
	symbol *row;
	int i = 0;

	row = table->symb;

	/*looping over the table and checking if the label already
	 * appears there, if yes we will return 0*/
	for(; i < table->num_of_symbols; i++)
	{
		int result;
		result = strcmp ((row + i)->label, label);
		if(!result)
		{
			return 1;
		}
	}


	return 0;
}


void add_attribute(symbol_table *table, char *label, int attribute)
{
	symbol *row;
	int i = 0;

	row = table->symb;

		/*finding the label in the table and adding the attribute*/
		for(; i < table->num_of_symbols; i++)
		{
				if(!strcmp ((row + i)->label, label))
				{
					switch(attribute)
					{
					case 0:
						(row + i)->attributes |= 1;
						break;
					case 1:
						(row + i)->attributes |= (1<<1);
						break;
					case 2:
						(row + i)->attributes |= (1<<2);
						break;
					default:
						(row + i)->attributes |= (1<<3);
						break;

					}


				}
		}

}


void free_symbol_table(symbol_table *table){

		free(table->symb);
		init_symbol_table(table);

}


unsigned char get_attribute(symbol_table *table,
 char * label)
{
	unsigned char ret = 0;
	symbol * row;
	unsigned int i = 0;

	row = table->symb;

	while(i < table->num_of_symbols)
	{
		if (!strcmp(label, (row + i)->label))
		{
			ret = (row + i)->attributes;
			break;
		}
		i++;
	}

	return ret;
}


void update_symbol_table(symbol_table *table, unsigned int inst_end_address)
{
	int count  = 0;
	symbol * symb_p = table->symb;

	while(count < table->num_of_symbols)
	{
		if ((symb_p + count)->attributes & 2) /*if this is a data attribute*/
		{
			(symb_p + count)->value += inst_end_address;
		}

		count++;
	}

}


void update_symbol_value(symbol_table *table, char * label, unsigned int value)
{
	int count  = 0;
	symbol * symb_p = table->symb;

	while(count <  table->num_of_symbols)
	{
		if(!strcmp( (symb_p + count)->label,label))
		{
			(symb_p + count)->value = value;
			break;
		}
		count++;
	}

}


unsigned int get_symbol_address(symbol_table * table, char * label)
{
	unsigned int res = 0;
	symbol * row;
	unsigned int i = 0;

	row = table->symb;

	while(i < table->num_of_symbols)
	{
		if (!strcmp(label, (row + i)->label))
		{
			res = (row + i)->value;
			break;
		}
		i++;
	}

	return res;
}


void print_table(symbol_table* table)
{
	int count = 0;
	symbol * symb_p = table->symb;

	while(count < table->num_of_symbols)
	{

		printf("label id is %d\n", count);
		printf("The label name is: %s ", (symb_p+count)->label);
		printf("The label value is: %d ", (symb_p+count)->value);
		printf("The label attribute  is: %d\n", (symb_p+count)->attributes);

		count++;

	}

}
