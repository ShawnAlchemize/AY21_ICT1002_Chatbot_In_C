/*
 * ICT1002 Assignment 2, 2018-19 Trimester 2.
 *
 * This file implements the chatbot's knowledge base.
 *
 * knowledge_get() retrieves the response to a question.
 * knowledge_put() inserts a new response to a question.
 * knowledge_read() reads the knowledge base from a file.
 * knowledge_reset() erases all of the knowledge.
 * kowledge_write() saves the knowledge base in a file.
 *
 * You may add helper functions as necessary.
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "chat1002.h"
extern jarvis_node *headptr_what;
extern jarvis_node *headptr_where;
extern jarvis_node *headptr_who;


/*
 * Get the response to a question.
 *
 * Input:
 *   intent   - the question word
 *   entity   - the entity
 *   response - a buffer to receive the response
 *   n        - the maximum number of characters to write to the response buffer
 *
 * Returns:
 *   KNOWLEDGE_FOUND, if a response was found for the intent and entity (the response is copied to the response buffer)
 *   KNOWLEDGE_NOTFOUND, if no response could be found
 *   KNOWLEDGE_INVALID, if 'intent' is not a recognised question word
 */
int knowledge_get(const char *intent, const char *entity, char *response, int n) {
	jarvis_node *search;
	int result = KNOWLEDGE_NOTFOUND;

	if (compare_token(intent, "what") == 0) {
		// Intent is what.
		search = headptr_what;
	} else if (compare_token(intent, "where") == 0) {
		// Intent is where.
		search = headptr_where;
	} else if (compare_token(intent, "who") == 0) {
		// Intent is who.
		search = headptr_who;
	} else {
		// Invalid intent.
		return KNOWLEDGE_INVALID;
	}

	while(search != NULL){
		if (compare_token(entity, search->entity) == 0){
			// Found response in knowledge base.
			snprintf(response, n, "%s" , search->response);
			result = KNOWLEDGE_FOUND;
			break;
		}
		search = search->next;
	}

	return result;
}


/*
 * Insert a new response to a question. If a response already exists for the
 * given intent and entity, it will be overwritten. Otherwise, it will be added
 * to the knowledge base.
 *
 * Input:
 *   intent    - the question word
 *   entity    - the entity
 *   response  - the response for this question and entity
 *
 * Returns:
 *   KNOWLEDGE_FOUND, if successful
 *   KNOWLEDGE_MEMALLOC_FAIL, if there was a memory allocation failure
 *   KNOWLEDGE_INVALID, if the intent is not a valid question word
 */
int knowledge_put(const char *intent, const char *entity, const char *response) {
	int result;
	if (compare_token(intent, "what") == 0) {
		// Intent is what.
		result = knowledge_update_what(jarvis_node_create(entity, response));
	} else if (compare_token(intent, "where") == 0) {
		// Intent is where.
		result = knowledge_update_where(jarvis_node_create(entity, response));
	} else if (compare_token(intent, "who") == 0) {
		// Intent is who.
		result = knowledge_update_who(jarvis_node_create(entity, response));
	} else {
		// Invalid intent.
		result = KNOWLEDGE_INVALID;
	}

	return result;
}


/*
 * Read a knowledge base from a file.
 *
 * Input:
 *   f - the file
 *
 * Returns: the number of entity/response pairs successful read from the file
 */
int knowledge_read(FILE *f) {
	/*
		ctr:	The number of successful results retrieved from the file.
		result:	The result of inserting the entity-response into the knowledge base.
		line:	The buffer to store content of the file.
	*/
	int ctr = 0, result;
	char line[MAX_LEN_INPUT];
	char *f_intent;

	const size_t MAX_READLEN = (size_t)(MAX_LEN_ENTITY + MAX_LEN_RESPONSE);

	while (fgets(line, MAX_READLEN, (FILE*) f)) {
		char *f_entity, *f_response;
		char *cr, *newline;

		// Read line by line.
		if (
			strcmp(line, "\n") == 0 ||
			strcmp(line, "\r\n") == 0
		) {
			// Empty line.
			continue;
		} else {
			cr = strchr(line, '\r');
			newline = strchr(line, '\n');

			if (cr != NULL) {
				// Carriage return found, replace it with null terminator.
				*cr = '\0';
			} else if (newline != NULL) {
				// newline found, replace it with null terminator.
				*newline = '\0';
			} else {
				// Clear any remaining input to prevent overflow.
				int c;
				while ((c = getchar()) != '\n' && c != EOF) {
					continue;
				}
			}

			if (
				strchr(line, '[') != NULL &&
				strchr(line, ']') != NULL
			) {
				// Square brackets found. Check intent.
				if (compare_token(line, "[what]") == 0) {
					// Intent: what.
					f_intent = "what";
				} else if (compare_token(line, "[where]") == 0) {
					// Intent: where.
					f_intent = "where";
				} else if (compare_token(line, "[who]") == 0) {
					// Intent: who.
					f_intent = "who";
				} else {
					// Invalid intent.
					f_intent = NULL;
				}
			} else if (
				f_intent != NULL &&
				strchr(line, '=') != NULL
			) {
				// Entity-Response pair line.
				f_entity = strtok(line, "=");
				f_response = strtok(NULL, "=");
				result = knowledge_put(f_intent, f_entity, f_response);

				if (result == KNOWLEDGE_FOUND) {
					// Increment the successful counter.
					ctr++;
				}
			}
		}
	}

	return ctr;
}


/*
 * Reset the knowledge base, removing all know entities from all intents.
 */
void knowledge_reset() {
	node_free(headptr_what);
	node_free(headptr_where);
	node_free(headptr_who);

	headptr_what = NULL;
	headptr_where = NULL;
	headptr_who = NULL;
}


/*
 * Write the knowledge base to a file.
 *
 * Input:
 *   f - the file
 */
void knowledge_write(FILE *f) {
	jarvis_node *what = headptr_what;
	jarvis_node *where = headptr_where;
	jarvis_node *who = headptr_who;

	// Save "what" intent's entitiy-responses.
	fprintf(f, "[what]\n");
	while (what != NULL) {
		fprintf(f, "%s=%s\n", what->entity, what->response);
		what = what->next;
	}

	// Save "where" intent's entitiy-responses.
	fprintf(f, "\n[where]\n");
	while (where != NULL) {
		fprintf(f, "%s=%s\n", where->entity, where->response);
		where = where->next;
	}

	// Save "what" intent's entitiy-responses.
	fprintf(f, "\n[who]\n");
	while (who != NULL) {
		fprintf(f, "%s=%s\n", who->entity, who->response);
		who = who->next;
	}
}
