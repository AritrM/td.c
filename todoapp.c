#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "filepath.h"

#define MAX 100
int flag = 0;

typedef struct {
	unsigned int index;
	int status;
	char *ptr;
	size_t ulen;	/*For used spaces*/
	size_t alen;	/*For Assigned spaces*/
} task;
typedef struct {
	task *data;
	int len;
	int ulen;
} list;
typedef struct {
	char *ptr;
	size_t ulen;
	size_t alen;
} string;

char* input_line(void){
	string *str = (string*) malloc(sizeof(string));
	char *s;
	int c;
	str->ulen = 0;
	str->alen = MAX;
	str->ptr = (char *) malloc (str->alen);
	str->ptr[str->ulen] = '\0';
	while ((c = getchar()) != EOF && c != '\n'){
		str->ptr[str->ulen] = c;
		++str->ulen;
		if (str->ulen == str->alen) {
			str->alen += 20;
			str->ptr = (char*) realloc(str->ptr,str->alen);
		}
	}
	str->ptr[str->ulen] = '\0';
	s = str->ptr;
	free(str);
	return s;
}

void input_cleaning(void) {
	int c;
	while ((c=getchar())!=EOF && c != '\n')
		;
}

int total_entries(void) {
	FILE *file = fopen(LIST_PATH,"r");
	int c;
	size_t line = 0;
	while ((c = fgetc(file))!=EOF)
		if (c=='\n')
			++line;
	// printf("Len: %lu\n",line);
	fclose(file);
	return line;
}

list *loading(list *l) {
	FILE *file = fopen(LIST_PATH,"r");
	int i,s,c;
	l->ulen = total_entries();
	l->len = l->ulen + 2;
	if (file == NULL) {
		free(l);
		return NULL;
	}
	l->data = (task *)malloc(sizeof(task)*l->len);
	l->data[l->ulen].ptr= l->data[l->ulen+1].ptr= NULL;
	if (fscanf(file,"%d.[%d]",&i,&s) == EOF) {
		printf("cannot find appropriate format\n");
		fclose(file);
		return l;
	} else
		rewind(file);
	while(fscanf(file,"%d.[%d]",&i,&s) != EOF) {
		--i;
		l->data[i].index = i;
		l->data[i].status = s;
		l->data[i].ulen = 0;
		l->data[i].alen = MAX;
		l->data[i].ptr= (char*)malloc(l->data[i].alen);
		l->data[i].ptr[l->data[i].ulen]='\0';
		while((c=fgetc(file))!=EOF&&c != '\n') {
			l->data[i].ptr[l->data[i].ulen]=c;
			++l->data[i].ulen;
			if (l->data[i].ulen == l->data[i].alen){
				l->data[i].alen += 20;
				l->data[i].ptr=(char*) realloc(l->data[i].ptr,l->data[i].alen);
			}
		}
		l->data[i].ptr[l->data[i].ulen]='\0';
	}
	fclose(file);
	return l;
}

void show(list* l) {
	int i = 0;
	if (l == NULL)
		printf("Tasks cannot be loaded.");
	else while (i < l->len) {
		printf("%d.",i+1);
		if (l->data[i].status == 1)
			printf("☑ "); //completion
		else
			printf("☐ "); //incompletion
		printf("%s",l->data[i].ptr);
			printf("\n");
		++i;
	}
}

int tempadd(list *l,char *s) {
	l->data[l->ulen].index = l->ulen;
	l->data[l->ulen].status = 0;
	l->data[l->ulen].ptr = s;
	l->ulen++;
	if (l->ulen == l->len) {
		l->len += 2;
		l->data = (task *) realloc(l->data,sizeof(task)*l->len);
		l->data[l->ulen].ptr= l->data[l->ulen+1].ptr= NULL;
	}
	show(l);
	return 0;
}

void copy(char *f1, char *f2) {
	FILE *file_r = fopen(f1,"r");
	FILE *file_w = fopen(f2,"w");
	char s[MAX];
	while(fgets(s,MAX,file_r) != NULL)
		fputs(s,file_w);
	fclose(file_r);
	fclose(file_w);
}

int tempedit(list *l,unsigned int line, char *s) {
	if(line<=l->ulen) {
		l->data[--line].ptr = s;
		show(l);
	}
	return 0;
}

void mark_toggle(list *l, int i) {
	if (l->data[i-1].ptr!= NULL){
		if (l->data[i-1].status == 1)
			l->data[i-1].status = 0;
		else
			l->data[i-1].status = 1;
	}
	else 
		printf("No task to mark\n");
	show(l);
}

int save(list *l) {
	FILE *file = fopen ("tmp.txt","w");
	int i = -1, correction = 0;
	printf("%d\n",l->ulen);
	while(++i<l->ulen) {
		if (l->data[i].ptr == NULL)
			++correction;
		else
			fprintf(file,"%d.[%d]%s\n",l->data[i].index+1-correction,l->data[i].status,l->data[i].ptr);
	}
	fclose(file);
	copy("tmp.txt",LIST_PATH);
	remove(TMP_PATH);
	return 0;
}

int tempdel(list *l,unsigned int line) {
	if(line<=l->ulen) {
		free(l->data[line-1].ptr);
		l->data[line-1].ptr = NULL;
	}
	return 0;
}

void free_space(list *l) {
	if (l != NULL) {
		if (l->ulen > 0) {
			while (--l->ulen > 0)
				free(l->data[l->ulen].ptr);
			free(l->data[l->ulen].ptr);
		}
		free(l->data);
		free(l);
	}
}

void clear_list(list *l) {
	if (l->ulen>0) {
		while (--l->ulen>0) {
			free(l->data[l->ulen].ptr);
			l->data[l->ulen].ptr = NULL;
			l->data[l->ulen].status= 0;
		}
		free(l->data[l->ulen].ptr);
		l->data[l->ulen].ptr = NULL;
		l->data[l->ulen].status= 0;
	}
	save(l);
}

list *refresh(list *l) {
	list *l1 = (list *)malloc(sizeof(list));
	free_space(l);
	return loading(l1);
}

void* timeOut(void *arg) {
	while (1) {
		sleep(10);
		if (flag == 0) {
			printf("Exited.\n");
			save((list*)arg);
			printf("Done here.\n");
			free_space((list*)arg);
			exit(0);
		}
	}
}

int main(void) {
	char c;
	char *str;
	int index;
	list *lptr = (list *)malloc(sizeof(list));
	pthread_t out;
	lptr = loading(lptr);
	pthread_create(&out,NULL,timeOut,lptr);
	system("clear");
	printf("To-Do List\n");
	while (1) {
		printf("\n[(m)ark or un(m)ark/e(x)it/(s)how/(c)lear/(a)dd/(e)dit/(W)rite/(r)emove]\n");
		printf("[To save, you have to (W)rite it.\nDeletion also requires (W)riting.]\n");
		printf("Enter choice: \n");
		flag = 0;
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
				show(lptr);
				flag = 1;
				break;
			case 'a':
				printf("Adding a task\n");
				str = input_line();
				tempadd(lptr,str);
				flag = 1;
				break;
			case 'e':
				printf("Editing a task\n");
				printf("Line to Edit: ");
				scanf("%d",&index);
				input_cleaning();
				str = input_line();
				tempedit(lptr,index,str);
				flag = 1;
				break;
			case 'm':
				printf("Marking/Unmarking a task as done\n");
				printf("Line to mark/unmark: ");
				scanf("%d",&index);
				input_cleaning();
				mark_toggle(lptr,index);
				flag = 1;
				break;
			case 'r':
				printf("Removing a task\n");
				printf("Line to Remove: ");
				scanf("%d",&index);
				input_cleaning();
				tempdel(lptr,index);
				flag = 1;
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

