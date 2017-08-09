#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { DIRECTORY, A_FILE } FileType;

typedef struct file_system_element
{
	char* elemName;
	FileType elemType;
} FSElement;

typedef struct node_t
{
	FSElement *Data;
	bool isRoot;
	struct node_t* firstChild;
	struct node_t* nextSibling;
} Node;

Node* createNode(FSElement *data, bool is_root)
{
	Node *new_node = NULL;

	if (data == NULL) 
		return NULL;

	new_node = (Node*)malloc(sizeof(Node));
	if (new_node == NULL)
		return NULL;

	new_node->Data = data;
	new_node->isRoot = is_root;
	new_node->firstChild = NULL;
	new_node->nextSibling = NULL;
	return new_node;
}

Node* createAndInitNode(FSElement *data, bool is_root, Node* first_child, Node* next_sibling)
{
	Node* new_node = createNode(data, is_root);
	
	if (new_node == NULL)
		return NULL;

	new_node->firstChild = first_child;
	new_node->nextSibling = next_sibling;
	return new_node;
}

FSElement* createFSElement(char* elem_name, FileType file_type)
{
	FSElement *fs_elem = NULL;
	
	if (elem_name == NULL)
		return NULL;

	fs_elem = (FSElement*)malloc(sizeof(FSElement));

	if (fs_elem == NULL)
		return NULL;

	fs_elem->elemName = elem_name;
	fs_elem->elemType = file_type;
	return fs_elem;
}

int compareNodes(Node* node1, Node* node2)
{
	FSElement *fs_elem1 = NULL, *fs_elem2 = NULL;
	
	if ((node1 == NULL) && (node2 == NULL))
		return 0;

	if (node2 == NULL)
		return -1;

	if (node1 == NULL)
		return 1;

	fs_elem1 = node1->Data;
	fs_elem2 = node2->Data;

	if (fs_elem1->elemType < fs_elem2->elemType)
		return 1;

	if (fs_elem1->elemType > fs_elem2->elemType)
		return -1;

	return strcmp(fs_elem1->elemName, fs_elem2->elemName);
}


bool addChild(Node* node, Node* child)
{
	Node *curr_node = NULL, *prev_node = NULL;

	if ((node == NULL) || (child == NULL))
		return false;

	/* Only directories can have children */
	if (node->Data->elemType != DIRECTORY)
		return false;

	if (node->firstChild == NULL)
	{
		node->firstChild = child;
		return true;
	}

	curr_node = node->firstChild;

	if (compareNodes(child, curr_node) <= 0)
	{
		child->nextSibling = curr_node;
		node->firstChild = child;
		return true;
	}

	prev_node = curr_node;
	curr_node = curr_node->nextSibling;

	while (compareNodes(child, curr_node) > 0)
	{
		prev_node = curr_node;
		curr_node = curr_node->nextSibling;
	}

	child->nextSibling = curr_node;
	prev_node->nextSibling = child;
	return true;
}

bool deleteSubTree(Node* root)
{
	Node *curr_node = NULL, *next_node = NULL;
	
	if (root == NULL)
		return true;

	if (root->firstChild == NULL)
	{
		free(root->Data);
		free(root);
		return true;
	}

	curr_node = root->firstChild;
	
	while (curr_node != NULL)
	{
		next_node = curr_node->nextSibling;
		deleteSubTree(curr_node);
		curr_node = next_node;
	}
	return true;
}



bool removeNode(Node* parent, Node* node_to_remove)
{
	Node *curr_node = NULL, *prev_node = NULL;
	
	if (node_to_remove == NULL)
		return false;

	if ((parent == NULL) && (node_to_remove->isRoot))
	{
		return deleteSubTree(node_to_remove);
	}

	if (parent->firstChild == node_to_remove)
	{
		parent->firstChild = node_to_remove->nextSibling;
		return deleteSubTree(node_to_remove);
	}

	prev_node = parent->firstChild;
	curr_node = prev_node->nextSibling;

	while ((curr_node != NULL) && (curr_node != node_to_remove))
	{
		prev_node = curr_node;
		curr_node = curr_node->nextSibling;
	}
	
	if (curr_node == NULL)
		return false;

	prev_node->nextSibling = curr_node->nextSibling;
	return deleteSubTree(curr_node);
}

	

