#include <stdio.h>

struct mytype
{
	void *myptr;
	int myval;
};

int a;
int b;

struct mytype mystructs[2] = {{&a, 10}, {&b, 20}};

void *myfunc(int input)
{
	if (input < 2 && input >= 0)
	{
		return mystructs[input].myptr;
	}
	else
	{
		return 0;
	}
}

int myfunc2(int input)
{
	if (input == 3)
		return input + 3;
	else
		return 0;
}

int main()
{
	int input;
	scanf("%d", &input);
	printf("%d\n", input);
	printf("%p\n",myfunc(input));
	printf("%d\n",myfunc2(input));
	return 0;
}
