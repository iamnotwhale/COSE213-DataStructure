#include <stdio.h>
#include <stdlib.h>	// malloc
#include <string.h>	// strdup
#include <ctype.h>	// isupper, tolower

#define MAX_DEGREE	27 // 'a' ~ 'z' and EOW
#define EOW			'$' // end of word

// used in the following functions: trieInsert, trieSearch, triePrefixList
#define getIndex(x)		(((x) == EOW) ? MAX_DEGREE-1 : ((x) - 'a'))

// TRIE type definition
typedef struct trieNode {
	int 			index; // -1 (non-word), 0, 1, 2, ...
	struct trieNode	*subtrees[MAX_DEGREE];
} TRIE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a trie node and returns its address to caller
	return	node pointer
			NULL if overflow
*/
TRIE *trieCreateNode(void);

/* Deletes all data in trie and recycles memory
*/
void trieDestroy( TRIE *root);

/* Inserts new entry into the trie
	return	1 success
			0 failure
*/
// 주의! 엔트리를 중복 삽입하지 않도록 체크해야 함
// 대소문자를 소문자로 통일하여 삽입
// 영문자와 EOW 외 문자를 포함하는 문자열은 삽입하지 않음
int trieInsert( TRIE *root, char *str, int dic_index);

/* Retrieve trie for the requested key
	return	index in dictionary (trie) if key found
			-1 key not found
*/
int trieSearch( TRIE *root, char *str);

/* prints all entries in trie using preorder traversal
*/
void trieList( TRIE *root, char *dic[]);

/* prints all entries starting with str (as prefix) in trie
	ex) "abb" -> "abbas", "abbasid", "abbess", ...
	this function uses trieList function
*/
void triePrefixList( TRIE *root, char *str, char *dic[]);

/* makes permuterms for given str
	ex) "abc" -> "abc$", "bc$a", "c$ab", "$abc"
	return	number of permuterms
*/
int make_permuterms( char *str, char *permuterms[]);

/* recycles memory for permuterms
*/
void clear_permuterms( char *permuterms[], int size);

/* wildcard search
	ex) "ab*", "*ab", "a*b", "*ab*"
	this function uses triePrefixList function
*/
void trieSearchWildcard( TRIE *root, char *str, char *dic[]);

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	TRIE *permute_trie;
	char *dic[100000];

	int ret;
	char str[100];
	FILE *fp;
	char *permuterms[100];
	int num_p; // # of permuterms
	int word_index = 0;
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n", argv[0]);
		return 1;
	}
	
	fp = fopen( argv[1], "rt");
	if (fp == NULL)
	{
		fprintf( stderr, "File open error: %s\n", argv[1]);
		return 1;
	}
	
	permute_trie = trieCreateNode(); // trie for permuterm index
	
	while (fscanf( fp, "%s", str) != EOF)
	{	
		num_p = make_permuterms( str, permuterms);
		
		for (int i = 0; i < num_p; i++)
			trieInsert( permute_trie, permuterms[i], word_index);
		
		clear_permuterms( permuterms, num_p);
		
		dic[word_index++] = strdup( str);
	}

	
	fclose( fp);
	
	printf( "\nQuery: ");
	while (fscanf( stdin, "%s", str) != EOF)
	{
		// wildcard search term
		if (strchr( str, '*')) 
		{
			trieSearchWildcard( permute_trie, str, dic);
		}
		// keyword search
		else 
		{
			ret = trieSearch( permute_trie, str);
			
			if (ret == -1) printf( "[%s] not found!\n", str);
			else printf( "[%s] found!\n", dic[ret]);
		}
		printf( "\nQuery: ");
	}

	for (int i = 0; i < word_index; i++)
		free( dic[i]);
	
	trieDestroy( permute_trie);
	
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a trie node and returns its address to caller
   return   node pointer
         NULL if overflow
*/
TRIE *trieCreateNode(void) {
   TRIE *newTRIE = (TRIE *) malloc (sizeof(TRIE));
    if (!newTRIE) return NULL;

    newTRIE->index = -1;
    for (int i=0; i<MAX_DEGREE; i++) {
       newTRIE->subtrees[i] = NULL;
    }
    return newTRIE;
}

/* Deletes all data in trie and recycles memory
*/
void trieDestroy( TRIE *root) {
   for (int i=0; i<MAX_DEGREE; i++) {
        if (root->subtrees[i]) {
          trieDestroy(root->subtrees[i]);
        }
    }
    free(root);
}

/* Inserts new entry into the trie
   return   1 success
         0 failure
*/
// 주의! 엔트리를 중복 삽입하지 않도록 체크해야 함
// 대소문자를 소문자로 통일하여 삽입
// 영문자와 EOW 외 문자를 포함하는 문자열은 삽입하지 않음
int trieInsert( TRIE *root, char *str, int dic_index) {
   
   int len = strlen(str);
   TRIE *cur = root;

   // 대소문자를 소문자로 통일하는 과정
   for (int i=0; i<len; i++) {
      if (isupper(str[i])) {
         str[i] = tolower(str[i]);
      }
      // 영문자, EOW 이외 문자가 포함된 문자열은 삽입하지 않음
      if (getIndex(str[i]) < 0 || getIndex(str[i]) > 26) {
         return 0;
      }
   }

   // 삽입과정
   for (int i=0; i<len; i++) {
      if (!cur->subtrees[getIndex(str[i])]) {
         cur->subtrees[getIndex(str[i])] = trieCreateNode();
      }
      cur = cur->subtrees[getIndex(str[i])];
   }

   // 중복 체크
   if (cur->index == -1) {
      cur->index = dic_index;
      return 1;
   }
   else return 0;
}

/* Retrieve trie for the requested key
   return   index in dictionary (trie) if key found
         -1 key not found
*/
int trieSearch( TRIE *root, char *str) {
   int len = strlen(str);
	char *copy = (char *) malloc (sizeof(char)*(len+2));
   for (int i=0; i<len; i++) {
      copy[i] = str[i];
   }
	copy[len++] = '$';
	copy[len] = '\0';

   // 검색과정
   TRIE *now = root;
   for (int i=0; i<len; i++) {
      if (!(now->subtrees[getIndex(copy[i])])) {
         free(copy);
         return -1;
      }
      else now = now->subtrees[getIndex(copy[i])];
   }
   free(copy);
   return now->index;
}

/* prints all entries in trie using preorder traversal
*/
void trieList( TRIE *root, char *dic[]) {
   TRIE *now = root;

   if (root->index != -1) {
      printf("%s\n", dic[root->index]);
   }

   for (int i=0; i<MAX_DEGREE; i++) {
      if (now->subtrees[i]) {
           trieList(now->subtrees[i], dic);
      }
   }

}

/* prints all entries starting with str (as prefix) in trie
   ex) "abb" -> "abbas", "abbasid", "abbess", ...
   this function uses trieList function
*/
void triePrefixList( TRIE *root, char *str, char *dic[]) {
   TRIE *now = root;
   int len = strlen(str);

	for (int i=0; i<len; i++) {
		now = now->subtrees[getIndex(str[i])];
      if (!now) return;
	}
	if (!now) return;

	trieList(now, dic);
}

/* makes permuterms for given str
   ex) "abc" -> "abc$", "bc$a", "c$ab", "$abc"
   return   number of permuterms
*/
int make_permuterms( char *str, char *permuterms[]) {
   int len = strlen(str);
   char *pStr = (char *) malloc (sizeof(char) * (len+2));
   // abc$ 만들기
   for (int i=0; i<len; i++) {
      pStr[i] = str[i];
   }
   pStr[len] = '$';
   pStr[len+1] = '\0';
   permuterms[0] = strdup(pStr);
   
   for (int i=1; i<=len; i++) {
      char transfer = pStr[0];
      for (int j=1; j<=len; j++) {
         pStr[j-1] = pStr[j];
      }
      pStr[len] = transfer;
      permuterms[i] = strdup(pStr);
   }
   
   free(pStr);
   return len+1;
}

/* recycles memory for permuterms
*/
void clear_permuterms( char *permuterms[], int size) {
   for (int i=0; i<size; i++) {
        free(permuterms[i]);
   }
}

/* wildcard search
   ex) "ab*", "*ab", "a*b", "*ab*"
   this function uses triePrefixList function
*/
void trieSearchWildcard( TRIE *root, char *str, char *dic[]) {
   int len = strlen(str);
   int starcount = 0;
   for (int i=0; i<len; i++) {
      if (str[i] == '*') starcount++;
   }

   char *searchptr;
   
   // *ab*$ -> ab* 찾기
   if (starcount == 2) {
      char copy[100];
      for (int i=1; i<len-1; i++) {
         copy[i-1] = str[i];
      }
      copy[len-2] = '\0';
      triePrefixList(root, copy, dic);
      return;
   }
   
   // 나머지: ab*$ -> $ab*, *ab$ -> ab$*, a*b$ -> b$a*
   char *permuterms[100];
	int num_p = make_permuterms(str, permuterms);

	for (int i=0; i<num_p; i++) {
		searchptr = permuterms[i];
      // *이 마지막으로 넘어왔을 때 -> *지우기/trie출력에 넣음
		if (searchptr[len] == '*') {
			searchptr[len] = '\0';
			break;
		}
	}

	triePrefixList(root, searchptr, dic);
   clear_permuterms(permuterms, num_p);
   return;
}