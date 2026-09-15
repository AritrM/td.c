#include <stdio.h>
#include <stdlib.h>

#define MAX 100

typedef struct {
	unsigned int index;
	int status;
	char *ptr;
} task;
typedef struct {
	task *data;
	int len;
	int ulen;
} list;

char* input_line(void){
	char *s = malloc(MAX);
	int c,i=0;
	printf("Enter: ");
	while (i<MAX-2 && (c=getchar()) != '\n')
		s[i++] = c;
	s[i++] = '\n';
	s[i] = '\0';
	return s;
}

void input_cleaning(void)
{
	int c;
	while ((c=getchar())!=EOF && c != '\n')
		;
}

int total_entries(void)
{
	FILE *file = fopen("list.txt","r");
	int index = 0;
	char s[MAX];
	if (file == NULL)
		return -1;
	while ((fgets(s,MAX,file)) != NULL)
		++index;
	// printf("l: %d\n",index);
	fclose(file);
	return index;
}

list *loading(list *lptr)
{
	FILE *file = fopen("list.txt","r");
	int i,s;
	lptr->ulen = total_entries();
	lptr->len = lptr->ulen + 2;
	if (file == NULL) {
		free(lptr);
		return NULL;
	}
	lptr->data = (task *)malloc(sizeof(task)*lptr->len);
	if (fscanf(file,"%d.[%d]",&i,&s) == EOF)
	{
		printf("cannot find appropriate format\n");
		fclose(file);
		return lptr;
	} else
		// freopen("list.txt","r",file);
		rewind(file);

	while(fscanf(file,"%d.[%d]",&i,&s) != EOF)
	{
		--i;
		lptr->data[i].index = i;
		lptr->data[i].status = s;
		lptr->data[i].ptr= malloc(MAX);
		fgets(lptr->data[i].ptr,MAX,file);
	}
	fclose(file);
	return lptr;
}

int show(void)
{
	FILE *file = fopen("list.txt","r");
	int index = 0;
	char s[MAX];
	if (file == NULL)
		return 0;
	printf("Total: %d\n",total_entries());
	if(fgets(s,MAX,file)==NULL)
		printf("No line.");
	else
		printf("%d. %s",++index,s);
	while ((fgets(s,MAX,file)) != NULL)
		printf("%d. %s",++index,s);
	fclose(file);
	return 0;
}

void showv2(list* lptr)
{
	int i = 0;
	if (lptr == NULL)
		printf("Tasks cannot be loaded.");
	else while (i < lptr->len)
	{
		printf("%d.",i+1);
		if (lptr->data[i].status == 1)
			printf("# "); //completion
		else
			printf("@ "); //incompletion
		printf("%s",lptr->data[i].ptr);
		if(lptr->data[i].ptr == NULL)
			printf("\n");
		++i;
	}
}

int add(char *s)
{
	FILE *file = fopen("list.txt","a");
	fputs(s,file);
	fputs("\n",file);
	fclose(file);
	return 0;
}

int tempadd(list *lptr,char *s)
{
	lptr->data[lptr->ulen].index = lptr->ulen;
	lptr->data[lptr->ulen].status = 0;
	lptr->data[lptr->ulen].ptr = s;
	lptr->ulen++;
	if (lptr->ulen == lptr->len)
	{
		lptr->len += 2;
		lptr->data = (task *) realloc(lptr->data,sizeof(task)*lptr->len);
	}
	showv2(lptr);
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
	FILE *file_r = fopen("list.txt","r");
	FILE *file_w = fopen("tmp.txt","w");
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
	copy("tmp.txt","list.txt");
	remove("tmp.txt");
	return 0;
}

int tempedit(list *lptr,unsigned int line, char *s)
{
	lptr->data[--line].ptr = s;
	showv2(lptr);
	return 0;
}

int save(list *lptr)
{
	FILE *file = fopen ("temp.txt","w");
	int i = -1, correction = 0;
	while(++i<lptr->ulen)
	{
		if (lptr->data[i].ptr == NULL)
			++correction;
		else
			fprintf(file,"%d.[%d]%s",lptr->data[i].index+1-correction,lptr->data[i].status,lptr->data[i].ptr);
	}
	fclose(file);
	copy("temp.txt","list.txt");
	remove("temp.txt");
	return 0;
}

int del(unsigned int line)
{
	FILE *file_r = fopen("list.txt","r");
	FILE *file_w = fopen("tmp.txt","w");
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
	copy("tmp.txt","list.txt");
	remove("tmp.txt");
	return 0;
}

int tempdel(list *lptr,unsigned int line)
{
	free(lptr->data[line-1].ptr);
	lptr->data[line-1].ptr = NULL;
	return 0;
}

void free_space(list *lptr)
{
	if (lptr != NULL)
	{
		if (lptr->ulen > 0) {
			while (--lptr->ulen > 0)
					free(lptr->data[lptr->ulen].ptr);
				free(lptr->data[lptr->ulen].ptr);
		}
		free(lptr->data);
		free(lptr);
	}
}

void clear_list(list *lptr)
{
	if (lptr->ulen>0)
	{
		while (--lptr->ulen>0)
		{
			free(lptr->data[lptr->ulen].ptr);
			lptr->data[lptr->ulen].ptr = NULL;
		}
		free(lptr->data[lptr->ulen].ptr);
		lptr->data[lptr->ulen].ptr = NULL;
	}
	save(lptr);
}

list *refresh(list *lptr)
{
	list *l = (list *)malloc(sizeof(list));
	free_space(lptr);
	return loading(l);
}

int main(void) {
	char c;
	char *str;
	int index;
	list *lptr = (list *)malloc(sizeof(list));
	system("clear");
	printf("To-Do List\n");
	lptr = loading(lptr);
	while (1)
	{
		printf("\n[e(x)it/(s)how/(c)lear/(a)dd/(e)dit/(W)rite/(r)emove]\n");
		printf("[To save, you have to (W)rite it.\nDeletion also requires (W)riting.]\n");
		printf("Enter choice: \n");
		c = getchar();
		input_cleaning();
		system("clear");
		printf("To-Do List\n");
		switch(c)
		{
			case 'x':
			case 'q':
				printf("Exited\n");
				free_space(lptr);
				exit(0);
			case 's':
				printf("All Tasks\n");
				showv2(lptr);
				break;
			case 'a':
				printf("Adding a task\n");
				str = input_line();
				tempadd(lptr,str);
				break;
			case 'e':
				printf("Editing a task\n");
				printf("Line to Edit: ");
				scanf("%d",&index);
				input_cleaning();
				str = input_line();
				tempedit(lptr,index,str);
				break;
			case 'r':
				printf("Removing a task\n");
				printf("Line to edit: ");
				scanf("%d",&index);
				input_cleaning();
				tempdel(lptr,index);
				break;
			case 'c':
				clear_list(lptr);
				break;
			case 'W':
				if(lptr == NULL)
					printf("Unable to write.");
				else {
					save(lptr);
					lptr = refresh(lptr);
				}
				break;
			default:
				printf("Wrong choice.\n");
				break;
		}
	}
	return 0;
}

