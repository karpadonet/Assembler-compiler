
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "extern_entry.h"
#include "common.h"


void init_extern_entry_table(extern_entry_table *table)
{
	table->table = NULL;
	table->table_len = 0;
}

void add_extern_entry_label(extern_entry_table *table, char *label, unsigned int address)
{
	extern_entry *row;
	/*add a new element in position table_len+1 */
		table->table_len+=1;

		/*if the data structure is empty, table->table = NULL, allocate memory using malloc*/
		if (table->table == NULL)
		{
			table->table = (extern_entry*) malloc(sizeof(extern_entry));

			if(table->table == NULL)
			{
				printf("Failed to allocate memory. Exiting program\n");
				exit(1);
			}
		}
		else /*if not empty allocate more memory using realloc */
		{
			table->table = (extern_entry*)realloc(table->table,table->table_len*sizeof(extern_entry));

		}

		/*update the data in the newly allocated row*/
		row = table->table;
		strcpy((row + table->table_len - 1 )->label,label);
		(row + table->table_len - 1 )->address = address;

}

void release_extern_entry_table(extern_entry_table *table)
{
	free(table->table);
	init_extern_entry_table(table);
}

void export_extern_entry_table(char * file_name, extern_entry_type file_type, extern_entry_table *table)
{
	FILE *fd;
	char file_exp_name[255]="";
	int i = 0;
	extern_entry *row = table->table;

	strncpy(file_exp_name, file_name, strlen(file_name) - 3);

	if(file_type == F_EXTERN)
	{
		strcat(file_exp_name, ".ext");
	}
	else
	{
		strcat(file_exp_name, ".ent");
	}


	fd = fopen(file_exp_name, "w");

	if(fd!=NULL)
	{
		while(i < table->table_len)
		{
			char temp[100] = "";
			char addr[5];

			strcat(temp,(row + i)->label);
			strcat(temp," ");

			num2str_padded((row + i)->address, addr);

			strcat(temp, addr);
			strcat(temp,"\n");


			fputs(temp,fd);
			i++;
		}

	fclose(fd);
	}
}

void print_extern_entry_table(extern_entry_table *table)
{
	int count = 0;
	extern_entry *table_p = table->table;

		while(count < table->table_len)
		{

			printf("label id is %d ", count);
			printf("The label name is: %s ", (table_p+count)->label);
			printf("The label value is: %d\n", (table_p+count)->address);

			count++;

		}
}
