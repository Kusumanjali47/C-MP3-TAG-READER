#ifndef TYPES_H
#define TYPES_H

typedef unsigned int uint;

typedef unsigned char uchar;


typedef enum
{
	e_success,
	e_failure
}Status;


typedef enum
{
	e_read = 1,
	e_edit = 2,
	e_unsupported
}OperationType;

typedef enum
{
	r_album_name,
	r_album_yr,
	r_artist,
	r_track_name,
	r_comp_name,
	r_song_name,
	r_album_art
}r_tags;

#endif
