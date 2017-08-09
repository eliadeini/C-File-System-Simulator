#include <stdio.h>
#include <stdlib.h>
#include "Node.h"

#define MAX_FILE_NAME 256

typedef struct file_system_tree
{
	Node* Root;
} FSTree;

FSTree* createNewTree()
{
	FSTree *new_tree = NULL;
	FSElement *new_fs_elem = createFSElement("Root", DIRECTORY);
	Node* root = NULL;

	if (new_fs_elem == NULL)
		return NULL;

	root = createNode(new_fs_elem, true);
	if (root == NULL)
	{
		free(new_fs_elem);
		return NULL;
	}

	new_tree = (FSTree*)malloc(sizeof(FSTree));
	if (new_tree == NULL)
	{
		free(root);
		free(new_fs_elem);
		return NULL;
	}
	else
	{
		new_tree->Root = root;
	}
	return new_tree;
}

/* Path shouldn't include "/Root/" */
Node* findPath(FSTree* tree, char* path)
{
	char curr_dir[MAX_FILE_NAME] = {'\0'};
	Node *curr_dir_node = NULL;
	Node *temp_node = NULL;
	int path_reader = 0, curr_dir_ind = 0;
	
	if ((tree == NULL) || (path == NULL))
		return NULL;

	curr_dir_node = tree->Root;
	while (path[path_reader] != '\0')
	{
		if (path[path_reader] == '/')
			path_reader++;

		curr_dir_ind = 0;
		while ((path[path_reader] != '/') && (path[path_reader] != '\0'))
		{
			curr_dir[curr_dir_ind] = path[path_reader];
			curr_dir_ind++;
			path_reader++;
		}

		if (path[path_reader] != '\0')
		{
			curr_dir[curr_dir_ind] = '\0';

			curr_dir_node = curr_dir_node->firstChild;
			temp_node = createNode(createFSElement(curr_dir, DIRECTORY), false);
			while ((curr_dir_node != NULL) && (!compareNodes(curr_dir_node, temp_node)))
				curr_dir_node = curr_dir_node->nextSibling;

			if (curr_dir_node == NULL)
				return NULL;
		}
	}
	return curr_dir_node;
}

Node* addElement(FSTree *tree, char* path, FSElement* elem)
{
	Node *add_to_node = findPath(tree, path), *new_node = NULL;
	
	if ((add_to_node == NULL) || (elem == NULL))
		return NULL;

	new_node = createNode(elem, false);
	if (new_node == NULL)
		return NULL;

	addChild(add_to_node, new_node);
	return new_node;
}

void printIndent(unsigned int indent)
{
	for (unsigned int i = 0; i<indent; i++)
		printf("	");
}

void printDir(Node* node, unsigned int indent)
{
	Node *curr_node = NULL;

	if (node == NULL)
		return;

	if (node->Data->elemType == A_FILE)
	{
		printIndent(indent);
		printf("%s		Type: %s\n", node->Data->elemName, "File");
		return;
	}

	if (node->Data->elemType == DIRECTORY)
	{
		printIndent(indent);
		printf("Type: %s		%s\n", "Directory", node->Data->elemName);
		
		curr_node = node->firstChild;
		while (curr_node != NULL)
		{
			printDir(curr_node, indent+1);
			curr_node = curr_node->nextSibling;
		}
	}
}

void printTree(FSTree* tree)
{
	printDir(tree->Root, 0);
	printf("\n\n");
}

/* Main 
int _tmain()
{
	getchar();
	return 0;
}*/









