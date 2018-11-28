#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//#include "pap.h" 

typedef enum login_input 
{
	RAR_PLUS,
	RAR_MINUS,	
}INPUT;


typedef enum login_output
{
	SAA,
	SAN
	
}OUTPUT;

typedef enum program_state
{
	ACK,
	OPEN,
	TRY2,
	TRY3,
	CLOSE
}STATE;

char *strtrim(char * s)
{
	char *end = (char*)(s + strlen(s) - 1);
	
	while(s <= end && isspace(*s))  
		s++;
	
	while(s <= end && isspace(*end))
		end--;
	*(end + 1) = '\0';
	
	return s;
}

INPUT check_login(char *login, char *password)
{

	FILE *fp = NULL;
	char *db_info = NULL;
	ssize_t res;
	size_t n = 0;
	char *access_str = NULL, *logint = NULL, *passwordt = NULL;
	INPUT result = RAR_MINUS;

	logint = strtrim(login);
	passwordt = strtrim(password);

	access_str = (char*)malloc(sizeof(char) * (2 + strlen(logint) +strlen(passwordt)));

	sprintf(access_str, "%s %s", logint, passwordt); 


	/*magical database search goes here...*/
	fp = fopen("./users.db", "r");
	if(!fp)
	{
		printf("501- Error opening users' database!\n");
		exit(501);
	}
	
	while((res = getline(&db_info, &n, fp)) != -1)
		if(!strcmp(strtrim(db_info), access_str))
		{
			result = RAR_PLUS;
			break;
		}
	
	fclose(fp);
	//free(db_info); //this is useless... isn't it?... "This buffer should be freed by the user program even if getline() failed" from http://man7.org/linux/man-pages/man3/getline.3.html
	
	//free(access_str); //also this...
	return result;
}

int pap()
{
	char *user = NULL;
	char *password = NULL;
	size_t nu = 0, np = 0;
	int success = 0, tries = 0;
	INPUT input;
	
	//while(tries <= 3).... this shoule be like this... but, not complete
	while(1)
	{
		//let's get our stuff done!
		printf("User: ");
		if(getline(&user, &nu, stdin) == -1)
			printf ("502 - Error readning standard input!\n");
		
		printf("Password: ");
		if(getline(&password, &np, stdin) == -1)
			printf ("502 - Error readning standard input!\n");

		input = check_login(user, password); 

		if(input == RAR_MINUS)
		{
			printf("\ni=%s\to=SAN\n\n", (input==RAR_PLUS)?"RAR+":"RAR-"); //OUTPUT
			tries++;
		}
		else if(input == RAR_PLUS && (tries < 3 || success) )
		{
			printf("\ni=%s\to=SAA\n\n", (input==RAR_PLUS)?"RAR+":"RAR-"); //OUTPUT
			success = 1;
		}
		else 
		{
			printf("\ni=%s\to=SAN\n\n", (input==RAR_PLUS)?"RAR+":"RAR-"); //OUTPUT
			tries=0;
		}

	}

	free(user);	
	free(password);
	
	return 0;

}

int pap_state_imp() 
{
	char *user = NULL;
	char *password = NULL;
	size_t nu = 0, np = 0;
	STATE state = ACK;
	INPUT input;
	
	//while(current_state != CLOSE || current_state != OPEN) //this is how it should be.... if you are open you continue or somehting...
	while(1)
	{
		//let's get our stuff done!
		printf("User: ");
		if(getline(&user, &nu, stdin) == -1)
			printf ("502 - Error readning standard input!\n");
		
		printf("Password: ");
		if(getline(&password, &np, stdin) == -1)
			printf ("502 - Error readning standard input!\n");

		input = check_login(user, password); 

		if(state == ACK && input == RAR_PLUS)
		{
			printf("\ni=%s\to=SAA\n\n", (input==RAR_PLUS)?"RAR+":"RAR-"); //OUTPUT
			state = OPEN;
		}
		else if(state == ACK && input == RAR_MINUS)
		{
			printf("\ni=%s\to=SAN\n\n", (input==RAR_PLUS)?"RAR+":"RAR-"); //OUTPUT
			state = TRY2;
		}

		else if(state == TRY2 && input == RAR_PLUS)
		{
			printf("\ni=%s\to=SAA\n\n", (input==RAR_PLUS)?"RAR+":"RAR-"); //OUTPUT
			state = OPEN;
		}
		else if(state == TRY2 && input == RAR_MINUS)
		{
			printf("\ni=%s\to=SAN\n\n", (input==RAR_PLUS)?"RAR+":"RAR-"); //OUTPUT
			state = TRY3;
		}

		else if(state == TRY3 && input == RAR_PLUS)
		{
			printf("\ni=%s\to=SAA\n\n", (input==RAR_PLUS)?"RAR+":"RAR-"); //OUTPUT
			state = OPEN;
		}
		else if(state == TRY3 && input == RAR_MINUS)
		{
			printf("\ni=%s\to=SAN\n\n", (input==RAR_PLUS)?"RAR+":"RAR-"); //OUTPUT
			state = CLOSE;
		}

		else if(state == CLOSE && input == RAR_PLUS)
		{
			printf("\ni=%s\to=SAN\n\n", (input==RAR_PLUS)?"RAR+":"RAR-"); //OUTPUT
			state = ACK;
		}
		else if(state == CLOSE && input == RAR_MINUS)
		{
			printf("\ni=%s\to=SAN\n\n", (input==RAR_PLUS)?"RAR+":"RAR-"); //OUTPUT
			state = CLOSE;
		}

		else if(state == OPEN && input == RAR_PLUS)
		{
			printf("\ni=%s\to=SAA\n\n", (input==RAR_PLUS)?"RAR+":"RAR-"); //OUTPUT
			state = OPEN;
		}
		else if(state == OPEN && input == RAR_MINUS)
		{
			printf("\ni=%s\to=SAN\n\n", (input==RAR_PLUS)?"RAR+":"RAR-"); //OUTPUT
			state = OPEN;
		}
	
	}

	free(user);	
	free(password);
	
	return 0;	

}

int main()
{
	return pap();
}
