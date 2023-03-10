#define SHOW_STEP 0 // 제출시 0
#define BALANCING 1 // 제출시 1 (used in _insert function)

#include <stdlib.h> // malloc
#include <stdio.h>
#include <string.h> //strcmp, strdup

#define max(x, y)	(((x) > (y)) ? (x) : (y))

////////////////////////////////////////////////////////////////////////////////
// AVL_TREE type definition
typedef struct node
{
	char		*data;
	struct node	*left;
	struct node	*right;
	int			height;
} NODE;

typedef struct
{
	NODE	*root;
	int		count;  // number of nodes
} AVL_TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a AVL_TREE head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
AVL_TREE *AVL_Create( void);

/* Deletes all data in tree and recycles memory
*/
void AVL_Destroy( AVL_TREE *pTree);
static void _destroy( NODE *root);

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int AVL_Insert( AVL_TREE *pTree, char *data);

/* internal function
	This function uses recursion to insert the new data into a leaf node
	return	pointer to new root
*/
static NODE *_insert( NODE *root, NODE *newPtr);

static NODE *_makeNode( char *data);

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
char *AVL_Retrieve( AVL_TREE *pTree, char *key);

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve( NODE *root, char *key);

/* Prints tree using inorder traversal
*/
void AVL_Traverse( AVL_TREE *pTree);
static void _traverse( NODE *root);

/* Prints tree using inorder right-to-left traversal
*/
void printTree( AVL_TREE *pTree);
/* internal traversal function
*/
static void _infix_print( NODE *root, int level);

/* internal function
	return	height of the (sub)tree from the node (root)
*/
static int getHeight( NODE *root);

/* internal function
	Exchanges pointers to rotate the tree to the right
	updates heights of the nodes
	return	new root
*/
static NODE *rotateRight( NODE *root);

/* internal function
	Exchanges pointers to rotate the tree to the left
	updates heights of the nodes
	return	new root
*/
static NODE *rotateLeft( NODE *root);

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	AVL_TREE *tree;
	char str[1024];
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n", argv[0]);
		return 0;
	}
	
	// creates a null tree
	tree = AVL_Create();
	
	if (!tree)
	{
		fprintf( stderr, "Cannot create tree!\n");
		return 100;
	}

	FILE *fp = fopen( argv[1], "rt");
	if (fp == NULL)
	{
		fprintf( stderr, "Cannot open file! [%s]\n", argv[1]);
		return 200;
	}

	while(fscanf( fp, "%s", str) != EOF)
	{

#if SHOW_STEP
		fprintf( stdout, "Insert %s>\n", str);
#endif		
		// insert function call
		AVL_Insert( tree, str);

#if SHOW_STEP
		fprintf( stdout, "Tree representation:\n");
		printTree( tree);
#endif
	}
	
	fclose( fp);
	
#if SHOW_STEP
	fprintf( stdout, "\n");

	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	AVL_Traverse( tree);
	fprintf( stdout, "\n");

	// print tree with right-to-left infix traversal
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
#endif

	fprintf( stdout, "Height of tree: %d\n", tree->root->height);
	fprintf( stdout, "# of nodes: %d\n", tree->count);
	
	// retrieval
	char *key;
	fprintf( stdout, "Query: ");
	while( fscanf( stdin, "%s", str) != EOF)
	{
		key = AVL_Retrieve( tree, str);
		
		if (key) fprintf( stdout, "%s found!\n", key);
		else fprintf( stdout, "%s NOT found!\n", str);
		
		fprintf( stdout, "Query: ");
	}
	
	// destroy tree
	AVL_Destroy( tree);

	return 0;
}


////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a AVL_TREE head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
AVL_TREE *AVL_Create( void) {
	AVL_TREE *newTree = (AVL_TREE *) malloc (sizeof(AVL_TREE));
	newTree->root = NULL;
	newTree->count = 0;

	return newTree;
}

/* Deletes all data in tree and recycles memory
*/
void AVL_Destroy( AVL_TREE *pTree) {
	if (pTree->root) {
        _destroy(pTree->root);
    }
    free(pTree);
}
static void _destroy( NODE *root) {
	if (!root) return;
    else {
        _destroy(root->left);
        _destroy(root->right);
		free(root->data);
        free(root);
    }
}

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int AVL_Insert( AVL_TREE *pTree, char *data) {
	NODE *newPtr = _makeNode(data);
    if (!newPtr ) return 0;

	if (pTree->root == NULL) {
        pTree->root = newPtr;
		pTree->count++;
        return 1;
    }

	pTree->root = _insert(pTree->root, newPtr);
	pTree->count++;
	
	return 1;
}

/* internal function
	This function uses recursion to insert the new data into a leaf node
	return	pointer to new root
*/
static NODE *_insert( NODE *root, NODE *newPtr) {
	if (!root) return newPtr;
	//newPtr data가 root data보다 더 작다 -> left 삽입
	if (strcmp(newPtr->data, root->data) < 0) {
		if (root->left == NULL) {
            root->left = newPtr;
        }
		else {
			root->left = _insert(root->left, newPtr);
		}
		root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
	}
	// right 삽입
	else {
		if (root->right == NULL) {
            root->right = newPtr;
        }
		else {
			root->right = _insert(root->right, newPtr);
			
		}
		root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
	}
	// balancing
	if (BALANCING) {
		int balanceCheck = getHeight(root->left) - getHeight(root->right);

		//left high
		if (balanceCheck > 1) {
			// LL
			if (getHeight(root->left->left) - getHeight(root->left->right) > 0) {
				root = rotateRight(root);
			}
			// LR
			else {
				root->left = rotateLeft(root->left);
				root = rotateRight(root);
			}
		}

		//right high
		else if (balanceCheck < -1) {
			// RR
			if (getHeight(root->right->left) - getHeight(root->right->right) < 0) {
				root = rotateLeft(root);
			}
			// RL
			else {
				root->right = rotateRight(root->right);
				root = rotateLeft(root);
			}
		}
	}

	return root;
}

static NODE *_makeNode( char *data) {
	NODE *newNode = (NODE *) malloc (sizeof(NODE));
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->height = 1;
	newNode->data = strdup(data);
	return newNode;
}

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
char *AVL_Retrieve( AVL_TREE *pTree, char *key) {
	if (pTree->root) {
		NODE *foundNode = _retrieve(pTree->root, key);
		if (foundNode) return foundNode->data;
		else return NULL;
	}
}

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve( NODE *root, char *key) {
	if (root) {
		if (strcmp(root->data,key) > 0) {
			return _retrieve(root->left, key);
		}
		else if (strcmp(root->data, key) <0) {
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

/* Prints tree using inorder traversal
*/
void AVL_Traverse( AVL_TREE *pTree) {
	if (pTree->root) {
        _traverse(pTree->root);
    }
}
static void _traverse( NODE *root) {
	if (root) {
		_traverse(root->left);
		printf("%s ", root->data);
		_traverse(root->right);
	}
}

/* Prints tree using inorder right-to-left traversal
*/
void printTree( AVL_TREE *pTree) {
	if (pTree) {
		_infix_print(pTree->root, 0);
	}
}
/* internal traversal function
*/
static void _infix_print( NODE *root, int level) {
	if (!root) return;

    if (root->right) {
        _infix_print(root->right, level+1);
    }
    for (int i=0; i<level; i++) {
        printf("\t");
    }
    printf("%s\n", root->data);
    if (root->left) {
        _infix_print(root->left, level+1);
    }
}

/* internal function
	return	height of the (sub)tree from the node (root)
*/
static int getHeight( NODE *root) {
	return (!root) ? 0 : root->height;
}

/* internal function
	Exchanges pointers to rotate the tree to the right
	updates heights of the nodes
	return	new root
*/
static NODE *rotateRight( NODE *root) {
	NODE *temp = root->left->right;
	NODE *leftNode = root->left;

	root->left->right = root;
	root->left = temp;

	root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
	leftNode->height = max(getHeight(leftNode->left), getHeight(leftNode->right)) + 1;

	return leftNode;
}

/* internal function
	Exchanges pointers to rotate the tree to the left
	updates heights of the nodes
	return	new root
*/
static NODE *rotateLeft( NODE *root) {
	NODE *temp = root->right->left;
	NODE *rightNode = root->right;

	root->right->left = root;
	root->right = temp;

	root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
	rightNode->height = max(getHeight(rightNode->left), getHeight(rightNode->right)) + 1;

	return rightNode;
}

/*
case 1: left of left

case 2: right of right

case 3: right of left

case 4: left of right
*/