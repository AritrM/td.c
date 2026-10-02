#include <stdio.h>

int main(void)
{
	FILE *ptr = fopen("list.txt","r");
	fpos_t pos;
	char a[100];
	fgetpos(ptr,&pos);
	fgets(a,100,ptr);
	fgets(a,100,ptr);
	fsetpos(ptr,&pos);
	fgets(a,100,ptr);
	printf("%s\n",a);
	return 0;
}
