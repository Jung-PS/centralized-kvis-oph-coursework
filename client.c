#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include "tui.h"
int authenticated;
char auth_token[16];
char auth_roles[20];
void send_network(char packet[1024], char buf[1024]){
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr = {AF_INET, htons(8080), {inet_addr("127.0.0.1")}};
	connect(sock, (struct sockaddr *)&addr, sizeof(addr));
	send(sock, packet, strlen(packet), 0);
	read(sock, buf, 1024);
	close(sock);
}
int main() {
	warning("Welcome to Kamnoetvidya Science Academy", "Enter to continue",0);
	while(1){
		printf("\n");
		// TODO: Fix injection attack, length safety
		char inputarr[20][1024], outputarr[20][1024], input[1024], output[1024], cache[1024]={0};
		if(!authenticated) printf("Please Login (L) or Register (R): ");
		else{
			printf("You can perform the following actions\n");
			printf("(V): View your balance\n");
			printf("(S): View your sticker\n");
			printf("(T): POS: Request transaction\n");
			printf("(F): Refresh\n");
			if(auth_roles[1]=='1'){
				printf("(C): View stickers of user\n");
				printf("(U): Set stickers of user\n");
			}
			if(auth_roles[2]=='1'){
				printf("(A): View roles of user\n");
				printf("(B): Set roles of user\n");
			}
			printf("Please input your desired action: ");
		}
		char cmd; scanf(" %c",&cmd);

		if (cmd == 'R') strcpy(inputarr[0], "REGISTER");
		if (cmd == 'L') strcpy(inputarr[0], "LOGIN"); 
		if (cmd == 'V') strcpy(inputarr[0], "VIEW");
		if (cmd == 'T') strcpy(inputarr[0], "TRANSFER");
		if (cmd == 'S' || cmd == 'C') strcpy(inputarr[0], "VIEWSTICKER");
		if (cmd == 'A' || cmd == 'F') strcpy(inputarr[0], "VIEWROLE");
		if (cmd == 'B') strcpy(inputarr[0], "SETROLE");
		if (cmd == 'R' || cmd == 'L'){
			if (cmd == 'R') printf("Enter your name: ");
			else if (cmd == 'L') printf("Enter your user ID: ");
			scanf("%s", inputarr[1]);
			printf("Enter your 6-digits PIN: ");
			scanf("%s", inputarr[2]);
			strcpy(inputarr[3],"END");
		}
		if (cmd == 'V' || cmd == 'S' || cmd == 'F'){
			strcpy(inputarr[1],auth_token);
			strcpy(inputarr[2],auth_token);
			strcpy(inputarr[3],"END");
		}	
		
		if (cmd == 'T'){
			strcpy(inputarr[1], auth_token);
			printf("POS: Enter user ID: ");
			scanf("%s", inputarr[2]);
			printf("POS: Enter 6-digits PIN: ");
			scanf("%s", inputarr[3]);
			printf("POS: Enter amount: ");
			scanf("%s", inputarr[4]);
			strcpy(inputarr[5], "END");
		}
		// Elevated
		if (cmd == 'C'){
			strcpy(inputarr[1], auth_token);
			printf("Sticker: Enter User ID: ");
			scanf("%s",inputarr[2]);
			strcpy(inputarr[3], "END");	
		}
		if (cmd == 'A'){
			strcpy(inputarr[1], auth_token);
			printf("Role: Enter User ID: ");
			scanf("%s", inputarr[2]);
			strcpy(inputarr[3], "END");
		}
		if (cmd == 'U') strcpy(inputarr[0], "SETSTICKER");
		if (cmd == 'U') {
			strcpy(inputarr[1], auth_token);
			printf("Sticker: Enter User ID: ");
			scanf("%s", inputarr[2]);
			printf("Sticker: Enter new value: ");
			scanf("%s", inputarr[3]);
			strcpy(inputarr[4], "END");
		}
		if (cmd == 'B') strcpy(inputarr[0], "SETROLE");
		if (cmd == 'B') {
			strcpy(inputarr[1], auth_token);
			printf("Role: Enter User ID: ");
			scanf("%s", inputarr[2]);
			printf("Role: Enter new value: ");
			scanf("%s", inputarr[3]);
			strcpy(inputarr[4], "END");
		}
			
		input[0]='\0';	
		for(int i=0;;++i){
			strcat(input,inputarr[i]);
			strcat(input,"|");
			if(strcmp(inputarr[i],"END")==0) break;
		}
		//printf("DEBUG: send %s\n", input);
		send_network(input, output);
		//printf("DEBUG: receive %s\n", output);
		for(int i=0, j=0; ;++i){
			if(output[i]=='|'){
				if(strcmp(cache,"END")==0) break;
				strcpy(outputarr[j],cache);
				cache[0]='\0';
				++j;
			}
			else{
				cache[strlen(cache)+1]='\0';
				cache[strlen(cache)]=output[i];
			}
		}
		if (cmd== 'R'){
			printf("Registration Successful, please login to continue\n");
			printf("=== PLEASE NOTE DOWN THE FOLLOWING INFORMATION ===\n");
			printf("Your user ID is %s\n", outputarr[0]);
			printf("=== END OF OUTPUT ===\n");
		}
		else if (cmd == 'L'){
			printf("Authentication: %s\n", outputarr[0]);
			if(strcmp(outputarr[0],"SUCCESS")==0){
				authenticated = 1;
				printf("Authenticated as %s\n", outputarr[2]);
				strcpy(auth_roles,outputarr[1]);
				strcpy(auth_token, inputarr[1]);
			}		
		}
		else if (cmd == 'V'){
			char buffer[10000];
			sprintf(buffer, "Your balance is %s", outputarr[0]);
			warning(buffer,"Enter to continue",1);
		}
		else if (cmd == 'S' || cmd == 'C'){
			char buffer[10000];
			sprintf(buffer, "Stickers list is %s", outputarr[0]);
			warning(buffer, "Enter to continue", 1);
		}
		else if (cmd == 'A' || cmd == 'F'){
			char buffer[10000];
			sprintf(buffer, "Roles list is %s", outputarr[0]);
			if (cmd == 'F') strcpy(auth_roles,outputarr[0]);
			warning(buffer, "Enter to continue", 1);
		}
		else if (cmd == 'U'){
			char buffer[10000];
			sprintf(buffer, "Changed stickers list is %s", outputarr[0]);
			warning(buffer, "Enter to continue", 1);
	
		}
		else if (cmd == 'B'){
			char buffer[10000];
			sprintf(buffer, "Changed roles list is %s", outputarr[0]);
			warning(buffer, "Enter to continue", 1);
			
		}
		else if (cmd == 'T'){
			printf("Money transfer: %s\n\n", outputarr[0]);
			if(strcmp(outputarr[0],"SUCCESS")==0){
				printf("================== CASH RECEIPT ==================\n");
				printf("    Kamnoetvidya Science Academy\n");
				printf("    999 Moo 1, Pa Yup Nai, Wang Chan\n");
				printf("    Rayong 21210 TH\n");
				printf("\n");
				printf("    Proof of money transfer\n");
				printf("    FROM: %s\n", outputarr[1]);
				printf("    TO:   %s\n", outputarr[2]);
				printf("    NET:  %s THB\n", inputarr[4]);
				printf("\n\n033 013 888");
				printf("\nThank you");
				printf("\n================== CASH RECEIPT ==================");
			}
		}
		printf("\n");
	}
}
