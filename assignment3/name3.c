#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_YEAR_DURATION	10	// 기간

// 이름 구조체 선언
typedef struct {
	char	name[20];				// 이름
	char	sex;					// 성별 M or F
	int		freq[MAX_YEAR_DURATION]; // 연도별 빈도
} tName;

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	tName		*dataPtr;
	struct node	*link;
} NODE;

typedef struct
{
	int		count;
	NODE	*head;
} LIST;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

// Allocates dynamic memory for a list head node and returns its address to caller
// return	head node pointer
// 			NULL if overflow
LIST *createList(void);

//  이름 리스트에 할당된 메모리를 해제 (head node, data node, name data)
void destroyList( LIST *pList);

// internal insert function
// inserts data into a new node
// return	1 if successful
// 			0 if memory overflow
static int _insert( LIST *pList, NODE *pPre, tName *dataInPtr);

// internal search function
// searches list and passes back address of node containing target and its logical predecessor
// return	1 found
// 			0 not found
static int _search( LIST *pList, NODE **pPre, NODE **pLoc, tName *pArgu);

// 이름 구조체를 위한 메모리를 할당하고, 이름(name)과 성별(sex)을 초기화
// return	할당된 이름 구조체에 대한 pointer
//			NULL if overflow
tName *createName( char *name, char sex); 

//  이름 구조체에 할당된 메모리를 해제
void destroyName( tName *pNode);

////////////////////////////////////////////////////////////////////////////////
// 입력 파일을 읽어 이름 정보(연도, 이름, 성별, 빈도)를 이름 리스트에 저장
// 이미 리스트에 존재하는(저장된) 이름은 해당 연도의 빈도만 저장
// 새로 등장한 이름은 리스트에 추가
// 주의사항: 동일 이름이 남/여 각각 사용될 수 있으므로, 이름과 성별을 구별해야 함
// 주의사항: 정렬 리스트(ordered list)를 유지해야 함
// start_year : 시작 연도 (2009)
void load_names( FILE *fp, int start_year, LIST *list);

// 이름 리스트를 화면에 출력
void print_names( LIST *pList, int num_year);

////////////////////////////////////////////////////////////////////////////////
// compares two names in name structures
// for _search function
static int cmpName( const tName *pName1, const tName *pName2)
{
	int ret = strcmp( pName1->name, pName2->name);
	if (ret == 0) return pName1->sex - pName2->sex;
	else return ret;
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	LIST *list;
	FILE *fp;
	
	if (argc != 2){
		fprintf( stderr, "usage: %s FILE\n\n", argv[0]);
		return 1;
	}
	
	fp = fopen( argv[1], "rt");
	if (!fp)
	{
		fprintf( stderr, "Error: cannot open file [%s]\n", argv[1]);
		return 2;
	}
	
	// creates an empty list
	list = createList();
	if (!list)
	{
		printf( "Cannot create list\n");
		return 100;
	}
	
	// 입력 파일로부터 이름 정보를 리스트에 저장
	load_names( fp, 2009, list);
	
	fclose( fp);
	
	// 이름 리스트를 화면에 출력
	print_names( list, MAX_YEAR_DURATION);
	
	// 이름 리스트 메모리 해제
	destroyList( list);
	
	return 0;
}


////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

// Allocates dynamic memory for a list head node and returns its address to caller
// return	head node pointer
// 			NULL if overflow
LIST *createList(void) {
	LIST *newList = (LIST *) malloc (sizeof(LIST));
	if (newList == NULL) return NULL;
	newList->head = NULL;
	newList->count = 0;
	return newList;
}

//  이름 리스트에 할당된 메모리를 해제 (head node, data node, name data)
void destroyList( LIST *pList) {
	NODE* cur;
	if (pList) {
		while (pList->count > 0) {
			cur = pList->head;	
			destroyName(cur->dataPtr);
			pList->head = pList->head->link;
			(pList->count)--;
			free(cur);
		}
		free (pList);
	}
}

// internal insert function
// inserts data into a new node
// return	1 if successful
// 			0 if memory overflow
static int _insert( LIST *pList, NODE *pPre, tName *dataInPtr) {
	NODE *pNew = (NODE *) malloc (sizeof(NODE));
	if (pNew == NULL) return 0;

	pNew->dataPtr = dataInPtr;
	pNew->link = NULL;

	if (pPre == NULL) {
		pNew->link = pList->head;
		pList->head = pNew;
	}
	else {
		pNew->link = pPre->link;
		pPre->link = pNew;
	}
	(pList->count)++;
	return 1;
}

// internal search function
// searches list and passes back address of node containing target and its logical predecessor
// return	1 found
// 			0 not found
static int _search( LIST *pList, NODE **pPre, NODE **pLoc, tName *pArgu) {
	*pPre = NULL;
	*pLoc = pList->head;

	if (pList->count == 0) return 0;

	while (*pLoc != NULL && cmpName((*pLoc)->dataPtr, pArgu) < 0) {
		*pPre = *pLoc;
		*pLoc = (*pLoc)->link;
	}

	if (*pLoc == NULL) return 0;
	else {
		if (cmpName((*pLoc)->dataPtr, pArgu) == 0) {
			return 1;
		}
		else {
			return 0;
		}
	}
}

// 이름 구조체를 위한 메모리를 할당하고, 이름(name)과 성별(sex)을 초기화
// return	할당된 이름 구조체에 대한 pointer
//			NULL if overflow
tName *createName( char *name, char sex) {
	tName *newName = (tName*) malloc (sizeof(tName));
	strcpy(newName->name, name);
	newName->sex = sex;
	memset(newName->freq, 0, 10*sizeof(int));

	return newName;
}

//  이름 구조체에 할당된 메모리를 해제
void destroyName( tName *pNode) {
	
	free(pNode);
}

////////////////////////////////////////////////////////////////////////////////
// 입력 파일을 읽어 이름 정보(연도, 이름, 성별, 빈도)를 이름 리스트에 저장
// 이미 리스트에 존재하는(저장된) 이름은 해당 연도의 빈도만 저장
// 새로 등장한 이름은 리스트에 추가
// 주의사항: 동일 이름이 남/여 각각 사용될 수 있으므로, 이름과 성별을 구별해야 함
// 주의사항: 정렬 리스트(ordered list)를 유지해야 함
// start_year : 시작 연도 (2009)
void load_names( FILE *fp, int start_year, LIST *list) {
	char name[20];
	char sex;
	int year, count;
	NODE *pPre, *pLoc;

	while (1) {
		fscanf(fp, "%d %s %c %d", &year, name, &sex, &count);
		if (feof(fp)) {
			break;
		}

		//입력받은 정보로 현재 tName 구조체 생성
		tName *curName = createName(name, sex);
		curName->freq[year-start_year] = count;

		//첫번째는 그냥 추가
		if (list->count == 0) {
			pPre = NULL;
			_insert(list, pPre, curName);
			continue;
		}

		int res = _search(list, &pPre, &pLoc, curName);
		//printf("%d\n", count);
		if (res == 0) {
			_insert(list, pPre, curName);
		}
		else {
			pLoc->dataPtr->freq[year-start_year] = count;
			free(curName);
		}
	}
}

// 이름 리스트를 화면에 출력
void print_names( LIST *pList, int num_year) {
	NODE *cur = pList->head;
	for (int i=0; i<pList->count; i++) {
		printf("%s\t%c", cur->dataPtr->name, cur->dataPtr->sex);
		for (int j=0; j<num_year; j++) {
			printf("\t%d", cur->dataPtr->freq[j]);
		}
		printf("\n");
		cur = cur->link;
	}
}