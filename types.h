#ifndef TYPES_H
#define TYPES_H

/* User defined types */
typedef unsigned int uint;

/* Status will be used in fn. return type */
typedef enum
{
    e_success,
    e_failure
} Status;

typedef enum
{
    read = 1,
    edit = 2,
    e_unsupported
} OperationType;

typedef enum
{
	r_album_name,
	r_album_yr,
	r_artist,
	r_track_num,
	r_comp_name,
	r_song_name,
	r_album_art
}tags;

#endif
