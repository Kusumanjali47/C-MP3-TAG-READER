#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "tag_read.h"
#include "tag_edit.h"
#include "test.h"
#include "types.h"

/*MAIN PROGRAM*/
int main(int argc, char *argv[])
{
	/*Declaring the variables*/
	TagRead tagread;

	TagEdit tagedit;

	int option;

	char ch;

	do
	{

		/*If the argc is 1 then read the option*/
		if (argc == 1)
		{
			printf("Select the option :\n\t1.Read\n\t2.Edit\n");

			printf("Enter the option :");
			scanf("%d", &option);

		}
		/*If it is false, invoke the function call check_operation_type*/
		else
		{
			option = check_operation_type(&argv[1]);
		}

		/*Switch case for reading and editing*/
		switch (option)
		{
			case e_read:

				/*If the argc is 1 read the file name*/
				if (argc == 1)
				{
					printf("Enter the file name :");

					scanf("%s", tagread.src_mp3_fname);
				}

				/*invoke the call for reading the information of tags*/
				get_tags(&tagread, argv, argc);

				break;

			case e_edit:
				
				/*If the argc is 1 read the file name*/
				if (argc == 1)
				{
					printf("Enter the file name :");

					scanf("%s", tagedit.src_mp3_fname);
				}
				
				/*invoke the function call to edit the tags*/
				copy_tag_names(&tagedit, argv, argc);

				break;

			default:

				printf("Unsupported\n");

				break;
		}

		/*Read the continue option*/
		printf("Do you want to continue: (y/n)");

		scanf("\n%c", &ch);

	} while (ch == 'y');

	return 0;
}
