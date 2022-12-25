#include <stdlib.h> // malloc, atoi, rand
#include <stdio.h>
#include <assert.h>
#include <time.h> // time

#define RANDOM_INPUT	1
#define FILE_INPUT		2

////////////////////////////////////////////////////////////////////////////////
// TREE type definition
typedef struct node
{
	int			data;
	struct node	*left;
	struct node	*right;
} NODE;

typedef struct
{
	NODE	*root;
} TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a tree head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
TREE *BST_Create( void);

/* Deletes all data in tree and recycles memory
*/
void BST_Destroy( TREE *pTree);

/* internal function (not mandatory)
*/
static void _destroy( NODE *root);

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int BST_Insert( TREE *pTree, int data);

/* internal function (not mandatory)
*/
static void _insert( NODE *root, NODE *newPtr);

NODE *_makeNode( int data);

/* Deletes a node with dltKey from the tree
	return	1 success
			0 not found
*/
int BST_Delete( TREE *pTree, int dltKey);

/* internal function
	success is 1 if deleted; 0 if not
	return	pointer to root
*/
static NODE *_delete( NODE *root, int dltKey, int *success);

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
int *BST_Retrieve( TREE *pTree, int key);

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve( NODE *root, int key);

/* prints tree using inorder traversal
*/
void BST_Traverse( TREE *pTree);
static void _traverse( NODE *root);

/* Print tree using inorder right-to-left traversal
*/
void printTree( TREE *pTree);
/* internal traversal function
*/
static void _inorder_print( NODE *root, int level);

/* 
	return 1 if the tree is empty; 0 if not
*/
int BST_Empty( TREE *pTree);

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	int mode; // input mode
	TREE *tree;
	int data;
	
	if (argc != 2)
	{
		fprintf( stderr, "usage: %s FILE or %s number\n", argv[0], argv[0]);
		return 1;
	}
	
	FILE *fp;
	
	if ((fp = fopen(argv[1], "rt")) == NULL)
	{
		mode = RANDOM_INPUT;
	}
	else mode = FILE_INPUT;
	
	// creates a null tree
	tree = BST_Create();
	
	if (!tree)
	{
		printf( "Cannot create a tree!\n");
		return 100;
	}

	if (mode == RANDOM_INPUT)
	{
		int numbers;
		numbers = atoi(argv[1]);
		assert( numbers > 0);

		fprintf( stdout, "Inserting: ");
		
		srand( time(NULL));
		for (int i = 0; i < numbers; i++)
		{
			data = rand() % (numbers*3) + 1; // random number (1 ~ numbers * 3)
			
			fprintf( stdout, "%d ", data);
			
			// insert function call
			int ret = BST_Insert( tree, data);
			if (!ret) break;
		}
	}
	else if (mode == FILE_INPUT)
	{
		fprintf( stdout, "Inserting: ");
		
		while (fscanf( fp, "%d", &data) != EOF)
		{
			fprintf( stdout, "%d ", data);
			
			// insert function call
			int ret = BST_Insert( tree, data);
			if (!ret) break;
		}
		fclose( fp);
	}
	
	fprintf( stdout, "\n");

	if (BST_Empty( tree))
	{
		fprintf( stdout, "Empty tree!\n");
		BST_Destroy( tree);
		return 0;
	}	

	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	BST_Traverse( tree);
	fprintf( stdout, "\n");
	
	// print tree with right-to-left inorder traversal
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
	
	while (1)
	{   
		fprintf( stdout, "Input a number to delete: "); 
		int num;
		if (scanf( "%d", &num) == EOF) break;
		
		int ret = BST_Delete( tree, num);
		if (!ret)
		{
			fprintf( stdout, "%d not found\n", num);
			continue;
		}
		
		// print tree with right-to-left inorder traversal
		fprintf( stdout, "Tree representation:\n");
		printTree(tree);
		
		if (BST_Empty( tree))
		{
			fprintf( stdout, "Empty tree!\n");
			break;
		}
	}
	
	BST_Destroy( tree);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a tree head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
TREE *BST_Create( void) {
    TREE *nTree = (TREE *) malloc (sizeof(TREE));
    nTree->root = NULL;
    return nTree;
}

/* Deletes all data in tree and recycles memory
*/
void BST_Destroy( TREE *pTree) {
    if (pTree->root) {
        _destroy(pTree->root);
    }
    free(pTree);
}

/* internal function (not mandatory)
*/
static void _destroy( NODE *root) {
    if (!root) return;
    else {
        _destroy(root->left);
        _destroy(root->right);
        free(root);
    }
}

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int BST_Insert( TREE *pTree, int data) {
    NODE *newPtr = _makeNode(data);
    if (!newPtr ) return 0;

    if (pTree->root == NULL) {
        pTree->root = newPtr;
        return 1;
    }
    else {
        _insert(pTree->root, newPtr);
        return 1;
    }
}

/* internal function (not mandatory)
*/
static void _insert( NODE *root, NODE *newPtr) {
    if (!root) return;

    if (root->data > newPtr->data) {
        if (root->left == NULL) {
            root->left = newPtr;
            return;
        }
        _insert(root->left, newPtr);
    }
    else {
        if (root->right == NULL) {
            root->right = newPtr;
            return;
        }
        _insert(root->right, newPtr);
    }
}

NODE *_makeNode( int data) {
    NODE *nNode = (NODE *) malloc (sizeof(NODE));
    nNode->data = data;
    nNode->left = NULL;
    nNode->right = NULL;
    return nNode;
}

/* Deletes a node with dltKey from the tree
	return	1 success
			0 not found
*/
int BST_Delete( TREE *pTree, int dltKey) {
    int success;

	if (pTree->root != NULL) {
		if (pTree->root->data == dltKey && pTree->root->left == NULL) {
			if (pTree->root->left == NULL) {
				NODE *tmp = pTree->root->right;
				free(pTree->root);
				pTree->root = tmp;
				success = 1;
			}
			else if (pTree->root->right == NULL) {
				NODE *tmp = pTree->root->left;
				free(pTree->root);
				pTree->root = tmp;
				success = 1;
			}
		}
		else {
			pTree->root = _delete(pTree->root, dltKey, &success);
		}
	}
	else return 0;

	return success;
}

/* internal function
	success is 1 if deleted; 0 if not
	return	pointer to root
*/
static NODE *_delete( NODE *root, int dltKey, int *success) {
    if (!root) {
		*success = 0;
		return NULL;
		//fprintf(stderr, "root is null\n");
	}
	else if (root->data > dltKey) {
		root->left = _delete(root->left, dltKey, success);
		//fprintf(stderr, "root data is bigger than key\n");
	}
	else if (root->data < dltKey) {
		root->right = _delete(root->right, dltKey, success);
		
		//fprintf(stderr, "root data is smaller than key\n");
	}
	else if (root->right == NULL) {
		NODE *tmp = root->left;
		free(root);
		root = tmp;
		*success = 1;
		
		//fprintf(stderr, "root right is null and delete success\n");
	}
	else if (root->left == NULL) {
		NODE *tmp = root->right;
		free(root);
		root = tmp;
		*success = 1;

		//fprintf(stderr, "root left is null and delete success\n");
	}
	else {
		NODE *tmp = root->right;
		while (tmp->left != NULL) {
			tmp = tmp->left;
		}
		root->data = tmp->data;
		//fprintf(stderr, "root and daughter change\n");
		root->right = _delete(root->right, tmp->data, success);
	}
	return root;
}

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
int *BST_Retrieve( TREE *pTree, int key) {
    if (pTree->root) {
		NODE *foundNode = _retrieve(pTree->root, key);
		if (foundNode) return &(foundNode->data);
		else return NULL;
	}
}

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve( NODE *root, int key) {
    if (root) {
		if ((root->data) > key) {
			return _retrieve(root->left, key);
		}
		else if ((root->data) < key) {
			return _retrieve(root->right, key);
		}
		else {
			return root;
		}
	}
	else {
		return NULL;
	}
}

/* prints tree using inorder traversal
*/
void BST_Traverse( TREE *pTree) {
    if (pTree->root) {
        _traverse(pTree->root);
    }
}
static void _traverse( NODE *root) {
    if (root) {
		_traverse(root->left);
		printf("%d ", root->data);
		_traverse(root->right);
	}
}

/* Print tree using inorder right-to-left traversal
*/
void printTree( TREE *pTree) {
    if (pTree->root) {
        _inorder_print(pTree->root, 0);
    }
}
/* internal traversal function
*/
static void _inorder_print( NODE *root, int level) {
    if (!root) return;

    if (root->right) {
        _inorder_print(root->right, level+1);
    }
    for (int i=0; i<level; i++) {
        printf("\t");
    }
    printf("%d\n", root->data);
    if (root->left) {
        _inorder_print(root->left, level+1);
    }
}

/* 
	return 1 if the tree is empty; 0 if not
*/
int BST_Empty( TREE *pTree) {
    if (pTree->root) return 0;
    else return 1;
}