


/* a line in a symbol table.
 * Each line in the table contains:
 * label - a string which holds the label's name
 * value - which holds the address to which the label points
 * attributes - the type of lines to which the label points:
 * bit 0  - code, bit 1 - data, bit 2 - extern, bit 3 - entry
 */
typedef struct symbol {
	char label[80];
	unsigned int value;
	unsigned char attributes;
} symbol;


typedef struct symbol_table {
	symbol *symb;
	int num_of_symbols;
} symbol_table;


/* initiation of data structure containing the
 * symbol table.
 *
 * Parameters;
 * table - a pointer to a symbol table
 */
void init_symbol_table(symbol_table *table);



/* The function add_symbol adds a label to the symbol table.
 *
 * Parameters;
 * table - a pointer to a symbol table
 * label - label's name
 * value- label's address
 * code, data, extern and entry are label's attributes
 */
void add_symbol(symbol_table * table, char * label,  unsigned int value, int code, int data,
		int external, int entry);


/*This function adds the attribute for the label
 * table - a pointer to the symbol_table
 * label - label's name
 * attribute- label's attribute: 0 - code, 1 - data, 2 - extern, 3 -entry
 *
 * */
void add_attribute(symbol_table *table, char *label, int attribute);


/*This function adds the attribute for the label
 * table - a pointer to the symbol_table
 * label - label's name
 * attribute- label's attribute: 0 - code, 1 - data, 2 - extern, 3 -entry
 *
 * */
void update_symbol_value(symbol_table *table, char * label, unsigned int value);


/*This function check's if the label appears in the table
 * table - a pointer to the symbol_table
 * label - label's name
 * for yes returns 1 , otherwise 0*/
int check_label(symbol_table *table, char *label);




/*Function get_attribute returns the attribute value of a label within
 * a symbol table
 *
 * Parameters:
 * table - the given symbol_table
 * label - the label we're searching for
 *
 * Returns:
 * attribute value of the label
 */
unsigned char get_attribute(symbol_table *table,
 char * label);


/*Function get_symbol_address returns the address value of a label within
 * a symbol table
 *
 * Parameters:
 * table - the given symbol_table
 * label - the label we're searching for
 *
 * Returns:
 * the stored address of the label
 */
unsigned int get_symbol_address(symbol_table * table, char * label);

/*This function frees the memory that was allocated for the
 * symbol table*/
void free_symbol_table(symbol_table *table);


/*This function updates the address of the data labels to be their stored address
 *  + inst_end_address
 *
 *Parameters:
 *table - the table to be updated
 *inst_end_address - the address in which the code ends
 */
void update_symbol_table(symbol_table *table, unsigned int inst_end_address);



/*Function print_table prints the given symbol table
 *
 * Parameters:
 * table - the symbol table to be printed
 *
 */
void print_table(symbol_table* table);
