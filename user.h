#include<stdio.h>
// TODO: Dynamic Memory Allocation (Replace USER_DB_LIMIT)
// TODO: Hashing of PIN
#define USER_DB_LIMIT 10000
typedef struct storage {
	int ID;
	char name[100];
	int salt;
	int hashed_pin;
	int balance;
	char sticker[20];
	char role[20];
} user;

typedef struct storage_config {
	int db_size;
} user_config;
	
void load_user(user user_data[USER_DB_LIMIT], user_config user_config_data){
	FILE *file = fopen("user_db.bin", "rb");
	fread(user_data, sizeof(user), USER_DB_LIMIT, file);
	fclose(file);
	FILE *file2 = fopen("user_config_db.bin", "rb");
	fread(&user_config_data, sizeof(user_config), 1, file2);
	fclose(file2);
	for(int i=0;i<10;++i) printf("%d\n",user_data[i].ID);
}

void save_user(user user_data[USER_DB_LIMIT], user_config user_config_data){
	FILE *file = fopen("user_db.bin", "wb");
	fwrite(user_data, sizeof(user), USER_DB_LIMIT, file);
	fclose(file);
	FILE *file2 = fopen("user_config_db.bin","wb");
	fwrite(&user_config_data,sizeof(user_config),1, file2);
	fclose(file2);
}

void new_user(user user_data[USER_DB_LIMIT], user_config *user_config_data, char name[], int pin, char output[])
{
	printf("DEBUG: Registering new user with %s and %d\n", name, pin);
	user_data[user_config_data->db_size].ID=user_config_data->db_size+1;
	// TODO: Hashing
	// TODO: Safety
	strcpy(user_data[user_config_data->db_size].name,name);
	user_data[user_config_data->db_size].hashed_pin = pin;
	user_data[user_config_data->db_size].balance = 0;
	for(int i=0;i<20;++i){
		user_data[user_config_data->db_size].role[i]='0';
		user_data[user_config_data->db_size].sticker[i]='0';
	}
	user_config_data->db_size++;
	save_user(user_data,*user_config_data);
	sprintf(output, "%d", user_config_data->db_size);
}

char authenticate(user user_data[USER_DB_LIMIT], int ID, int pin){
	// TODO: It should return sessionID tho
	printf("DEBUG: authenticate: %d\n", user_data[ID-1].hashed_pin);
	if(user_data[ID-1].hashed_pin == pin) return '1';
	else return '0';
}
void get_user_balance(user user_data[USER_DB_LIMIT], int ID, char output[]){
	// TODO: Change input to sessionID then query for matched ID
	printf("DEBUG: balance for %d = %d\n", ID, user_data[ID-1].balance);
	sprintf(output,"%d", user_data[ID-1].balance);
}

void money_transfer(user user_data[USER_DB_LIMIT], user_config user_config_data, int ID, int fromID,int fromPIN, int amount, char output[]){
	// TODO: Change input to sessionID then query for matchedID
	if(amount<0 || fromPIN!=user_data[fromID-1].hashed_pin){
		strcpy(output,"FAILURE");
		return ;
	}
	user_data[fromID-1].balance-=amount;
	user_data[ID-1].balance+=amount;
	save_user(user_data, user_config_data);
	strcpy(output,"SUCCESS");
}
int change_user_balance(user user_data[USER_DB_LIMIT], user_config user_config_data, int ID, int pin, int delta){
	/*
	 * Require Role Assignement
	 * Registration Center
	 */
	if(delta<0 || pin!=user_data[ID-1].hashed_pin) return 0;
	user_data[ID-1].balance+=delta;
	save_user(user_data, user_config_data);
	return 1;
}


int change_user_role(user user_data[USER_DB_LIMIT], user_config user_config_data, int ID, char input[]){	
	for(int i=0;i<20;++i){
		if(input[i]=='1') user_data[ID-1].role[i]='1';
		else user_data[ID-1].role[i]='0';
	}
	save_user(user_data, user_config_data);
	return 1;
}

int set_user_sticker(user user_data[USER_DB_LIMIT], user_config user_config_data, int ID, char input[]){	
	for(int i=0;i<20;++i){
		if(input[i]=='1') user_data[ID-1].sticker[i]='1';
		else user_data[ID-1].sticker[i]='0';
	}
	save_user(user_data, user_config_data);
	return 1;
}
void view_user_role(user user_data[USER_DB_LIMIT], int ID, char output[]){
	for(int i=0;i<20;++i) output[i]=(user_data[ID-1].role[i]=='1')?'1':'0';
}


void view_user_sticker(user user_data[USER_DB_LIMIT], int ID, char output[]){
	for(int i=0;i<20;++i) output[i]=(user_data[ID-1].sticker[i]=='1')?'1':'0';
}

