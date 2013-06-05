#include <stdio.h>
#include <ctype.h>
#include <string.h>

//forward declarations
void print_letters(char arg[], int length);

void print_arguments(int argc, char *argv[])
{
	int i = 1;
	int length = strlen(argv[i]);
	for(i = 1; i < argc; i++){
		print_letters(argv[i], length);
	}
}

void print_letters(char arg[], int length)
{
	int i = 0;

	for(i = 0; i < length; i++){
		char ch = arg[i];

		if(isalpha(ch) || isblank(ch)){
			printf("'%c' == %d ", ch, ch);
		}
	}

	printf("\n");
}

int main(int argc, char *argv[])
{
	print_arguments(argc, argv);
	return 0;
}