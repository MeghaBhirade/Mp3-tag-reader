#ifndef  EDIT_H
#define EDIT_H

#include "types.h"


typedef struct _EditInfo
{
	FILE *fptr_mp3;
	char mp3_fname[32];
	char edit_tags[5];
	
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

}TagEdit;

OperationType check_operation(char ** argv);

/*Read and validate argument parameters*/
Status read_and_validate_edit_args(char *argv[],TagEdit*tagedit);

/*open files for editing*/
Status open_edit_files(TagEdit *tagread);

/*Get size of tag size*/
Status get_size_tag_edit(FILE *fptr);


/*Get tags for edit*/
int get_tags_to_edit(char *argv[]);

/*Get options for editing tag*/
Status get_options(int option, char *argv[], int argc, TagEdit *tagedit);

/*Edit tags*/
Status edit_tags(TagEdit *tagedit);

/*copy edited tag names*/
Status copy_tag_names(TagEdit *tagedit, char *argv[], int argc);
#endif
