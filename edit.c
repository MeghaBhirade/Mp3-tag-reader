#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "edit.h"
#include "tags.h"
#include "types.h"


/*Read and validate argument parameters*/
Status read_and_validate_edit_args(char *argv[],TagEdit *tagedit)
{
	strcpy(tagedit -> mp3_fname,  argv[3]);

	printf("%s", tagedit->mp3_fname);
	
	return e_success;
}
/*open files for editing*/
Status open_edit_files(TagEdit *tagedit)
{
	tagedit -> fptr_mp3 = fopen(tagedit ->mp3_fname, "r+" );

	/*Error checking*/
	if (tagedit -> fptr_mp3 == NULL)
	{
		perror("fopen");
		fprintf(stderr,"ERROR: unable to open the file %s\n",tagedit ->mp3_fname);

		return e_failure;
	}
	return e_success;

}
/*Get size of tag size*/
Status get_size_tag_edit(FILE *fptr)
{
	int idx;
	unsigned char ch ='\0';
	int size = 0;

	for (idx = 0; idx < sizeof(int); idx++)
	{
		/*read the data from file character by char*/
		fread(&ch ,sizeof(char), 1, fptr);

		/*find size*/
		size = size|ch;

		if((sizeof(int)-1)!= idx)
		{
			/*shift the size 32 bits*/
			size <<= (sizeof(char)*8);
		}
	}
	return size;
}
/*Get tags for edit*/
int get_tags_to_edit(char *argv[])
{
	int idx;

	/*array with all tag information*/
	char *array[6] = {"-album_name", "-singer_name", "-song_name", "-year", "-tracknum", "-composer_name"};

	for (idx = 0; idx < 6; idx++)
	{
		/*comparision of tag lengths*/
		if (strncmp(argv[2], array[idx], strlen(array[idx])) == 0)
		{
			break;
		}
	}

	return idx+1;

}
/*Get options for editing tag*/
Status get_options(int option, char *argv[], int argc, TagEdit *tagedit)
{

	if (argc == 4)

		/*assign tags to option */
		option = get_tags_to_edit(argv);

	else
	{
		/*choose the options*/
		printf("Choose the edit option:\n1.Album\n2.Singer\n3.Song\n4.Year\n5.Tracknum\n6.Composer\nEnter the option:  ");
		scanf("%d", &option);
	}

	/*switch to select option*/
	switch (option)
	{
		case 1:
			/*copy the album name for editing*/
			strcpy(tagedit->edit_tags, "TALB");
			break;

		case 2:
			/*copy the  singername for editing*/
			strcpy(tagedit->edit_tags, "TPE1");
			break;

		case 3:
			/*copy the song name for editing*/
			strcpy(tagedit->edit_tags, "TIT2");
			break;

		case 4:
			/*copy the year  for editing*/
			strcpy(tagedit->edit_tags, "TYER");
			break;

		case 5:
			/*copy the track number for editing*/
			strcpy(tagedit->edit_tags, "TRCK");
			break;

		case 6:
			/*copy the composer name for editing*/
			strcpy(tagedit->edit_tags, "TCOM");
			break;
	}

}
/*Edit tags*/
Status edit_tags(TagEdit *tagedit)
{
	int size, idx, length;
	char *ptr, buffer[50], temp[3], nul = '\0';

	/*get each tag size*/
	size = get_size_tag(tagedit->fptr_mp3);
	
	fread(temp, 1, 2, tagedit->fptr_mp3);

	/*Read new data from user and store into buffer*/
	printf("Enter the new data: ");
	scanf("\n%[^\n]", buffer);

	/*calculate buffer length*/
	length = strlen(buffer);

	if (length + 1 == size)
	{
		/*write character*/
		fwrite(&nul, 1, 1, tagedit->fptr_mp3);

		/*write charater according to buffer length*/
		fwrite(&buffer, 1, strlen(buffer), tagedit->fptr_mp3);
	}		
	if (length + 1 < size)
	{
		fwrite(&nul, 1, 1, tagedit->fptr_mp3);

		fwrite(&buffer, 1, strlen(buffer), tagedit->fptr_mp3);

		for (idx = 0; idx < (size - length - 1); idx++)
		{
			fwrite(&nul, 1, 1, tagedit->fptr_mp3);
		}
	}

}
/*copy edited tag names*/
Status copy_tag_names(TagEdit *tagedit, char *argv[], int argc)
{
	
	int byte_read, idx;
	char buffer[4];
	int option;
	char ch;


	if (argc == 4)
	{	
		/*function call to read and validate arguments*/
		read_and_validate_edit_args(argv, tagedit);
	}
	open_edit_files(tagedit);

		/*function call to get option*/
		get_options(option, argv,  argc, tagedit);

	while(feof(tagedit -> fptr_mp3)==0)
	{
		/*read the character from file*/
		fread(&ch ,sizeof(char) ,1 ,tagedit->fptr_mp3);

		if(ch == tagedit->edit_tags[0])
		{
			buffer[0] = ch;

			for(idx = 1; idx < 4; idx++)
			{
				/*read the next 3 bytes*/
				fread(&ch ,sizeof(char) ,1 ,tagedit->fptr_mp3);
				buffer[idx] = ch;

			}
			buffer[idx] = '\0';
			
			if (strcmp(buffer, tagedit->edit_tags) == 0)
			{
				/*function call to edit tags*/
				edit_tags(tagedit);
				break;
			}
		}
	}
			/*close the file*/
			fclose(tagedit->fptr_mp3);
}


			




	

