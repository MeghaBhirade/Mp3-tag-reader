#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "tags.h"
#include "read.h"
#include "edit.h"

/*main function*/
int main(int argc, char *argv[])
{
	int option;
	char ch;
	
	TagRead tagread;
	TagEdit tagedit;

	/*check for argument count(a.out)*/
	if (argc == 1)
	{
		/*choose option from user*/
		printf("Select the option:\n\t1.Read\n\t2.Edit\nEnter option: ");
		scanf("%d", &option);
	}

	else
		/*Else pass from command line*/
		option = check_operation(&argv[1]);

	switch (option)
	{
		case read:
			/*read mp3 file*/
			printf("Enter the file name: ");
			scanf("%s", tagread.mp3_fname);

			/*function callto read and print tags*/
			get_tags(&tagread, argv, argc);
			break;

		case edit:

			/*read mp3 file*/
			printf("Enter the file name: ");
			scanf("%s", tagedit.mp3_fname);

         	copy_tag_names(&tagedit, argv,  argc);

			break;

		default:
			/*check for default case*/
			printf("Invalid option\n");
	}

	return e_success;

}
