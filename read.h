#ifndef  READ_H
#define READ_H

#include "types.h"


typedef struct _ReadInfo
{
	/*mp3 file info*/
	FILE *fptr_mp3;
	char mp3_fname[32];
	
	int album_name_size;
	char *album_name;

	int singer_name_size;
	char *singer_name;

	int song_name_size;
	char *song_name;

	int album_year_size;
	char *album_year;


	int track_number_size;
	char *track_number;

	int album_art_size;
	char *album_art_name;

	int composer_name_size;
	char *composer_name;

}TagRead;

/*check operation type */
OperationType check_operation(char ** argv);

/*Read and validate the argument parameters*/
Status read_and_validate_read_args(char *argv[],TagRead *tagread);

/*open files for read data*/
Status open_files(TagRead *tagread);

/*Get size of tag  */
Status get_size_tag(FILE *fptr);

/*Print the tags*/
Status print_tags(int size, char *ptr);

/*Read the tag information*/
Status readInfo(int *size, char **ptr, FILE *fptr);

/*Get tags for reading*/
Status gets_tags_for_read(char*buffer, TagRead *tagread);

/*Read the tags and print*/
Status get_tags(TagRead *tagread, char *argv[], int argc);
#endif
