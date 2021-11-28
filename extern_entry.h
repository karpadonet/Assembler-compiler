
typedef enum extern_entry_type{F_EXTERN, F_ENTRY} extern_entry_type;
/* a line in a extern_entry table.
 * Each line in the table contains:
 * label - a string which holds the label's name
 * address- which holds the address of the label
 * */
typedef struct extern_entry {
	char label[80];
	unsigned int address;
} extern_entry;

/* extern_entry table.
 * table points to the table
 * table_len is the table's length
 * */
typedef struct extern_entry_table{
	extern_entry *table;
	unsigned int table_len;
}extern_entry_table;

/* initiation of data structure containing the
 * extern_entry table.
 *
 * Parameters;
 * table - a pointer to a symbol table
 */
void init_extern_entry_table(extern_entry_table *table);

/* The function adds a label to the extern_entry table.
 *
 * Parameters;
 * table - a pointer to the table
 * label - label's name
 * adress- label's address
 *
 */
void add_extern_entry_label(extern_entry_table *table, char *label, unsigned int address);

/*This function frees the memory that was allocated for the
 * extern_entry table*/
void release_extern_entry_table(extern_entry_table *table);

/*Function export_extern_entry_table exports the given table to a file according to the file_type flag
 *
 * Parameters:
 * file_name - the name of the source file
 * file_type - type of export file: .ext or .ent
 * table - the given table
 *
 */
void export_extern_entry_table(char * file_name, extern_entry_type file_type, extern_entry_table *table);

/*Function print_extern_entry_table prints the given table to the console
 *The functions is used for debug.
 *
 *Parameters:
 *table - the table to be printed
 */
void print_extern_entry_table(extern_entry_table *table);

