#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "chat1002.h"
extern jarvis_node *headptr_what;
extern jarvis_node *headptr_where;
extern jarvis_node *headptr_who;

/* Get the name of the chatbot. Returns: the name of the chatbot as a null-terminated string */
const char *chatbot_botname()
{
	return "JARVIS";
}

/* Get the name of the user. Returns: the name of the user as a null-terminated string */
const char *chatbot_username()
{
	return "User";
}

/* This function remove a newline from a string */
void remove_char(char *str, char charToRemmove)
{
	int len = strlen(str);
	int i, j;
	for (i = 0; i < len; i++)
	{
		if (str[i] == charToRemmove)
		{
			for (j = i; j < len; j++)
			{
				str[j] = str[j + 1];
			}
			len--;
			i--;
		}
	}
}

/* This function will validate whether the user enter y/n it will re-ask if it detect anything outside of y/n. */
void userinput_ask(char *new_input, int n)
{
tryagain:
	fgets(new_input, n, stdin);
	size_t len = strlen(new_input);
	if (new_input[len - 1] != '\n')
	{
		printf("Please enter only y/n! (y/n)\n");
		fseek(stdin, 0, SEEK_END); //to clear the input buffer
		goto tryagain;
	}
	else
	{
		while (!(tolower(new_input[0]) == 'y' || tolower(new_input[0]) == 'n') || (tolower(new_input[0]) == ' '))
		{
			printf("Please enter Y/N only! (y/n)\n");
			userinput_ask(new_input, n);
		}
	}
}


char yesno[4];
char save_yn[4];
char new_filename[255];
int checker_exit = 0;
int checker_reset = 0;

/* This function will get a response from the user input, 
run the function based on the user input, and 
return 0 if the chatbot is to continue, return 1 if the chatbot is to stop  */
int chatbot_main(int inc, char *inv[], char *response, int n)
{
	checker_exit = 0;
	checker_reset = 0;
	/* look for an intent and invoke the corresponding do_* function */

	// Check if the userinput is exiting the program
	if (chatbot_is_exit(inv[0]))
	{
		/* Validate whether user wan to save the knowledege of the chatbot before exiting.  */
		checker_exit = 1;
		printf("Do you want to save the chat history before exiting? (y/n)\n");
		userinput_ask(yesno, 3);
		if (tolower(yesno[0]) == 'y')
		{
			printf("Name of file to save to: (eg. 'test.txt')\n");
			fgets(new_filename, 255, stdin);
			fseek(stdin, 0, SEEK_END);		//to clear the input buffer
			remove_char(new_filename, '\n'); //remove \n from input
			while (strlen(new_filename) == 0)
			{
				printf("No filepath detected!\n");
				// Ask for a file path and save to that file
				printf("Please enter the filepath to save to: (eg. 'test.txt')\n");
				fgets(new_filename, 255, stdin);
				remove_char(new_filename, '\n'); //remove \n from input
			}
			//replace '\0' with '_'
			int qq = 0;
			while (new_filename[qq])
			{
				if ((isspace(new_filename[qq])) > 0) //if white-space character, we change to underscore
				{
					new_filename[qq] = '_';
				}
				qq++;
			}
			inv[1] = new_filename;
			return chatbot_do_save(inc, inv, response, n);
		}
		else
			return chatbot_do_exit(inc, inv, response, n);
	}
	else if (chatbot_is_smalltalk(inv[0]))
	// Check if the userinput is smalltalk
		return chatbot_do_smalltalk(inc, inv, response, n);
	else if (chatbot_is_load(inv[0]))
	// Check if the user want to load a knowledgde file
		return chatbot_do_load(inc, inv, response, n);
	else if (chatbot_is_question(inv[0]))
	// Check if the user input is a question
		return chatbot_do_question(inc, inv, response, n);
	else if (chatbot_is_reset(inv[0]))
	{
	// Check if the chatbot user input is reset,
	// checker_reset variable have been implemented to prevent the chatbot from resetting itself.
		checker_reset = 1;
		return chatbot_do_reset(inc, inv, response, n);
	}
	else if (chatbot_is_update(inv[0]))
		// Check if the chatbot is update, 
		return chatbot_do_update(inc, inv, response, n);
	else if (chatbot_is_save(inv[0]))
	{
		// Check if the chatbot user input is save, 
		/* Validate if there is no file path being input by the user.  */
		if (inv[1] == NULL)
		{
			printf("No filepath detected!'\n");
			// Ask for a file path and save to that file
			printf("Please enter the filepath to save to: (eg. 'test.txt')\n");
			fgets(new_filename, 255, stdin);
			remove_char(new_filename, '\n'); 
			while (strlen(new_filename) == 0)
			{
				printf("No filepath detected!\n");
				// Ask for a file path and save to that file
				printf("Please enter the filepath to save to: (eg. 'test.txt')\n");
				fgets(new_filename, 255, stdin);
				//remove \n from input
				remove_char(new_filename, '\n'); 
			}
			//to clear the input buffer
			fseek(stdin, 0, SEEK_END); 
			//replace '\0' with '_'
			int qq = 0;
			while (new_filename[qq])
			{
				if ((isspace(new_filename[qq])) > 0) //if white-space character, we change to underscore
				{
					new_filename[qq] = '_';
				}
				qq++;
			}
			inv[1] = new_filename;
			return chatbot_do_save(inc, inv, response, n);
		}
		/*Confirmation to save*/
		printf("Are you sure you want to save the chat history? (y/n)\n");
		/* Validate user input.  */
		userinput_ask(save_yn, 3);
		if (tolower(save_yn[0]) == 'y')
		{
			return chatbot_do_save(inc, inv, response, n);
		}
		else
			return 0;
	}
	else
	{
		snprintf(response, n, "I don't understand \"%s\".", inv[0]);
		return 0;
	}
}


/*
 * Description: Determine whether an intent is Exit.
 * Input: INTENT
 * Returns:
 *  1, if the intent is "exit" or "quit"
 *  0, otherwise
 */
int chatbot_is_exit(const char *intent)
{
	return (
		compare_token(intent, "exit") == 0 ||
		compare_token(intent, "quit") == 0);
}

/*
 * Description: This function is to exit the program.
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_exit(int inc, char *inv[], char *response, int n)
{
	// Free the allocated memory.
	knowledge_reset();
	snprintf(response, n, "Goodbye!");
	return 1;
}

/*
 * Description: Determine whether an intent is LOAD.
 * Input: INTENT
 * Returns:
 *  1, if the intent is "load"
 *  0, otherwise
 */
int chatbot_is_load(const char *intent)
{
	return compare_token(intent, "load") == 0;
}

/*
 * Description: This function is used to load the knowledge base from a file.
 * Returns:
 *   0 (the chatbot always continues chatting after loading knowledge)
 */
int chatbot_do_load(int inc, char *inv[], char *response, int n)
{
	/*
		fp:		The file pointer.
		ctr:	The number of successful results retrieved from the file.
	*/

	if (inv[1] != NULL)
	{
		FILE *fp;
		int ctr = 0;
		char file_path[MAX_LEN_INPUT];

		// Get the file path from the user input.
		if (compare_token(inv[1], "from") == 0)
		{
			// LOAD[0] from[1] /path/to/file[2]
			strcpy(file_path, inv[2]);
		}
		else
		{
			// LOAD[0] /path/to/file[1]
			strcpy(file_path, inv[1]);
		}

		// Open the file in read mode.
		fp = fopen(file_path, "r");

		if (fp != NULL)
		{
			// File exists.
			ctr = knowledge_read(fp);
			fclose(fp);
			snprintf(response, n, "I have loaded %d results from the knowledge base [%s].", ctr, file_path);
		}
		else
		{
			// File does not exist.
			snprintf(response, n, "Sorry, I can't load the knowledge base [%s].", file_path);
		}
	}
	else
	{
		snprintf(response, n, "Sorry, A file name is required");
	}

	return 0;
}

/*
 * Description: Determine whether an intent is a question.
 * Input: INTENT
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_question(const char *intent)
{
	return (
		compare_token(intent, "what") == 0 ||
		compare_token(intent, "where") == 0 ||
		compare_token(intent, "who") == 0);
}

/*
 * Description: This function is used to answer a question.
 * inv[0] contains the the question word.
 * inv[1] may contain "is" or "are"; if so, it is skipped.
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_question(int inc, char *inv[], char *response, int n)
{
	/*
		unsure:		A string for unsure questions.
		enti:		A string to store the entity.
		ans:		A string to store the answer to the question.
	*/
	char unsure[MAX_LEN_RESPONSE] = "I don't know. ";
	char enti[MAX_LEN_ENTITY] = "";
	char ans[MAX_LEN_RESPONSE] = "";

	size_t offset = 1;

	// Check where does the question start. If inv[1] is empty, if it is, re-enter question.
	if (inv[1] != NULL)
	{
		if (
			compare_token(inv[1], "is") == 0 ||
			compare_token(inv[1], "are") == 0)
		{
			offset = 2;
		}

		str_concat(enti, inv, inc, (MAX_LEN_ENTITY - 1), offset);

		if (knowledge_get(inv[0], enti, response, n) == KNOWLEDGE_NOTFOUND)
		{
			str_concat(unsure, inv, inc, (MAX_LEN_RESPONSE - 1), 0);
			strcat(unsure, "?");

			prompt_user(ans, MAX_LEN_RESPONSE, "%s", unsure);
			knowledge_put(inv[0], enti, ans);
			snprintf(response, n, "Thank you.");
		}
	}
	else
	{
		snprintf(response, n, "Sorry, what are you asking?");
	}
	return 0;
}

/*
 * Description: Determine whether an intent is RESET.
 * Input: INTENT
 * Returns:
 *  1, if the intent is "reset"
 *  0, otherwise
 */
int chatbot_is_reset(const char *intent)
{
	return compare_token(intent, "reset") == 0;
}

/*
 * Description: This funtion is to reset the chatbot's knowledge base.
 * Returns:
 *   0 (the chatbot always continues chatting after beign reset)
 */
int chatbot_do_reset(int inc, char *inv[], char *response, int n)
{
	// Reseed the random number generator.
	srand((unsigned)(time(NULL)));
	if (checker_reset == 1)
	{
		// Reset the knowledge base in memory.
		knowledge_reset();
		snprintf(response, n, "I have reset my knowledge for this session.");
	}
	else {
		snprintf(response, n, "Hello, welcome back :)");
	}
	return 0;
}

/*
 * Description: Determine whether an intent is SAVE.
 * Input: INTENT
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_save(const char *intent)
{
	return compare_token(intent, "save") == 0;
}

/*
 * Description: This function is used to save the knowledge base to a file.
 * Returns:
 *   0 (the chatbot always continues chatting after saving knowledge)
 */
int chatbot_do_save(int inc, char *inv[], char *response, int n)
{
	/*fp: The file pointer.*/
	FILE *fp;
	char file_path[MAX_LEN_INPUT];
	char full_name[MAX_LEN_INPUT];
	char underscore[] = "_";

	// Get the file path from the user input.
	if (
		compare_token(inv[1], "to") == 0 ||
		compare_token(inv[1], "as") == 0)
	{
		// Save[0] to[1] /path/to/file[2]
		// Save[0] as[1] /path/to/file[2]
		strcpy(full_name, inv[2]);

		for (int i = 3; i < inc; i++)
		{
			strcat(full_name, underscore);
			strcat(full_name, inv[i]);
		}
		strcpy(file_path, full_name);
	}
	else
	{
		// save[0] /path/to/file[1]
		strcpy(full_name, inv[1]);

		for (int i = 2; i < inc; i++)
		{
			strcat(full_name, underscore);
			strcat(full_name, inv[i]);
		}
		strcpy(file_path, full_name);
	}

	// Check if file exist, if not, create it.
	fp = fopen(file_path, "r");
	if (fp == NULL)
	{
		fp = fopen(file_path, "w");
		knowledge_write(fp);
		fclose(fp);
		// printf("Saving...\n");
		loading_bar();
		printf("I have saved the results from the knowledge base to [%s].\n", file_path);
		if (checker_exit == 1)
		{
			printf("Exiting program now.\n");
			exit(0);
		}
		else
		{
			fseek(stdin, 0, SEEK_END); //to clear the input buffer
			return 0;
		}
	}
	else
	{
		fclose(fp);
		// Ask if user want to overwrite the file.
		printf("File exist, do you want to overwrite it? (y/n)\n");
		char overwrite[4];
		userinput_ask(overwrite, 5);
		if (tolower(overwrite[0]) == 'y')
		{
			// printf("Saving...\n");
			loading_bar();
			fp = fopen(file_path, "w");
			knowledge_write(fp);
			fclose(fp);
			printf("I have overwritten the results from the current knowledge base to [%s].\n", file_path);
			if (checker_exit == 1)
			{
				printf("Exiting program now.\n");
				exit(0);
			}
			else
			{
				return 0;
			}
		}
		else
		{
			printf("I have not overwritten the results from the knowledge base to [%s].\n", file_path);
			return 0;
			// if (checker_exit == 1)
			// {
			// 	printf("Exiting program now.\n");
			// 	exit(0);
			// }
			// else
			// {
			// 	return 0;
			// }
		}
	}
	return 0;
}

// Initalise small talk 2d Array
const char *smalltalk[][20] = {
	{"can", "do", "are", "how", "why"},					   // 0 - Bot Question
	{"good"},											   // 1 - Greeting
	{"hello", "hey", "hi", "whatsup", "wassup", "waddup"}, // 2 - Greeting 2
	{"it", "its", "it's"},								   // 3 - Statement
	{"time"},											   // 4 - Time
	{"i", "i'm", "im", "me"},							   // 5 - User
	{"You"},											   // 6 - Bot statement
	{"Thanks", "Thank", "Appreciate", "Xie"}			   // 7 - Thanks
};

// Initalise small talk responses arrays
const char *random_greeting[] = {"Hi!", "Hello!", "Hello there!", "Hey hey~", "What's Up!!"};
//const char *random_question[] = {"What do you think?", "Maybe I could!", "Well, it's either a yes or a no."};
const char *random_response[] = {"Why?", "Good to know!", "I see.", "Tell me more~", "Oh!"};
const char *random_thanks[] = {"No problem!", "You're Welcome!", "Don't sweat it~", "Ain't no thing"};
const int smalltalk_size = sizeof(smalltalk) / sizeof(smalltalk[0]);

/*
 * Description: Transpose the subject of the user's question between "You" and "I", Example "Do u know SIT?", the chatbot will return "I know SIT?"
 * Returns:
 *  void, makes changes on the user input
 */
void chatbot_smalltalk_transpose(int inc, char *inv[], char *response, int n)
{
	for (int i = 0; i < inc; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			if (smalltalk[5][j] != NULL && smalltalk[6][j] != NULL)
			{
				if (compare_token(smalltalk[5][j], inv[i]) == 0)
				{
					inv[i] = "you";
				}
				if (compare_token(smalltalk[6][j], inv[i]) == 0)
				{
					inv[i] = "I";
				}
			}
		}
	}
}

/*
 * Description: Determine which an intent is smalltalk.
 * Input: INTENT
 * Returns:
 *  1, if the intent is the first word of one of the smalltalk phrases
 *  0, otherwise
 */
int chatbot_is_smalltalk(const char *intent)
{

	for (int i = 0; i < smalltalk_size; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			if (smalltalk[i][j] != NULL)
			{
				if (compare_token(intent, smalltalk[i][j]) == 0)
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

/*
 * Respond to smalltalk.
*  Description: This function is to do small talk, it will return a response to the user based on the intent.
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop chatting (e.g. the smalltalk was "goodbye" etc.)
 */
int chatbot_do_smalltalk(int inc, char *inv[], char *response, int n)
{

	size_t rand_int = (size_t)(rand() % sizeof(random_greeting) / sizeof(random_greeting[0])); //Random integer for deciding response
	if (compare_token("time", inv[0]) == 0)
	{ // SMALLTALK: "Time" feature.
		char the_time[24];
		time_t timer = time(NULL);
		strftime(the_time, 24, "%I:%M %p, %d %b %Y", localtime(&timer));
		snprintf(response, n, "It is now %s.", the_time);
	}

	for (int i = 0; i < 20; i++)
	{
		// - Bot Questions
		if (smalltalk[0][i] != NULL)
		{
			if (compare_token(smalltalk[0][i], inv[0]) == 0)
			{
				if (inc > 1)
				{
					if (compare_token(inv[0], "how") == 0 && compare_token(inv[1], "are") == 0)
					{
						snprintf(response, n, "Good!");
						break;
					}
					else
					{
						char user_question[MAX_LEN_RESPONSE] = "I don't know. ";
						char answer[MAX_LEN_RESPONSE] = "";
						str_concat(user_question, inv, inc, (MAX_LEN_RESPONSE - 1), 0);
						strcat(user_question, "?");
						snprintf(response, n, "%s", user_question);
						break;
					}
				}
				else
				{
					snprintf(response, n, "%s what?", inv[0]);
					break;
				}
			}
		}
		// - Greetings
		if (smalltalk[1][i] != NULL)
		{
			if (compare_token(smalltalk[1][i], inv[0]) == 0)
			{
				if (inc > 1)
				{
					snprintf(response, n, "Good %s to you too!", inv[1]);
					break;
				}
				else
				{
					snprintf(response, n, "Good day!");
					break;
				}
			}
		}
		// - Greetings 2
		if (smalltalk[2][i] != NULL)
		{
			if (compare_token(smalltalk[2][i], inv[0]) == 0)
			{
				size_t rand_greeting = (size_t)(rand() % sizeof(random_greeting) / sizeof(random_greeting[0]));
				snprintf(response, n, "%s", random_greeting[rand_greeting]);
				break;
			}
		}
		// - Statement
		if (smalltalk[3][i] != NULL)
		{
			if (compare_token(smalltalk[3][i], inv[0]) == 0)
			{
				snprintf(response, n, "Indeed it is.");
				break;
			}
		}
		// - User & Bot statement
		if (smalltalk[5][i] != NULL && smalltalk[6][i] != NULL)
		{
			if (compare_token(smalltalk[5][i], inv[0]) == 0 || compare_token(smalltalk[6][i], inv[0]) == 0)
			{
				size_t rand_response = (size_t)(rand() % sizeof(random_response) / sizeof(random_response[0]));
				snprintf(response, n, "%s", random_response[rand_response]);
				break;
			}
		}
		// - Bot statement
		if (smalltalk[6][i] != NULL)
		{
			if (compare_token(smalltalk[6][i], inv[0]) == 0)
			{
				size_t rand_response = (size_t)(rand() % sizeof(random_response) / sizeof(random_response[0]));
				snprintf(response, n, "%s", random_response[rand_response]);
			}
		}
		// - Thanks
		if (smalltalk[7][i] != NULL)
		{
			if (compare_token(smalltalk[7][i], inv[0]) == 0)
			{
				size_t rand_response = (size_t)(rand() % sizeof(random_thanks) / sizeof(random_thanks[0]));
				snprintf(response, n, "%s", random_thanks[rand_response]);
			}
		}
	}

	return 0;
}

/*
 * Determine whether an intent is a update.
 *
 * Input:
 *  intent - the intent
 * Returns:
 *  1, if the intent is update
 *  0, otherwise
 */
int chatbot_is_update(const char *intent)
{
	return compare_token(intent, "update") == 0;
}

/*
 * Respond to update.
 * Description: This function is to update a already exist entity, if the entity is not exist, it will create a new entity.	
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop chatting (e.g. the smalltalk was "goodbye" etc.)
 */
int chatbot_do_update(int inc, char *inv[], char *response, int n)
{
	if (inv[1] != NULL && inv[2] != NULL && (strcmp(inv[1], "where") == 0 || strcmp(inv[1], "what") == 0 || strcmp(inv[1], "how") == 0))
	{
		char NewMeaning[MAX_LEN_RESPONSE] = "Enter the new meaning.";
		char enti[MAX_LEN_ENTITY] = "";
		char ans[MAX_LEN_RESPONSE] = "";
		size_t offset = 1;

		// Check where does the question start.
		if (

			compare_token(inv[2], "is") == 0 ||
			compare_token(inv[2], "are") == 0)
		{
			offset = 3;
		}

		str_concat(enti, inv, inc, (MAX_LEN_ENTITY - 1), offset);
		knowledge_get(inv[1], enti, response, n);
		prompt_user(ans, MAX_LEN_RESPONSE, "%s", NewMeaning);
		knowledge_put(inv[1], enti, ans);
		snprintf(response, n, "Meaning updated.");
	}
	else
	{
		snprintf(response, n, "Sorry, what are you trying to update?");
	}
	return 0;
}
