#include <stdio.h>
int main(void)
{
	int age=20;
	float marks=85.2;
	char grade='a';
	printf("Age : %zu bytes\n",sizeof(age));
	printf("Marks : %zu bytes\n",sizeof(marks));
	printf("Grade : %zu bytes\n",sizeof(grade));
	return 0;
}

