#ifndef _CHAT1002_H
#define _CHAT1002_H

#include <stdio.h>

/* the maximum number of characters we expect in a line of input (including the terminating null)  */
#define MAX_LEN_INPUT    256

/* the maximum number of characters allowed in the name of an entity (including the terminating null)  */
#define MAX_LEN_ENTITY   64

/* the maximum number of characters allowed in a response (including the terminating null) */
#define MAX_LEN_RESPONSE 256

/* return codes for knowledge_get() and knowledge_put() */
#define KNOWLEDGE_FOUND     200
#define KNOWLEDGE_NOTFOUND  404
#define KNOWLEDGE_INVALID   500
#define KNOWLEDGE_MEMALLOC_FAIL     501


 
/* Intent-Entity-Response struct. */
typedef struct node_struct {
	char entity[MAX_LEN_ENTITY];
	char response[MAX_LEN_RESPONSE];
	struct node_struct *next; // Pointer to the next node in the linked list.
} jarvis_node;

/* Main.c defines the functions. */
int compare_token(const char *token1, const char *token2);
void prompt_user(char *buffer, int n, const char *format, ...);

/* Chatbot.c defines the functions. */
const char *chatbot_botname();
const char *chatbot_username();
int chatbot_main(int inc, char *inv[], char *response, int n);
int chatbot_is_exit(const char *intent);
int chatbot_do_exit(int inc, char *inv[], char *response, int n);
int chatbot_is_load(const char *intent);
int chatbot_do_load(int inc, char *inv[], char *response, int n);
int chatbot_is_question(const char *intent);
int chatbot_do_question(int inc, char *inv[], char *response, int n);
int chatbot_is_reset(const char *intent);
int chatbot_do_reset(int inc, char *inv[], char *response, int n);
int chatbot_is_save(const char *intent);
int chatbot_do_save(int inc, char *inv[], char *response, int n);
int chatbot_is_smalltalk(const char *intent);
int chatbot_do_smalltalk(int inc, char *inv[], char *response, int n);
int chatbot_is_update(const char *intent);
int chatbot_do_update(int inc, char *inv[], char *response, int n);

/* Knowledge.c defines the functions. */
int knowledge_get(const char *intent, const char *entity, char *response, int n);
int knowledge_put(const char *intent, const char *entity, const char *response);
void knowledge_reset();
int knowledge_read(FILE *f);
void knowledge_write(FILE *f);

/* Knowledge_node_crud.c defines the functions. */
void str_concat(char *ptr_dest, char *ptr_src[], size_t ptr_src_size, size_t n, int offset);
jarvis_node * jarvis_node_create(const char *entity, const char *resp);
void node_append(jarvis_node *head, jarvis_node *node);
void node_free(jarvis_node *node);
int knowledge_update_what(jarvis_node *new_node);
int knowledge_update_where(jarvis_node *new_node);
int knowledge_update_who(jarvis_node *new_node);

/* Initalise loading bar for the animation, this is for saving function */
void loadingBar();

/* Chatbot Linked lists, our chatbot named Jarvis. */
jarvis_node *headptr_what;
jarvis_node *headptr_where;
jarvis_node *headptr_who;

#endif
