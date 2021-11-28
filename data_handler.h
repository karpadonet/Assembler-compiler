#include "parser.h"

/*This structure is the data table that contains
 * the pointer to the data - data
 * and a counter to allocate the place the data is stored at*/
typedef struct data_table{
	unsigned char *data;
	unsigned int counter;
}data_table;

/*This function initializes the data table*/
void init_data(data_table *table);

/*This function adds more memory space for the table as needed*/
void void_allocate_table(data_table *table, unsigned int n);

/*This function allocates the char x in the table
 * table - the pointer to the data table
 * cell- the place we will allocate the x char at
 * x - the char we need to allocate*/
void write_to_cell(data_table *table, unsigned char x, unsigned int cell);

/*This function frees the memory that was allocated for the
 * data table*
 *
 * Parameters:
 * table - The table to de-allocate
 */
void free_data_table(data_table *table);


/*Function add_dh adds two byte n numbers from numbers into the data_table structure,
 * and updates the data_counter accordingly.
 *
 * Parameters:
 * table - the table to be updated
 * numbers - an array of numbers
 * n - the number of numbers to copy
 */
void add_dh(data_table  *table, int * numbers, int n);

/*Function add_dw adds four byte n numbers from numbers into the data_table structure,
 * and updates the data_counter accordingly.
 *
 * Parameters:
 * table - the table to be updated
 * numbers - an array of numbers
 * n - the number of numbers to copy
 */
void add_dw(data_table  *table, int * numbers, int n);

/*Function add_db adds 1 byte n numbers from numbers into the data_table structure,
 * and updates the data_counter accordingly.
 *
 * Parameters:
 * table - the table to be updated
 * numbers - an array of numbers
 * n - the number of numbers to copy
 */
void add_db(data_table  *table, int * numbers, int n);


/*Function add_asciz adds a string to the data_atble by adding each number as a char (1 byte)
 * and allocating an additional byte for end of string.
 *
 * Parameters:
 * table - the table to be updated
 * numbers - an array of numbers
 * n - the number of numbers to copy
 */
void add_asciz(data_table  *table, int * numbers, int n);


/* The function add_arith_cmd adds an arithmetic assembler command to a an instruction data table
 *
 * Parameters:
 * table - the data structure to which the data is added
 * type - the type of the command
 * _rs - the first register
 * _rt - the second register
 * _rd - the destination register
 */
void add_arithm_cmd(data_table *table, instruction_type type, int _rs, int _rt, int _rd);

/* The function add_move_cmd adds a move assembler command to a an instruction data table
 *
 * Parameters:
 * table - the data structure to which the data is added
 * type - the type of the command
 * _rs - the first register
 * _rd - the destination register
 */
void add_move_cmd(data_table *table, instruction_type type, int _rs, int _rd);

/* The function add_arithm_i_cmd adds an arithmetic assembler command to a an instruction data table
 *
 * Parameters:
 * table - the data structure to which the data is added
 * type - the type of the command
 * _rs - the first register
 * _rt - the second register
 * _immed - the given immediate value
 */
void add_arithm_i_cmd(data_table *table, instruction_type type, int _rs, int _rt, int immed);

/* The function add_branch_cmd adds a branch assembler command to a an instruction data table
 *
 * Parameters:
 * table - the data structure to which the data is added
 * type - the type of the command
 * _rs - the first register
 * _rt - the second register
 *
 * Note: The jump to address is set to 0.
 */
void add_branch_cmd(data_table *table, instruction_type type, int _rs, int _rt);


/* The function add_store_cmd adds a branch assembler command to a an instruction data table
 *
 * Parameters:
 * table - the data structure to which the data is added
 * type - the type of the command
 * _rs - the first register
 * _immed - the immediate value
 * _rt - the second register
 */
void add_store_cmd(data_table * table, instruction_type type, int _rs, int _rt, int immed);


/* The function add_j_cmd adds a jump assembler command to a an instruction data table
 *
 * Parameters:
 * table - the data structure to which the data is added
 * type - the type of the command
 * _rs - the register value
 * label - the label to which to jump
 */
void add_j_cmd(data_table * table, instruction_type type, int _rs, char * label);


/* The function add_stop_cmd adds a stop assembler command to a an instruction data table
 *
 *Parameters:
 * table - the data structure to which the data is added
 */
void add_stop_cmd(data_table * table);

/*The function update_j_cmd updates the value of the immed field for the instruction in place
 * ic*4
 *
 * Parameters:
 * table - the data table to be updated
 * ic - instruction counter
 * value - the address value to be added to the table
 */
void update_j_cmd(data_table * table, int ic, unsigned int value );

/*The function update_branch_cmd updates the value of the immed field for the instruction in place
 * ic*4
 *
 * Parameters:
 * table - the data table to be updated
 * ic - the current instruction counter
 * value - the address value to be added to the table
 * curr_address - the address of the current instruction
 *
 * Returns:
 * 1 - if the jump is possible, 0 otherwise
 */
int update_branch_cmd(data_table * table, int ic, unsigned int value , unsigned int curr_address);

/* Function export_data_tables exports the instruction and data table to an .ob file
 *
 * Parameters:
 * file_name - the file name of the source file
 * inst_table - the instruction data table
 * data_table - the data data table
 */
void export_data_tables(char * file_name, data_table *inst_table, data_table *data_table );

/*Function print_data_table prints the given data table to the console.
 * This function is used for debug only.
 *
 * Parameters:
 * table - a data table
 *
 */
void print_data_table(data_table  *table);
