#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

int get_term_width(void) {
        struct winsize w;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 0)
                return w.ws_col;
        return 80;
}

unsigned get_len(char *s)
{
	unsigned i = 0;
	while (s[i++] != '\0')
		;
	return --i;
}

void print_line(char ch) {
        int no = get_term_width();
        while (no-- > 0)
                fprintf(stdout, "%c",ch);
        fprintf(stdout, "\n");
}

void middle_text(char * s) {
        int width = get_term_width();
        int len = get_len(s);
        int i = 0;
        while (i++ <width/2 - len/2)
                printf(" ");
        printf("%s",s);
        printf("\n");
}

