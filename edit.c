#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "tag_edit.h"
#include "test.h"

/*Function for read and validAate args*/
Status read_and_validate_edit_args(char *argv[], TagEdit *tagedit)
{
	strcpy(tagedit->src_mp3_fname, argv[2]);

	printf("MP3 file name		: %s\n", tagedit->src_mp3_fname);

	return e_success;
}

/*opening the mp3 file to edit*/
Status open_edit_files(TagEdit *tagedit)
{
	tagedit->fptr_src_mp3 = fopen(tagedit->src_mp3_fname, "r+");

	/*sanity check*/
	if (tagedit->fptr_src_mp3 == NULL)
	{
		perror("fopen");

		fprintf(stderr, "ERROR: Unable to open file %s\n", tagedit->src_mp3_fname);

		return e_failure;
	}
	return e_success;
}

/*calculating the size*/
Status get_size_tag(FILE *fptr)
{
	int i_index;

	int size = 0;

	unsigned char ch = '\0';

	/*coverting the char bytes to the integer*/
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

/*option for to edit the tags*/
int get_tags_to_edit(char *argv[])
{
	int i_index;

	char *array[6] = {"-album", "-singer", "-song", "-year", "-track", "-composer"};

	/*for loop to count the options*/
	for (i_index = 0; i_index < 6; i_index++)
	{
		/*comparing the string s and argv*/
		if (strncmp(argv[2], array[i_index], strlen(array[i_index])) == 0)
		{
			break;
		}
	}

	return i_index + 1;
}

/*Get optins to edit*/
Status get_options(int option, char *argv[], int argc, TagEdit *tagedit)
{
	/*checking the count is 4 or not*/
	if (argc == 4)
	{
		/*invoking the function calls to edit*/
		option = get_tags_to_edit(argv);
	}
	else
	{
		/*Read the option*/
		printf("Enter the option:\n\t1.Album_name\n\t2.Singer_name\n\t3.Song_name\n\t4.Year\n\t5.Track number\n\t6.Composer name\nEnter the option:");
		scanf("%d", &option);
	}

	/*switch case for editing the tags*/
	switch (option)
	{
		case 1:
			
			/*copy the album name tag*/
			strcpy(tagedit->edit_tags, "TALB");

			break;

		case 2:
			
			/*copy the string name tag*/
			strcpy(tagedit->edit_tags, "TPE1");
		
			break;
		
		case 3:
			
			/*copy the song name tag*/
			strcpy(tagedit->edit_tags, "TIT2");
			
			break;

		case 4:
			
			/*copy the year tag*/
			strcpy(tagedit->edit_tags, "TYER");
			
			break;

		case 5:
			
			/*copy the track number tag*/
			strcpy(tagedit->edit_tags, "TRCK");
			
			break;

		case 6:
			
			/*copy the composer tag*/
			strcpy(tagedit->edit_tags, "TCOM");
			
			break;
	}
}

/*Function for Editing the tags*/
Status edit_tags(TagEdit *tagedit)
{
	int size, i_index, length;

	char *ptr, buffer[50], temp[3], nul = '\0';

	/*invoking the function call to get the size of the tag*/
	size = get_size_tag(tagedit->fptr_src_mp3);

	fread(temp, 1, 2, tagedit->fptr_src_mp3);

	/*Reading the new data*/
	printf("Enter the new data: ");

	scanf("\n%[^\n]", buffer);

	/*computing the length*/
	length = strlen(buffer);

	/*check whether the length is equal to size or not*/
	if ((length + 1) == size)
	{
		fwrite(&nul, 1, 1, tagedit->fptr_src_mp3);

		fwrite(&buffer, 1, strlen(buffer), tagedit->fptr_src_mp3);
	}

	/*check whether the length is less than the size or not*/
	if ((length + 1) < size)
	{
		fwrite(&nul, 1, 1, tagedit->fptr_src_mp3);
		
		fwrite(&buffer, 1, strlen(buffer), tagedit->fptr_src_mp3);
		
		for (i_index = 0; i_index < (size - length - 1); i_index++)
		{
			fwrite(&nul, 1, 1, tagedit->fptr_src_mp3);
		}
	}
}

/*copy the tag names to the file*/
Status copy_tag_names(TagEdit *tagedit, char *argv[], int argc)
{
	int byte_read, i_index;

	char buffer[4];
	
	int option;

	char ch;


	/*check whether the argc is 3 or not*/
	if (argc == 3)
	{
		/*invoking the function call*/
		read_and_validate_edit_args(argv, tagedit);
	}

	/*opening the files*/
	open_edit_files(tagedit);
	
	/*Reading the options*/
	get_options(option, argv, argc, tagedit);
	
	/*feof return 0 if there are bytes to read*/
	while (feof(tagedit->fptr_src_mp3) == 0)
	{
		fread(&ch, sizeof(char), 1, tagedit->fptr_src_mp3);

		/*checking the ch is equal to edit tags*/
		if (ch == tagedit->edit_tags[0])
		{
			buffer[0] = ch;

			for (i_index = 1; i_index < 4; i_index++)
			{
				fread(&ch, sizeof (char), 1, tagedit->fptr_src_mp3);
				
				buffer[i_index] = ch;
			}

			buffer[i_index] = '\0';

			/*comparing the buffer and edit tags*/
			if (strcmp(buffer, tagedit->edit_tags) == 0)
			{
				/*invoking the edit tags function call*/
				edit_tags(tagedit);

				break;
			}
		}
	}

	/*close file*/
	fclose(tagedit->fptr_src_mp3);
}
