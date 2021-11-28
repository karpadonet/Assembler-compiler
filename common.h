
#ifndef COMMON_H_
#define COMMON_H_

#define BASE_ADD 100
#define MAXLEN 80
#define MAX_DB 127
#define MIN_DB -128
#define MAX_DW 32767
#define MIN_DW -32768
#define MAX_DH 2147483647
#define MIN_DH -2147483647

/*Function num2hex converts the given byte value to a string which represent a hexadecimal value
 *
 *Parameters:
 * num - the given number
 * num_str - the resulting string
 *
 */
void num2hex(unsigned char num,char * num_str );


/*Function num2strconverts the given number to a string
 *
 *Parameters:
 * num - the given number
 * num_str - the resulting string
 *
 */
void num2str(unsigned int num,char * num_str );


/*Function num2str_padded converts the given number to a four digit string
 *
 *Parameters:
 * num - the given number
 * num_str - the resulting string
 *
 */
void num2str_padded(unsigned int num,char * num_str );

/* This function receives a string which starts with a number. The function reads the number
 * until the next space, comma character or end of string, and removes the number from
 * the given string.
 *
 * Parameters:
 * str - The given string
 * num - The number converted from the string.
 *
 * Returns:
 * 1 if the number was converted successfully, 0- otherwise
 */
int str2num(char * str, int * num);

/*This function removes n characters from a given string
 *
 * Parameters-
 * line- a pointer to a given string
 * n - number of characters to remove
 * */
void remove_chars(char *line, int n);

/*This function removes all the spaces and tabs in the start of a given string
 *
 * Parameters:
 * line - a pointer to the start of the line
 * */
void remove_spaces(char *line);


/*This function function checks if there is a full stop (.)
 * character in the string.
 *
 * Parameters:
 * line - the given string
 *
 * Returns:
 * 1 - if a dot is found, 0 - otherwise
 * */

int check_dot(char *line);

/* The function check_two_dots check if there is a colon character in the string.
 *
 * Parameters:
 * line - the given string
 *
 * Returns:
 * 0 for no, 1 for yes.
 *
 */
int check_two_dots(char *line);

/* Function check_commas checks how many commas there are at the start of the given string.
 *The function counts commas until the next non space or tab character.
 *Additionally, the function removes all the characters from the string until the last comma.
 *
 *Parameters:
 *str - the given strin
 *
 *Returns:
 *The number of commas.
 */
int check_commas(char * str);



#endif /* COMMON_H_ */
