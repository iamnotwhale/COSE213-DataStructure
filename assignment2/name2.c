#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_YEAR_DURATION	10	// 기간

// 구조체 선언
typedef struct {
	char	name[20];		// 이름
	char	sex;			// 성별 M or F
	int		freq[MAX_YEAR_DURATION]; // 연도별 빈도
} tName;

typedef struct {
	int		len;		// 배열에 저장된 이름의 수
	int		capacity;	// 배열의 용량 (배열에 저장 가능한 이름의 수)
	tName	*data;		// 이름 배열의 포인터
} tNames;

////////////////////////////////////////////////////////////////////////////////
// 함수 원형 선언

// 입력 파일을 읽어 이름 정보(연도, 이름, 성별, 빈도)를 이름 구조체에 저장
// 이미 구조체에 존재하는(저장된) 이름은 해당 연도의 빈도만 저장
// 새로 등장한 이름은 구조체에 추가
// 주의사항: 동일 이름이 남/여 각각 사용될 수 있으므로, 이름과 성별을 구별해야 함
// 주의사항: 정렬 리스트(ordered list)를 유지해야 함 (qsort 함수 사용하지 않음)
// 이미 등장한 이름인지 검사하기 위해 bsearch 함수를 사용
// 새로운 이름을 저장할 메모리 공간을 확보하기 위해 memmove 함수를 이용하여 메모리에 저장된 내용을 복사
// names->capacity는 1000으로부터 시작하여 1000씩 증가 (1000, 2000, 3000, ...)
// start_year : 시작 연도 (2009)
void load_names( FILE *fp, int start_year, tNames *names);

// 구조체 배열을 화면에 출력
void print_names( tNames *names, int num_year);

// bsearch를 위한 비교 함수
// 정렬 기준 : 이름(1순위), 성별(2순위)
int compare( const void *n1, const void *n2);

// 이진탐색 함수
// return value: key가 발견되는 경우, 배열의 인덱스
//				key가 발견되지 않는 경우, key가 삽입되어야 할 배열의 인덱스
int binary_search( const void *key, const void *base, size_t nmemb, size_t size, int (*compare)(const void *, const void *));

////////////////////////////////////////////////////////////////////////////////
// 함수 정의

// 이름 구조체 초기화
// len를 0으로, capacity를 1로 초기화
// return : 구조체 포인터
tNames *create_names(void)
{
	tNames *pnames = (tNames *)malloc( sizeof(tNames));
	
	pnames->len = 0;
	pnames->capacity = 1000;
	pnames->data = (tName *)malloc(pnames->capacity * sizeof(tName));

	return pnames;
}

// 이름 구조체에 할당된 메모리를 해제
void destroy_names(tNames *pnames)
{
	free(pnames->data);
	pnames->len = 0;
	pnames->capacity = 0;

	free(pnames);
}
	
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	tNames *names;
	FILE *fp;
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n\n", argv[0]);
		return 1;
	}

	// 이름 구조체 초기화
	names = create_names();
	
	fp = fopen( argv[1], "r");
	if (!fp)
	{
		fprintf( stderr, "cannot open file : %s\n", argv[1]);
		return 1;
	}

	fprintf( stderr, "Processing [%s]..\n", argv[1]);
		
	// 연도별 입력 파일(이름 정보)을 구조체에 저장
	load_names( fp, 2009, names);
	
	fclose( fp);
	
	// 이름 구조체를 화면에 출력
	print_names( names, MAX_YEAR_DURATION);

	// 이름 구조체 해제
	destroy_names( names);
	
	return 0;
}

////////////////////////////////////////////////////////
// 입력 파일을 읽어 이름 정보(연도, 이름, 성별, 빈도)를 이름 구조체에 저장
// 이미 구조체에 존재하는(저장된) 이름은 해당 연도의 빈도만 저장
// 새로 등장한 이름은 구조체에 추가
// 주의사항: 동일 이름이 남/여 각각 사용될 수 있으므로, 이름과 성별을 구별해야 함
// 주의사항: 정렬 리스트(ordered list)를 유지해야 함 (qsort 함수 사용하지 않음)
// 이미 등장한 이름인지 검사하기 위해 bsearch 함수를 사용
// 새로운 이름을 저장할 메모리 공간을 확보하기 위해 memmove 함수를 이용하여 메모리에 저장된 내용을 복사
// names->capacity는 1000으로부터 시작하여 1000씩 증가 (1000, 2000, 3000, ...)
// start_year : 시작 연도 (2009)
void load_names( FILE *fp, int start_year, tNames *names) {
	int year, count, index;
    char *name, sex;
    name = (char *) malloc (sizeof(char)*20);

    while (1) {
        fscanf(fp, "%d %s %c %d", &year, name, &sex, &count);
        if (feof(fp)) {
            break;
        }

        tName key;
        strcpy(key.name, name);
        key.sex = sex;

		// bsearch로 검색 확인
        tName *res;
		res = bsearch(&key, names->data, names->len, sizeof(tName), compare);

		// 검색성공시
        if (res != NULL) {
			res->freq[year-start_year] = count;
		}

		// 검색실패시 : binary_search 함수로 끼워넣을 index찾기, memmove 함수로 이동 후 삽입
		else {
			if (names->len == names->capacity) {
				names->capacity += 1000;
				names->data = (tName*) realloc(names->data, (names->capacity) * sizeof(tName));
			}

			if (names->len == 0) {
				names->data[0].sex = sex;
				strcpy(names->data[0].name, name);
				memset(names->data[0].freq, 0, 10*sizeof(int));
				names->data[0].freq[year-start_year] = count;
				(names->len)++;
				continue;
			}

			index = binary_search(&key, names->data, names->len, sizeof(tName), compare);
			
			memmove(&(names->data[index+1]), &(names->data[index]), sizeof(tName)*((names->len) - index));


			names->data[index].sex = sex;
			strcpy(names->data[index].name, name);
			memset(names->data[index].freq, 0, 10*sizeof(int));
			names->data[index].freq[year-start_year] = count;
			(names->len)++;
			
		}
    }

    free(name);
}

// 구조체 배열을 화면에 출력
void print_names( tNames *names, int num_year) {
	for (int i=0; i<names->len; i++) {
		printf("%s\t%c", names->data[i].name, names->data[i].sex );
		for (int j=0; j<num_year; j++) {
			printf("\t%d", names->data[i].freq[j]);
		}
		printf("\n");
	}
}

// bsearch를 위한 비교 함수
// 정렬 기준 : 이름(1순위), 성별(2순위)
int compare( const void *n1, const void *n2) {
    //이름이 같을 때
	if (strcmp(((tName *)n1)->name, ((tName *)n2)->name) == 0) {
        if (((tName *)n1)->sex < ((tName *)n2)->sex) return -1;
        else if (((tName *)n1)->sex == ((tName *)n2)->sex) return 0;
        else return 1;
    }
    //이름이 다를 때
    else {
        if (strcmp (((tName *)n1)->name, ((tName *)n2)->name) > 0) return 1;
        else return -1;
    }
}

// 이진탐색 함수
// return value: key가 발견되는 경우, 배열의 인덱스
//				key가 발견되지 않는 경우, key가 삽입되어야 할 배열의 인덱스

//bsearch는 탐색 실패하면 NULL값을 반환, 우리가 원하는 함수 아님
//탐색을 실패했다면 어느 위치에 새로운 값을 끼워 넣어야 할지를 찾는 함수가 필요함
//탐색->실패시 binary search함수로 다시찾기? 인듯??
int binary_search( const void *key, const void *base, size_t nmemb, size_t size, int (*compare)(const void *, const void *)) {
	int l = 0;
	int r = nmemb-1; //배열의 마지막 인덱스
	int mid, cmp_res;

	//tName *x = (tName *) base;

	while ( l <= r) {
		
		mid = (l+r) / 2;

		cmp_res = compare(key, (tName*)base+mid);
		

		if (cmp_res == 0) return mid;
		else if (cmp_res < 0) {
			r = mid -1;
		}
		else {
			l = mid + 1;
		}
	}

	return l;
}