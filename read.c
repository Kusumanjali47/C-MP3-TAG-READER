#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "tag_read.h"
#include "test.h"

/*Operation check type*/
OperationType check_operation_type(char **argv)
{
	/*To read the file*/
	if (strncmp(*argv, "-r", 2) == 0)
	{
		return e_read;
	}
	/*To edit the file*/
	if(strncmp(*argv, "-e", 2) == 0)
	{
		return e_edit;
	}
	else
		return e_unsupported;

}

/*Read and validate arguements*/
Status read_and_validate_args(char *argv[], TagRead *tagread)
{
	strcpy(tagread->src_mp3_fname, argv[2]);

	printf("MP3 file name : %s\n", tagread->src_mp3_fname);

	return e_success;
}

/*opening the files*/
Status open_files(TagRead *tagread)
{
	tagread->fptr_src_mp3 = fopen(tagread->src_mp3_fname, "r");

	/*sanity check*/
	if (tagread->fptr_src_mp3 == NULL)
	{
		perror("fopen");

		fprintf(stderr, "ERROR: Unable to open file %s\n", tagread->src_mp3_fname);

		return e_failure;
	}
	return e_success;
}

/*Computing the size*/
Status get_size(FILE *fptr)
{
	int i_index;

	int size = 0;

	unsigned char ch = '\0';

	/*coverting character bytes to integer*/
	for (i_index = 0; i_index < sizeof (int); i_index++)
	{
		fread(&ch, sizeof (char), 1, fptr);

		size = size | ch;

		if ((sizeof (int) - 1) != i_index)
		{
			size <<= (sizeof (char) * 8);
		}
	}
	return size;
}

/*Readin the info of the tags*/
Status readinfo(int *size, char **ptr, FILE *fptr)
{
	int tag_size;

	char flag[3];

	int i_index;

	/*invoking the function call get size*/
	*size = get_size(fptr);

	/*Allocating the memory*/
	*ptr = (char*)malloc(*size * sizeof (char));

	fread(flag, sizeof (char), 2, fptr);

	for (i_index = 0; i_index < (*size); i_index++)
	{
		fread(*ptr + i_index, sizeof (char), 1, fptr);
	}

	return e_success;
}

/*printing the tags*/
Status print_tags(int size, char *ptr)
{
	int i_index;

	/*for loop to print the tags neglecting the non printable characters*/
	for (i_index = 0; i_index < size; i_index++)
	{
		if (ptr[i_index] > 31 && ptr[i_index] != 92 && ptr[i_index] != 127)
		{
			printf("%c", ptr[i_index]);
		}
	}

	printf("\n");

	return e_success;
}

/*copying the contents of the file to the album art*/
Status album_art(TagRead *tagread)
{
	char s1 = 0xff, s2 = 0xd8;

	char e1 = 0xff, e2 = 0xd9;

	char ch;

	int flag = 0;

	FILE *fp;

	/*opening the album art file*/
	fp = fopen("Picture.jpg", "w");

	/*feof returns 0 if there are bytes to read*/
	while (feof(tagread->fptr_src_mp3) == 0)
	{

		fread(&ch, sizeof (char), 1, tagread->fptr_src_mp3);

		/*checking for the ff*/
		while (ch == s1)
		{
			/*checking for the d8*/
			if (ch == s2)
			{
				fwrite(&s1, 1, 1, fp);

				fwrite(&s2, 1, 1, fp);

				/*copying the contents upto ff d9*/
				while (1)
				{

					fread(&ch, sizeof (char), 1, tagread->fptr_src_mp3);

					fwrite(&ch, 1, 1, fp);
					
					/*checking for the char ff d9*/
					if (flag && ch == e2)
					{
						break;	
					}
					
					/*If the char is ff then set the flag*/
					if (ch == e1)
					{
						flag = 1;
					}
					else
					{
						flag = 0;
					}
				}
			}
		}
	}
	return e_success;
}

/*To read the imp tags*/
Status get_tags_for_read(char *string, TagRead *tagread)
{
	int i_index;

	char *tag_buffer[7] = {ALBUM_NAME, SINGER_NAME, ALBUM_YEAR, SONG_NAME, TRACK_NUMBER, COMPOSER_NAME, ALBUM_ART};

	for (i_index = 0; i_index < 6; i_index++)
	{
		if (strncmp(string, tag_buffer[i_index], 4) == 0)
		{
			switch (i_index)
			{
				case 0:
					
					/*reading the album name info*/
					readinfo(&tagread->album_name_size, &tagread->album_name, tagread->fptr_src_mp3);
					
					printf("%-20s", "ALBUM NAME");

					print_tags(tagread->album_name_size, tagread->album_name);

					break;

				case 1:
					
					/*reading the singer name info*/
					readinfo(&tagread->singer_name_size, &tagread->singer_name, tagread->fptr_src_mp3);
					
					printf("%-20s", "SINGER NAME");

					print_tags(tagread->singer_name_size, tagread->singer_name);

					break;

				case 2:
					
					/*reading the album year info*/
					readinfo(&tagread->album_year_size, &tagread->album_year, tagread->fptr_src_mp3);
					
					printf("%-20s", "ALBUM YEAR");

					print_tags(tagread->album_year_size, tagread->album_year);

					break;

				case 3:
					
					/*reading the song name info*/
					readinfo(&tagread->song_name_size, &tagread->song_name, tagread->fptr_src_mp3);

					printf("%-20s", "SONG NAME");

					print_tags(tagread->song_name_size, tagread->song_name);

					break;

				case 4:
					
					/*reading the track number info*/
					readinfo(&tagread->track_number_size, &tagread->track_number, tagread->fptr_src_mp3);
					
					printf("%-20s", "TRACK NUMBER");

					print_tags(tagread->track_number_size, tagread->track_number);

					break;

				case 5:
					
					/*reading the composer name*/
					readinfo(&tagread->composer_name_size, &tagread->composer_name, tagread->fptr_src_mp3);
					
					printf("%-20s", "COMPOSER NAME");

					print_tags(tagread->composer_name_size, tagread->composer_name);

					break;

				case 6:
					
					/*invoking the function call*/
					album_art(tagread);
					
					break;
			}
		}
	}
	return e_success;
}


/*Real function to get the tags*/
Status get_tags(TagRead *tagread, char *argv[], int argc)
{
	char ch;

	char buffer[4];

	int i_index;

	/*check whether the argc is 3 or not*/
	if (argc == 3)
	{
		/*invoking the function call to read the args*/
		read_and_validate_args(argv, tagread);
	}

	/*opeining the files*/
	open_files(tagread);

	/*feof returns zero if there are bytes to read*/
	while (feof(tagread->fptr_src_mp3) == 0)
	{
		fread(&ch, sizeof (char), 1, tagread->fptr_src_mp3);

		/*checking for the char T and A to get the tags*/
		if (ch == 'T' || ch == 'A')
		{
			/*storing the char*/
			buffer[0] = ch;

			for (i_index = 1; i_index < 4; i_index++)
			{
				fread(&ch, sizeof (char), 1, tagread->fptr_src_mp3);

				buffer[i_index] = ch;
			}

			buffer[i_index] = '\0';

			/*invoking the function call get_tags read*/
			get_tags_for_read(buffer, tagread);
		}
	}

	/*closing the file*/
	fclose(tagread->fptr_src_mp3);

	return e_success;
}

