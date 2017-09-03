#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "read.h"
#include "tags.h"
#include "types.h"

/*check operation type */
OperationType check_operation(char ** argv)
{
	if (strncmp(*argv,"-r",2) == 0)
	{
		return read;
	}
	if (strncmp(*argv,"-e",2 )== 0)
	{
		return edit;
	}
	else
		return e_unsupported;

}
/*Read and validate the argument parameters*/
Status read_and_validate_read_args(char *argv[],TagRead *tagread)
{
	strcpy(tagread -> mp3_fname,  argv[2]);

	printf("%s", tagread->mp3_fname);

	return e_success;
}
/*open files for read data*/
Status open_files(TagRead *tagread)
{
	tagread -> fptr_mp3 =fopen(tagread ->mp3_fname,"r" );

	/*Error checking*/
	if(tagread -> fptr_mp3 == NULL)
	{
		perror("fopen");
		fprintf(stderr,"ERROR: unable to open the file %s\n",tagread ->mp3_fname);

		return e_failure;
	}
	return e_success;

}
/*Get size of tag  */
Status get_size_tag(FILE *fptr)
{
	int idx;
	unsigned char ch ='\0';
	int size =0;

	for(idx = 0; idx < sizeof(int); idx++)
	{
		/*Read the character from byte*/
		fread(&ch ,sizeof(char), 1, fptr);

		size = size|ch;

		if((sizeof(int)-1)!= idx)
		{
			/*shift the size upto 32 bit*/
			size <<= (sizeof(char)*8);
		}
	}
	return size;
}
/*Get image for album art*/
Status album_art(TagRead *tagread)
{
	char s1 = 0xff , s2 = 0xd8 ;
	char e1 = 0xff , e2 = 0xd9 ;
	char ch;
	int flag = 0;
	FILE *fp;

	/*open the jpg file*/
	fp = fopen("pictures.jpg","w");

	/*Error checking*/
	if(fp == NULL)
	{
		printf("Error to open the file\n");
	}
	while(feof(tagread->fptr_mp3)== 0)
	{
		/*read data from file character by character*/
		fread(&ch , sizeof(char) , 1 ,tagread->fptr_mp3);

		/*check for ff*/
		while(ch == s1)
		{
			/*check for d8*/
			if(ch == s2)
			{
				/*data start from ffd8 then write */
				fwrite(&s1 , 1 ,1 ,fp);
				fwrite(&s2 , 1 , 1 , fp);

				while(1)
				{
					/*read data from file char by char*/
					fread(&ch, sizeof(char) , 1 ,tagread->fptr_mp3);
					fwrite(&ch,1 ,1, fp);

					/*check for d9*/
					if(flag&&ch == e2)
					{
						break;
					}
					/*check for ff*/
					if(ch == e1)
					
						/*if ffd9 is found set the flag*/
						flag = 1;
					else
						flag = 0;
				}
			}
		}
	}
}
/*Read the tag information*/
Status readInfo(int *size, char **ptr, FILE *fptr)
{
	int idx;
	int tag_size;
	char flag_buff[2];

	/*function call to get the size of flag*/
	*size = get_size_tag(fptr);

	/*dynamic memory allocation*/
	*ptr = (char*)malloc(*size * sizeof(char));

	/*read the file content just store the 2 flags in flag buffer*/
	fread(flag_buff , sizeof(char) , 2 ,fptr);


	/*run the loop up to size of tag*/
	for(idx= 0; idx < (*size) ; idx++)
	{
		/*get read information*/
		fread(*ptr + idx , sizeof(char),1 ,fptr);
	}
}
/*Print the tags*/
Status print_tags(int size, char *ptr)
{
	int idx;

	for (idx = 0; idx < size; idx++)
	{
		/*check condition for nonprintable characters*/	
		if (ptr[idx] > 31 && ptr[idx] != 92 && ptr[idx] != 127)
		{
			/*print the tags*/
			printf("%c", ptr[idx]);
		}

	}
	printf("\n");
	
	return e_success;
}
/*Get tags for reading*/
Status gets_tags_for_read(char*buffer, TagRead *tagread)
{
	int idx;
	/*array of pointer to stor the macros*/
	char *ptr_buff[7] ={ALBUM_NAME, ARTIST_NAME, TRACK_NAME,ALBUM_YEAR,TRACK_NO,COMPOSER_NAME,ALBUM_ART};

	for(idx=0; idx < 6; idx++)
	{

		/*compare data within buffer with macros*/
		if(strncmp(buffer, ptr_buff[idx], 4) == 0)
		{

			switch(idx)
			{
				case 0:
					/*function call to read and print album name*/
					readInfo( &tagread->album_name_size, &tagread->album_name ,tagread-> fptr_mp3);
					printf("ALBUM NAME   :");
					print_tags(tagread->album_name_size, tagread->album_name);
					break;

				case 1:
					/*function call to read and print album name*/
					readInfo( &tagread->singer_name_size, &tagread->singer_name ,tagread-> fptr_mp3);
					printf("SINGER NAME  :");
					print_tags(tagread->singer_name_size, tagread->singer_name);
					break;

				case 2:
					/*function call to read and print song name*/
					printf("\n---------------------------------------------\n");
					readInfo( &tagread->song_name_size, &tagread->song_name ,tagread-> fptr_mp3);
					printf("SONG NAME    :");
					print_tags(tagread->song_name_size, tagread->song_name);
					break;

				case 3:
					/*function call to read and print album year*/
					readInfo( &tagread->album_year_size, &tagread->album_year ,tagread-> fptr_mp3);
					printf("ALBUM YEAR   :");
					print_tags(tagread->album_year_size, tagread->album_year);
					break;

				case 4:
					/*function call to read and print track number*/
					readInfo( &tagread->track_number_size, &tagread->track_number ,tagread-> fptr_mp3);
					printf("TRACK NUMBER :");
					print_tags(tagread->track_number_size, tagread->track_number);
					break;

				case 5:
					/*function call to read and print composer name*/
					readInfo( &tagread->composer_name_size, &tagread->composer_name ,tagread-> fptr_mp3);
					printf("COMPOSER NAME :");
					print_tags(tagread->composer_name_size, tagread->composer_name);
					printf("\n---------------------------------------------\n");
					break;

				case 6:

					album_art(tagread);
					break;

			}

		}
	}
}

/*Read the tags and print*/
Status get_tags(TagRead *tagread, char *argv[], int argc)
{

	int byte_read, idx;
	char buffer[4];
	char ch;

	if (argc == 3)
		read_and_validate_read_args(argv, tagread);
	open_files(tagread);
	/*Error checking*/
	while(feof(tagread -> fptr_mp3)==0)
	{
		/*read character from file*/
		fread(&ch ,sizeof(char) ,1 ,tagread->fptr_mp3);

		/*check for character T and A*/
		if(ch == 'T' || ch == 'A')
		{
			buffer[0] = ch;

			/*loop to read remaing 3 bytes*/
			for(idx = 1; idx < 4; idx++)
			{
				/*read the remaining bytes*/
				fread(&ch ,sizeof(char) ,1 ,tagread->fptr_mp3);
				buffer[idx] = ch;

			}
			buffer[idx] = '\0';

			/*function call to get tags for read*/
			gets_tags_for_read(buffer, tagread);
		}
	}
   fclose(tagread->fptr_mp3);
}

