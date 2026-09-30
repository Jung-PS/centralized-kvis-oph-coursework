#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

void warning(char message[], char prompt[], int mode){
	if(mode == 1){
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
	}
	char pattern[10][1000]={
		"  _  ____     _____ ____     ___                   _                          ",
		" | |/ /\\ \\   / /_ _/ ___|   / _ \\ _ __   ___ _ __ | |__   ___  _   _ ___  ___ ",
		" | ' /  \\ \\ / / | |\\___ \\  | | | | '_ \\ / _ \\ '_ \\| '_ \\ / _ \\| | | / __|/ _ \\",
		" | . \\   \\ V /  | | ___) | | |_| | |_) |  __/ | | | | | | (_) | |_| \\__ \\  __/",
		" |_|\\_\\   \\_/  |___|____/   \\___/| .__/ \\___|_| |_|_| |_|\\___/ \\__,_|___/\\___|",
		"                                 |_|                                          "
	};
	strcpy(pattern[7],message);
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	printf("\033[H");
	for(int i=0;i<w.ws_row;++i){
		int len=i-w.ws_row/2+5;
		int lim=w.ws_col;
		for(int j=0;j<lim;++j){
			int idx=j-10;
			if(i==1 || i==w.ws_row-1) printf("_");
			else if(j==1 || j==w.ws_col-1) printf("|");
			else if(0<=len && len<=9 && 0<=idx && idx<strlen(pattern[len])){
				if(len<=6){
					if(idx<=6) printf("\033[32m");
					else if(idx<=25) printf("\033[35m");
					else printf("\033[31m");
				}
				else printf("\033[33m");
				printf("%c", pattern[len][idx]);
			}
			else if(i==w.ws_row-2 && j==w.ws_col-15){
				printf("\033[44m<   OK   >");
				j+=9;
			}
			else printf(" ");
			printf("\033[0m");
		}
		printf("\n");
	}
	printf("%s", prompt);
	getchar();
	printf("\033[2J\033[H");
}
