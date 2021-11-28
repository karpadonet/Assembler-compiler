#define MAXLEN 80
#define BASE_ADDRESS 100


/*Function first run performs the first assembler run during which
 * it builds a template of the instruction and data image
 * before sending it for processing a second time.
 */
void first_run(FILE *fd);

/*Function second_run performs the second assmebler run during which
 * it completes the construction of the instruction and data image
 * and exports it to .ob .ext and .ent files
 */
void second_run(FILE *fd, char * file_name);


