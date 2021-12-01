#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chat1002.h"
extern jarvis_node *headptr_what;
extern jarvis_node *headptr_where;
extern jarvis_node *headptr_who;

/*
		Description: This function safely concatenates an array of strings.
		Arguments:
			ptr_dest 		[char *]:	The pointer to the ptr_destination.
			ptr_src 		[char *[]]:	The pointer to the source array of strings.
			ptr_src_size	[size_t]:	The number of elements in ptr_src.
			n			[size_t]:	The maximum size of the ptr_destination.
			offset		[int]:		The position of ptr_src to start from.
	*/
void str_concat(char *ptr_dest, char *ptr_src[], size_t ptr_src_size, size_t n, int offset)
{
	size_t length_to_check, remainder, last;

	for (int i = offset; i < ptr_src_size; i++)
	{
		// Calculate the length of the string to be concatenated.
		if (i != (ptr_src_size - 1))
		{
			// Checking for additinal spaces.
			length_to_check = strlen(ptr_dest) + strlen(ptr_src[i]) + 1;
		}
		else
		{
			length_to_check = strlen(ptr_dest) + strlen(ptr_src[i]);
		}

		if (length_to_check < n)
		{
			// Check if "ptr_dest" still have any space left for the next string
			strncat(ptr_dest, ptr_src[i], n);

			if (i != (ptr_src_size - 1))
			{
				// Add a space between strings.
				strcat(ptr_dest, " ");
			}
		}
		else
		{
			// Check if the string is too long.
			remainder = n - strlen(ptr_dest);
			if (remainder > 0)
			{
				strncat(ptr_dest, ptr_src[i], remainder);
			}
			// Break the loop if there is no space left.
			break;
		}
	}
	// Ensure that string isn't null terminated.
	last = strlen(ptr_dest);
	ptr_dest[last] = '\0';
}

/*
		Description: This function is to free up all nodes in a linked list.
		Arguments:
			node 	[jarvis_node *]:	The head node of the linked list.
	*/
void node_free(jarvis_node *node)
{
	jarvis_node *current;
	while (node != NULL)
	{
		current = node;
		node = node->next;
		free(current);
	}
}

/*
		Description: This function will creates a node.
		Arguments:
			entity 	[char *]:	The entity.
			resp 	[char *]:	The response.
		Returns:
			node	[jarvis_node *]:	The newly created node.
			NULL	[NULL]:		Returns NULL if memory allocation fails.
	*/
jarvis_node *jarvis_node_create(const char *entity, const char *resp)
{
	jarvis_node *node = malloc(sizeof(jarvis_node));
	if (node == NULL)
	{
		printf("[!] Memory allocation failure.\n");
		return NULL;
	}
	snprintf(node->entity, MAX_LEN_ENTITY, "%s", entity);
	snprintf(node->response, MAX_LEN_RESPONSE, "%s", resp);
	node->next = NULL;
	return node;
}

/*
		Description: This function adds a new node to a linked list.
		Arguments:
			head 	[jarvis_node *]:	The head node of the linked list.
			node 	[jarvis_node *]:	The new node to add to the linked list.
	*/
void node_append(jarvis_node *head, jarvis_node *node)
{
	jarvis_node *current = head;
	while (current != NULL)
	{
		// Check if entity already exists.
		if (compare_token(current->entity, node->entity) == 0)
		{
			// Overwrite the response
			snprintf(current->response, MAX_LEN_RESPONSE, "%s", node->response);
			break;
		}
		else if (current->next == NULL)
		{
			current->next = node;
			break;
		}
		current = current->next;
	}
}

/*
		Description: This function adds a new node to the "WHAT" knowledge base linked list.
		Arguments:
			new_node 	[jarvis_node *]:	The new node to add to the knowledge base.
	*/
int knowledge_update_what(jarvis_node *new_node)
{
	// Check if allocation to memory failed.
	if (new_node == NULL)
	{
		return KNOWLEDGE_MEMALLOC_FAIL;
	}
	// Check if the linked list is empty.
	if (headptr_what == NULL)
	{
		// Assign new_node to the first node of the list.
		headptr_what = new_node;
	}
	else
	{
		// Append new_node to the WHAT linked list.
		node_append(headptr_what, new_node);
	}
	return KNOWLEDGE_FOUND;
}

/*
		Description: This function adds a new node to the "WHERE" knowledge base linked list.
		Arguments:
			new_node 	[jarvis_node *]:	The new node to add to the knowledge base.
	*/
int knowledge_update_where(jarvis_node *new_node)
{
	// Check if allocation to memory failed.
	if (new_node == NULL)
	{
		return KNOWLEDGE_MEMALLOC_FAIL;
	}
	// Check if the linked list is empty.
	if (headptr_where == NULL)
	{
		// Assign new_node to the first node of the list.
		headptr_where = new_node;
	}
	else
	{
		// Append new_node to the WHERE linked list.
		node_append(headptr_where, new_node);
	}
	return KNOWLEDGE_FOUND;
}

/*
		Description: This function adds a new node to the "WHO" knowledge base linked list.
		Arguments:
			new_node 	[jarvis_node *]:	The new node to add to the knowledge base.
	*/
int knowledge_update_who(jarvis_node *new_node)
{
	// Check if allocation to memory failed.
	if (new_node == NULL)
	{
		// Failed to allocate memory.
		return KNOWLEDGE_MEMALLOC_FAIL;
	}
	// Check if the linked list is empty.
	if (headptr_who == NULL)
	{
		// Assign new_node to the first node of the list.
		headptr_who = new_node;
	}
	else
	{
		// Append new_node to the WHERE linked list.
		node_append(headptr_who, new_node);
	}
	return KNOWLEDGE_FOUND;
}
