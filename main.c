#include <stdio.h>
#include <string.h>
#include "runner.h"

int main(int argc, char *argv[]){


	int i = 1;
	for(;i < argc ; i++)/*main loop to go over files*/
	{
		/*checking the files name length to use it later*/
		int len = 0;
		len =  strlen(argv[i]);

		if(len >= 4){
		/*check if file name is legal*/


			if(strcmp((argv[i] + len - 3), ".as") == 0)
			{
				/*check if file exists*/
	 				FILE *file;
	 				file = fopen(argv[i], "r");
	 				if (file){

	 					/*if ok proceed with assembler*/
	 					printf("Running assembler for file: %s\n",argv[i]);

	 					first_run(file);

	 					second_run(file,argv[i]);

	 					fclose(file);
	 				}

	 				else{
	 					printf("File %s is not accessible\n",argv[i]);
	 				}
			}
			else
			{
				printf("File name %s is illegal\n",argv[i]);
			}
		}
		else{
			printf("File name %s is illegal\n",argv[i]);
		}

	}


	printf("Completed running for all files\n");
	return 0;


}


