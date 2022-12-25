#include <stdlib.h> // malloc

#include "adt_dlist.h"

// internal insert function
// inserts data into list
// return	1 if successful
// 			0 if memory overflow
static int _insert( LIST *pList, NODE *pPre, void *dataInPtr) {
    NODE *pNew = (NODE *) malloc (sizeof(NODE));
    if (!pNew) return 0;

    pNew->dataPtr = dataInPtr;
	pNew->llink = NULL;
	pNew->rlink = NULL;

    // 처음에 삽입
    if (pPre == NULL && pList->count != 0) {
        pNew->rlink = pList->head;
		pList->head = pNew;
    }
	// 빈 list 삽입
    else if (pList->count == 0) {
		pList->head = pNew;
        pList->rear = pNew;
    }
	// 맨 뒤에 삽입
	else if (pPre->rlink == NULL) {
		pNew->rlink = pPre->rlink;
        pPre->rlink = pNew;
        pNew->llink = pPre;
		pList->rear = pNew;
	}
    // 중간 or 끝에 삽입
    else {
		pPre->rlink->llink = pNew;
        pNew->rlink = pPre->rlink;
        pPre->rlink = pNew;
        pNew->llink = pPre;
		
    }

    (pList->count)++;
    return 1;
}

// internal delete function
// deletes data from list and saves the (deleted) data to dataOutPtr
static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, void **dataOutPtr) {
    
    *dataOutPtr = pLoc->dataPtr;
	// 아무것도 없어서 삭제가 불가능
	if (pList->count == 0) return;
	// 첫 번째 노드 삭제
	else if (pPre == NULL) {
		pList->head = pLoc->rlink;
		pList->head->llink = NULL;
		(pList->count)--;
		free(pLoc);
		return;
	}
	// 마지막 노드 삭제
	else if (pLoc->rlink == NULL) {
		pPre->rlink = pLoc->rlink;
		pList->rear = pPre;
		(pList->count)--;
		free(pLoc);
		return;
	}
	else {
		pLoc->rlink->llink = pPre;
		pPre->rlink = pLoc->rlink;
		(pList->count)--;
		free(pLoc);
		return;
	}
}

// internal search function
// searches list and passes back address of node containing target and its logical predecessor
// return	1 found
// 			0 not found
static int _search( LIST *pList, NODE **pPre, NODE **pLoc, void *pArgu) {
    *pPre = NULL;
	*pLoc = pList->head;

	if (pList->count == 0) return 0;

	while (*pLoc != NULL && pList->compare((*pLoc)->dataPtr, pArgu) < 0) {
		*pPre = *pLoc;
		*pLoc = (*pLoc)->rlink;
	}

	if (*pLoc == NULL) return 0;
	else {
		if (pList->compare((*pLoc)->dataPtr, pArgu) == 0) {
			return 1;
		}
		else {
			return 0;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// function declarations

// Allocates dynamic memory for a list head node and returns its address to caller
// return	head node pointer
// 			NULL if overflow
LIST *createList( int (*compare)(const void *, const void *)) {
    LIST *nList = (LIST *) malloc (sizeof(LIST));
	nList->head = NULL;
    nList->rear = NULL;
    nList->compare = compare;
	nList->count = 0;
    return nList;
}

//  이름 리스트에 할당된 메모리를 해제 (head node, data node, name data)
void destroyList( LIST *pList, void (*callback)(void *)) {
    NODE *cur;
	if (pList ) {
		while (pList->count > 0) {
			cur = pList->head;
			callback(cur->dataPtr);
			pList->head = cur->rlink;
			free(cur);
			(pList->count)--;
		}
		free (pList);
	}
}

// Inserts data into list
//	return	0 if overflow
//			1 if successful
//			2 if duplicated key
int addNode( LIST *pList, void *dataInPtr, void (*callback)(const void *, const void *)) {
    // dataOutPtr 어떻게함
    NODE *pPre, *pLoc;
    void *dataOutPtr;

    int searchRes = _search(pList, &pPre, &pLoc, dataInPtr);
    if (!searchRes) {
        int insertRes = _insert(pList, pPre, dataInPtr);
		//fprintf(stderr, "%d %d", searchRes, insertRes);
        if (!insertRes) {
			return 0;
		}
        else return 1;
    }
    // duplicated
    else {
        int searchNodeRes = searchList(pList, dataInPtr, &dataOutPtr);
        callback(dataOutPtr, dataInPtr);
        return 2;
    }
}

// Removes data from list
//	return	0 not found
//			1 deleted
int removeNode( LIST *pList, void *keyPtr, void **dataOutPtr) {
    NODE *pPre, *pLoc;

	int found = searchList(pList, keyPtr, dataOutPtr);

	if (found) {
		int foundRes = _search(pList, &pPre, &pLoc, keyPtr);
		_delete(pList, pPre, pLoc, dataOutPtr);
	}

	return found;
}

// interface to search function
//	pArgu	key being sought
//	dataOutPtr	contains found data
//	return	1 successful
//			0 not found
int searchList( LIST *pList, void *pArgu, void **dataOutPtr) {
    NODE *pPre, *pLoc;
    int searchRes = _search(pList, &pPre, &pLoc, pArgu);

    if (!searchRes) {
        *dataOutPtr = NULL;
        return 0;
    }
    else {
        *dataOutPtr = pLoc->dataPtr;
        return 1;
    }
}

// returns number of nodes in list
int countList( LIST *pList) {
    return pList->count;
}

// returns	1 empty
//			0 list has data
int emptyList( LIST *pList) {
    return (pList->count == 0) ? 1 : 0;
}

// traverses data from list (forward)
void traverseList( LIST *pList, void (*callback)(const void *)) {
    int cnt = pList->count;
    if (pList->count == 0) return;

    NODE *cur = pList->head;
    while (cnt > 0) {
        callback(cur->dataPtr);
        cur = cur->rlink;
        cnt--;
    }
}

// traverses data from list (backward)
void traverseListR( LIST *pList, void (*callback)(const void *)) {
    int cnt = pList->count;
    if (pList->count == 0) return;

    NODE *cur = pList->rear;
    while (cnt > 0) {
        callback(cur->dataPtr);
        cur = cur->llink;
        cnt--;
    }
}
