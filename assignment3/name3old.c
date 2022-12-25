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

//////////////////////////////////////////////////////////////////////////////////

// Allocates dynamic memory for a list head node and returns its address to caller
// return	head node pointer
// 			NULL if overflow
LIST *createList(void) {
	
	LIST *list_struct = (LIST*) malloc (sizeof(LIST));
	list_struct->count = 0;
	list_struct->head = NULL;
	//printf("dfsfdf");
	
	return list_struct;
}

//  이름 리스트에 할당된 메모리를 해제 (head node, data node, name data)
void destroyList( LIST *pList) {
	NODE *cur = pList->head;
	NODE *link;

	while (link != NULL) {
		link = cur->link;
		destroyName(link->dataPtr);
		free(cur);
		cur = link;
	}

	free(pList);
}

// internal insert function
// inserts data into a new node
// return	1 if successful
// 			0 if memory overflow
static int _insert( LIST *pList, NODE *pPre, tName *dataInPtr) {
	NODE *pNew = (NODE *) malloc (sizeof(NODE));
	//overflow
	if (pNew == NULL) return 0;
	pNew->dataPtr = dataInPtr;

	//빈 list의 경우
	if (pPre == NULL) {
		pList->head->link = pNew;
	}

	//비어있지 않은 경우
	else {
		pNew->link = pPre->link;
		pPre->link = pNew;
	}
	return 1;
}

// internal search function
// searches list and passes back address of node containing target and its logical predecessor
// return	1 found
// 			0 not found
static int _search( LIST *pList, NODE **pPre, NODE **pLoc, tName *pArgu) {
	*pPre = NULL;	
	*pLoc = pList->head;

	int foundRes;

	//while 조건에 pLoc NULL이 아닌동안 + 타겟이 pLoc의 key보다 큰 동안 포함되어야함
	while (pLoc != NULL) {
		*pPre = *pLoc;
		*pLoc = (*pLoc)->link;
	}

	if (pLoc == NULL) {
		foundRes = 0;
	}
	else {
		if (cmpName(pArgu, (*pLoc)->dataPtr)) {
			foundRes = 1;
		}
		else {
			foundRes = 0;
		}
	}
	return foundRes;
}

// 이름 구조체를 위한 메모리를 할당하고, 이름(name)과 성별(sex)을 초기화
// return	할당된 이름 구조체에 대한 pointer
//			NULL if overflow
tName *createName( char *name, char sex) {
	tName *name_struct = (tName *) malloc (sizeof(tName));
	name_struct->sex = sex;
	strcpy(name_struct->name, name);
	memset(name_struct->freq, 0, sizeof(int)*10);
	
	//overflow 아닐때
	return name_struct;
}

//  이름 구조체에 할당된 메모리를 해제
void destroyName( tName *pNode) {
	//pNode->sex = NULL;
	//strcpy(pNode->name, NULL);
	//memset(pNode->freq, 0, sizeof(int)*10);

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
	int year, count;
    char *name, sex;
    name = (char *) malloc (sizeof(char)*20);

	NODE **pPre, **pLoc;

	printf("dfsfdf");

	while (1) {
		//정보 읽기
		fscanf(fp, "%d %s %c %d", &year, name, &sex, &count);
        if (feof(fp)) {
            break;
        }
		//내용을 포함하는 tName구조체 만들기
		tName *currentName = createName(name, sex);

		//검색결과
		int searchRes = _search(list, pPre, pLoc, currentName);

		//리스트에 저장
		//기존에 있는 이름 -> count만 저장
		if (searchRes == 1) {
			(*pLoc)->dataPtr->freq[year] = count;
		}
		else {
			_insert(list, *pPre, currentName);
		}

	}
}

// 이름 리스트를 화면에 출력
void print_names( LIST *pList, int num_year) {
	for (int i=0; i<pList->count; i++) {
		printf("%s\t%c", pList->head->dataPtr->name, pList->head->dataPtr->sex);
		for (int j=0; j<num_year; j++) {
			printf("\t%d", pList->head->dataPtr->freq[j]);
		}
		printf("\n");
	}
}
