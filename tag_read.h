#ifndef TAG_READ_H
#define TAG_READ_H

#include "types.h"
#include "test.h"

/*Structure definition*/
typedef struct _TagRead
{
	/*File info*/
	char src_mp3_fname[32];
	FILE *fptr_src_mp3;

	/*album name info*/
	int album_name_size;
	char *album_name;

	/*singer name info*/
	int singer_name_size;
	char *singer_name;

	/*song name info*/
	int song_name_size;
	char *song_name;

	/*composer name info*/
	int composer_name_size;
	char *composer_name;

	/*album year info*/
	int album_year_size;
	char *album_year;

	/*track number info*/
	int track_number_size;
	char *track_number;

	/*album art info*/
	int album_art_size;
	char *album_art_name;

}TagRead;

/*Checking the operation type*/
OperationType check_operation_type(char *argv[]);

/*To read and validate the args*/
Status read_and_validate_args(char *argv[], TagRead *tagread);

/*opening the files*/
Status open_files(TagRead *tagread);

/*Calculting the size*/
Status get_size(FILE *fptr);

/*Reading the info of tags*/
Status readinfo(int *size, char **ptr, FILE *fptr);

/*Reading the tags*/
Status get_tags_for_read(char *string, TagRead *tagread);

/*To read the tags*/
Status get_tags(TagRead *tagread, char *argv[], int argc);

/*album art*/
Status album_art(TagRead *tagread);

/*printing the tags*/
Status print_tags(int size, char *ptr);

#endif
