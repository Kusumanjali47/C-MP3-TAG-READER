#ifndef TAG_EDIT_H
#define TAG_EDIT_H

#include "types.h"
#include "test.h"

/*Structure definition*/
typedef struct _TagEdit
{
	/*file info*/
	char src_mp3_fname[32];
	char edit_tags[5];
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

}TagEdit;

/*read and validate function*/
Status read_and_validate_edit_args(char *argv[], TagEdit *tagedit);

/*opening the files*/
Status open_edit_files(TagEdit *tagedit);

/*calculating the size*/
Status get_size_tag(FILE *fptr);

/*Get tags to edit*/
int get_tags_to_edit(char *argv[]);

/*Get option to edit*/
Status get_options(int option, char *argv[], int argc, TagEdit *tagedit);

/*Edit the tags*/
Status edit_tags(TagEdit *tagedit);

/*copy the tags into file*/
Status copy_tag_names(TagEdit *tagedit, char *argv[], int argc);

#endif
