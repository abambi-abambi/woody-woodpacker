#include <stdio.h>

int hello();

int main(void)
{
	int x = hello();
	printf("Return code: %x\n", x);

	return 0;
}

/*
** Compile c-file with this library:
** gcc sample_main_4dynlib.c  -ldynamic -L. -o res
**
** Launch:
** ./res
*/