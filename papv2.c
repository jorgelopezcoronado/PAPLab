#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//#include "pap.h" 

typedef enum login_input 
{
	RAR_PLUS = 0,
	RAR_MINUS = 1,	
}INPUT;


typedef enum login_output
{
	SAA=0,
	SAN=1
	
}OUTPUT;

typedef enum program_state
{
	ACK = 0,
	OPEN = 1,
	TRY2 = 2,
	TRY3 = 3,
	CLOSE = 4
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
	int res;
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

OUTPUT get_output_reaction(int *success, int *tries, INPUT input)
{
		printf("Coverage inputs: input=%s, sucess=%i, tries=%i", (input==RAR_PLUS)?"RAR+":"RAR-", *success, *tries);
		if(input == RAR_MINUS)
		{
			*tries=*tries+1;
			return SAN;
		}
		else if(input == RAR_PLUS && (*tries < 3 || *success) )
		{
			*success = 1;
			return SAA;
		}
		else 
		{
			*tries = 0;
			return SAN;
		}
}

int pap()
{
	char *user = NULL;
	char *password = NULL;
	size_t nu = 0, np = 0;
	int success = 0, tries = 0;
	INPUT input;
	OUTPUT output;
	
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
		output = get_output_reaction(&success, &tries, input);

		printf("\ni=%s\to=%s\n\n", (input==RAR_PLUS)?"RAR+":"RAR-", (output==SAA)?"SAA":"SAN"); //OUTPUT

	}

	free(user);	
	free(password);
	
	return 0;
}

OUTPUT h(STATE *state, INPUT input)
{
		if(*state == ACK && input == RAR_PLUS)
		{
			*state = OPEN;
			return SAA;
		}
		else if(*state == ACK && input == RAR_MINUS)
		{
			*state = TRY2;
			return SAN;
		}

		else if(*state == TRY2 && input == RAR_PLUS)
		{
			*state = OPEN;
			return SAA;
		}
		else if(*state == TRY2 && input == RAR_MINUS)
		{
			*state = TRY3;
			return SAN;
		}

		else if(*state == TRY3 && input == RAR_PLUS)
		{
			*state = OPEN;
			return SAA;
		}
		else if(*state == TRY3 && input == RAR_MINUS)
		{
			*state = CLOSE;
			return SAN;
		}

		else if(*state == CLOSE && input == RAR_PLUS)
		{
			*state = ACK;
			return SAN;
		}
		else if(*state == CLOSE && input == RAR_MINUS)
		{
			*state = CLOSE;
			return SAN;
		}

		else if(*state == OPEN && input == RAR_PLUS)
		{
			*state = OPEN;
			return SAA;
		}
		else if(*state == OPEN && input == RAR_MINUS)
		{
			*state = OPEN;
			return SAN;
		}

		return SAN;
}

int pap_state_imp() 
{
	char *user = NULL;
	char *password = NULL;
	size_t nu = 0, np = 0;
	STATE state = ACK;
	INPUT input;
	OUTPUT output;
	
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

		output = h(&state, input);

		printf("\ni=%s\to=%s\n\n", (input==RAR_PLUS)?"RAR+":"RAR-", (output==SAA)?"SAA":"SAN"); //OUTPUT

	}

	free(user);	
	free(password);
	
	return 0;	

}

int main()
{
	return pap();
	//return pap_state_imp();
}
