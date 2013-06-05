#include <stdio.h>

int main(int argc, char *argv[])
{
	int numbers[4] = {0};
	char name[7] = {'a'};

	//first, print them out raw
	printf("numbers: %d %d %d %d\n", numbers[0], numbers[1], numbers[2], numbers[3]);

	printf("name each: %c %c %c %c %c %c %c\n", name[0], name[1], name[2], name[3], name[4], name[5], name[6]);

	printf("name: %s\n", name);

	// setup the numbers
	numbers[0] = 1;
	numbers[1] = 2;
	numbers[2] = 3;
	numbers[3] = 4;

	// set up name
	name[0] = 'C';
	name[1] = 'i';
	name[2] = 'n';
	name[3] = 'q';
	name[4] = 'u';
	name[5] = 'e';
	name[6] = '\0';

	// then print them out initialized
	printf("numbers: %d %d %d %d\n", numbers[0], numbers[1], numbers[2], numbers[3]);

	printf("name each: %c %c %c %c %c %c %c\n", name[0], name[1], name[2], name[3], name[4], name[5], name[6]);

	// print the name like a string
	printf("name: %s\n", name);

	// another way to use name

	char *another = "Cinque";

	printf("another: %s\n", another);

	printf("another each: %c %c %c %c %c %c %c\n", another[0], another[1], another[2], another[3], another[4], another[5], another[6]);

	return 0;
}