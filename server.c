#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include "user.h"

user user_data[USER_DB_LIMIT];
user_config user_config_data;
int main() {
	// FOR CLEAR
	char cache[1024];
	new_user(user_data, &user_config_data, "ADMIN", 123456,cache);
	change_user_balance(user_data, user_config_data, 1, 123456, 10000000);
	change_user_role(user_data, user_config_data, 1, "01100000000000000000");
	//
	
	load_user(user_data, user_config_data);
	int server = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in address = {
		.sin_family = AF_INET,
		.sin_port = htons(8080),
		.sin_addr.s_addr = INADDR_ANY
	};
	bind(server, (struct sockaddr *)&address, sizeof(address));
	listen(server, 5);
	while (1) {
		int client = accept(server, NULL, NULL);
		char message[1024] = {0};
		read(client, message, sizeof(message) - 1);
		printf("Receive from client: %s\n", message);
		char inputarr[20][1024];
		char cache[1024]={0};
		for(int i=0, j=0;i<strlen(message);++i){
			if(message[i]=='|'){
				strcpy(inputarr[j],cache);
				cache[0]='\0';
				++j;
			}
			else{
				cache[strlen(cache)+1]='\0';
				cache[strlen(cache)]=message[i];
			}
		}
		char responsearr[20][1024], response[1024];	
		if (strcmp(inputarr[0], "REGISTER") == 0)
		{
			// TODO: SSV, inputarr must have length of 6 and is all digit
			new_user(user_data, &user_config_data, inputarr[1], atoi(inputarr[2]), responsearr[0]);	
			strcpy(responsearr[1], "END");
		}	
		if (strcmp(inputarr[0], "LOGIN") == 0) {
			if(authenticate(user_data, atoi(inputarr[1]),atoi(inputarr[2]))=='1'){
				strcpy(responsearr[0], "SUCCESS");
				view_user_role(user_data, atoi(inputarr[1]), responsearr[1]);
				strcpy(responsearr[2], user_data[atoi(inputarr[1])-1].name);
			}
			else strcpy(responsearr[0], "FAILURE");
			strcpy(responsearr[3], "END");
		}
		if (strcmp(inputarr[0], "VIEW") == 0){
			get_user_balance(user_data, atoi(inputarr[1]), responsearr[0]);
			strcpy(responsearr[1], "END");
		}
		if (strcmp(inputarr[0], "TRANSFER") == 0){
			// Required privilege 0
			money_transfer(user_data, user_config_data, atoi(inputarr[1]), atoi(inputarr[2]), atoi(inputarr[3]), atoi(inputarr[4]), responsearr[0]);
			strcpy(responsearr[1],user_data[atoi(inputarr[2])-1].name);
			strcpy(responsearr[2],user_data[atoi(inputarr[1])-1].name);
			strcpy(responsearr[5], "END");
		}
		if (strcmp(inputarr[0], "VIEWROLE") == 0){
			strcpy(responsearr[0], "FAILURE");
			if(atoi(inputarr[1]) == atoi(inputarr[2]) || user_data[atoi(inputarr[1])-1].role[2]=='1') {
				view_user_role(user_data, atoi(inputarr[2]), responsearr[0]);
			}
			strcpy(responsearr[1], "END");	
		}
		if (strcmp(inputarr[0], "VIEWSTICKER") == 0){
			strcpy(responsearr[0], "FAILURE");
			if(atoi(inputarr[1]) == atoi(inputarr[2]) || user_data[atoi(inputarr[1])-1].role[1]=='1') {
				view_user_sticker(user_data, atoi(inputarr[2]), responsearr[0]);
			}
			strcpy(responsearr[1], "END");	
		}
		if (strcmp(inputarr[0], "SETROLE") == 0) {
			// Required privilege 2
			strcpy(responsearr[0], "FAILURE");
			if(user_data[atoi(inputarr[1])-1].role[2]=='1') {
				change_user_role(user_data, user_config_data, atoi(inputarr[2]), inputarr[3]);
				view_user_role(user_data, atoi(inputarr[2]), responsearr[0]);
			}
			strcpy(responsearr[1], "END");
		}
		if (strcmp(inputarr[0], "SETSTICKER") == 0) {
			// Required privilege 1
			strcpy(responsearr[0], "FAILURE");
			if(user_data[atoi(inputarr[1])-1].role[2]=='1') {
				set_user_sticker(user_data, user_config_data, atoi(inputarr[2]), inputarr[3]); 
				view_user_sticker(user_data, atoi(inputarr[2]), responsearr[0]);
			}
			strcpy(responsearr[1], "END");
		}
		response[0]='\0';
		for(int i=0;;++i){
			strcat(response,responsearr[i]);
			strcat(response,"|");
			if(strcmp(responsearr[i], "END") == 0) break;
		}
		printf("Send to client %s\n", response);
		write(client, response, strlen(response));
		close(client);
	}
	close(server);
	return 0;
}
