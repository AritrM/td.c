#include <stdio.h>
#include <stdlib.h>
#define MAX 100

void input_line(char  s[]){
	int c,i=0;
	printf("Enter:\n");
	while (i<MAX-1 && (c=getchar()) != '\n')
		s[i++] = c;
	s[i++] = '\0';
}

void input_cleaning(void)
{
	int c;
	while ((c=getchar())!=EOF && c != '\n')
		;
}

char *take_input(void)
{
	char *str = malloc(MAX);
	int i = 0;
	int c;
	char *ini = str;
	while (i<MAX-1 && (c = getchar()) != '\n')
		str[i++] = c;
	str[i] = '\0';
	return str;
}

int show(void)
{
	FILE *file = fopen("/home/calllol/newfolder/files/list.txt","r");
	if (file == NULL)
		return 0;
	int index = 0;
	char s[MAX];
	if(fgets(s,MAX,file)==NULL)
		printf("No line.");
	else
		printf("%d. %s",++index,s);
	while ((fgets(s,MAX,file)) != NULL)
		printf("%d. %s",++index,s);
	fclose(file);
	return 0;
}

int add(char *s)
{
	FILE *file = fopen("/home/calllol/newfolder/files/list.txt","a");
	fputs(s,file);
	fputs("\n",file);
	fclose(file);
	return 0;
}

void copy(char *f1, char *f2)
{
	FILE *file_r = fopen(f1,"r");
	FILE *file_w = fopen(f2,"w");
	char s[MAX];
	while(fgets(s,MAX,file_r) != NULL)
		fputs(s,file_w);
	fclose(file_r);
	fclose(file_w);
}

int edit(unsigned int line, char *s)
{
	FILE *file_r = fopen("/home/calllol/newfolder/files/list.txt","r");
	FILE *file_w = fopen("/home/calllol/newfolder/files/tmp.txt","w");
	char task[MAX];
	while(fgets(task,MAX,file_r)!=NULL)
	{
		if (--line == 0)
		{
			printf("found\n");
			fputs(s,file_w);
			fputs("\n",file_w);
		}
		else
			fputs(task,file_w);
	}
	fclose(file_r);
	fclose(file_w);
	copy("/home/calllol/newfolder/files/tmp.txt","/home/calllol/newfolder/files/list.txt");
	remove("/home/calllol/newfolder/files/tmp.txt");
	return 0;
}

int del(unsigned int line)
{
	FILE *file_r = fopen("/home/calllol/newfolder/files/list.txt","r");
	FILE *file_w = fopen("/home/calllol/newfolder/files/tmp.txt","w");
	char task[MAX];

	while(fgets(task,MAX,file_r)!=NULL)
	{
		if (--line == 0)
			;
		else
			fputs(task,file_w);
	}
	fclose(file_r);
	fclose(file_w);
	copy("/home/calllol/newfolder/files/tmp.txt","/home/calllol/newfolder/files/list.txt");
	remove("/home/calllol/newfolder/files/tmp.txt");
	return 0;
}
void clear_list(void)
{
	remove("/home/calllol/newfolder/files/list.txt");
	FILE *file_r = fopen("/home/calllol/newfolder/files/list.txt","w");
}
int something(void) {
	char c;
	char str[MAX];
	int index;
	while (1)
	{
		printf("\n");
		printf("[e(x)it/(s)how/(c)lear/(a)dd/(e)dit/(r)emove]\n");
		printf("Enter choice: \n");
		c = getchar();
		input_cleaning();
		system("clear");
		printf("To-Do List\n");
		switch(c)
		{
			case 'x':
				printf("Exited\n");
				exit(0);
			case 's':
				printf("All Tasks\n");
				show();
				break;
			case 'a':
				printf("Adding a task\n");
				input_line(str);
				add(str);
				break;
			case 'e':
				printf("Editing a task\n");
				printf("Line to edit: ");
				scanf("%d",&index);
				input_cleaning();
				input_line(str);
				edit(index,str);
				break;
			case 'r':
				printf("Removing a task\n");
				printf("Line to edit: ");
				scanf("%d",&index);
				input_cleaning();
				del(index);
				break;
			case 'c':
				clear_list();
				break;
			default:
				printf("Wrong choice.\n");
				break;
		}
	}
	;return 0;
}

int main(void)
{
	something();
	return 0;
}
