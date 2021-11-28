#include <string.h>
#include "common.h"


void num2hex(unsigned char num, char * num_str)
{
	unsigned char temp[2];
	int i;

	/*convert the first digit*/
	temp[1] = num & 0xF;
	temp[0] = ((num & 0xF0) >> 4);

	for(i = 0; i<2 ; i++)
	{
		if (temp[i] >= 0 && temp[i] <= 9)
		{
			num_str[i] = '0' + temp[i];
		}
		else
		{
			num_str[i] = 'A' + temp[i] - 10;
		}
	}

	num_str[2] = '\0';
}

void num2str(unsigned int num,char * num_str )
{
	int num_len = 0, temp_num;

	if (num == 0)
	{
		num_len = 1;

		num_str[0] = '0';
		num_str[1] = '\0';

	}
	else
	{
		temp_num = num;

		while(temp_num!=0)
		{
			temp_num = temp_num / 10;
			num_len++;
		}

		num_str[num_len] = '\0';
		temp_num = num;

		while(num_len > 0)
		{
			num_len--;
			num_str[num_len] = temp_num % 10 + '0';
			temp_num = temp_num/10;
		}

	}

}

void num2str_padded(unsigned int num,char * num_str )
{
	int i;

	for(i= 0 ; i < 4; i++)
		num_str[i]= '0';
	num_str[4] = '\0';

	i = 3;
	while(i>=0 )
	{
		num_str[i] = num % 10 + '0';
		num = num/10;
		i--;
	}
}


int str2num(char * str, int * num)
{
	int i=0, sign = 1;

	*num = 0;

	/*check sign of number*/
	if(str[0]!='\0' && str[0]== '-')
	{
		i++;
		sign = -1;
	}

	while(str[i]!='\0' && str[i] >='0' && str[i] <='9')
	{
		*num = 10*(*num) +(str[i] - '0');
		i++;
	}

	if ((str[i]!='\0') && (str[i]!=',') && (str[i]!=' ') && (str[i]!='\t'))
	{
		return 0;
	}

	*num *= sign;

	remove_chars(str, i);

	return 1;

}

void remove_chars(char *line, int n)
{
	int i = 0;
	int k = n;

	for(; k < strlen(line) ; k++ , i++)
	{
		line[i] = line[k];
	}
	line[i] = '\0';
}


int check_commas(char * str)
{
	int i = 0;
	int count = 0;

	while ((str[i] == ',' || str[i] == ' ' || str[i]=='\t') && str[i]!='\0')
	{
		if (str[i]==',')
		{
			count++;
		}
		i++;
	}


	remove_chars(str, i);

	return count;

}


void remove_spaces(char *line)
{

	int i = 0;
	int k = 0;
	while(line[i] == '\t' || line[i] == ' ')
	{
		i++;
	}

	k = i;
	while(line[k] != '\0')
	{
		line[k - i] = line[k];
		k++;
	}

	line[k - i] = '\0';
}


int check_dot(char *line)
{
	int i=0;

	while(line[i]!='\0')
	{
		if (line[i] == '.')
		{
			return 1;
		}

		i++;
	}

	return 0;

}


int check_two_dots(char *line)
{
	int i=0;

	while(line[i]!='\0')
	{
		if (line[i] == ':')
		{
			return 1;
		}

		i++;
	}

	return 0;

}
