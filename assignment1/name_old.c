#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_YEAR_DURATION	10	// 기간
#define LINEAR_SEARCH 0
#define BINARY_SEARCH 1

// 구조체 선언
typedef struct {
	char	name[20];		// 이름
	char	sex;			// 성별 'M' or 'F'
	int		freq[MAX_YEAR_DURATION]; // 연도별 빈도
} tName;

typedef struct {
	int		len;		// 배열에 저장된 이름의 수
	int		capacity;	// 배열의 용량 (배열에 저장 가능한 이름의 수)
	tName	*data;		// 이름 배열의 포인터
} tNames;

////////////////////////////////////////////////////////////////////////////////
// 함수 원형 선언(declaration)

// 연도별 입력 파일을 읽어 이름 정보(이름, 성별, 빈도)를 이름 구조체에 저장
// 이미 구조체에 존재하는(저장된) 이름은 해당 연도의 빈도만 저장
// 새로 등장한 이름은 구조체에 추가
// 주의사항: 동일 이름이 남/여 각각 사용될 수 있으므로, 이름과 성별을 구별해야 함
// names->capacity는 1000으로부터 시작하여 1000씩 증가 (1000, 2000, 3000, ...)
// start_year : 시작 연도 (2009)
// 선형탐색(linear search) 버전
void load_names_lsearch( FILE *fp, int start_year, tNames *names);

// 이진탐색(binary search) 버전
// bsearch 함수 이용; qsort 함수를 이용하여 이름 구조체의 정렬을 유지해야 함
void load_names_bsearch( FILE *fp, int start_year, tNames *names);

// 구조체 배열을 화면에 출력
void print_names( tNames *names, int num_year);

// qsort, bsearch를 위한 비교 함수
// 정렬 기준 : 이름(1순위), 성별(2순위)
int compare( const void *n1, const void *n2);

////////////////////////////////////////////////////////////////////////////////
// 함수 정의 (definition)

// 이름 구조체를 초기화
// len를 0으로, capacity를 1000으로 초기화
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
	int option;
	FILE *fp;
	
	if (argc != 3)
	{
		fprintf( stderr, "Usage: %s option FILE\n\n", argv[0]);
		fprintf( stderr, "option\n\t-l\n\t\twith linear search\n\t-b\n\t\twith binary search\n");
		return 1;
	}
	
	if (strcmp( argv[1], "-l") == 0) option = LINEAR_SEARCH;
	else if (strcmp( argv[1], "-b") == 0) option = BINARY_SEARCH;
	else {
		fprintf( stderr, "unknown option : %s\n", argv[1]);
		return 1;
	}
	
	// 이름 구조체 초기화
	names = create_names();

	if ((fp = fopen( argv[2], "r")) == NULL) 
	{
		fprintf( stderr, "cannot open file : %s\n", argv[2]);
		return 1;
	}

	if (option == LINEAR_SEARCH)
	{
		// 연도별 입력 파일(이름 정보)을 구조체에 저장
		// 선형탐색 모드
		//load_names_lsearch( fp, 2009, names);
	}
	else // (option == BINARY_SEARCH)
	{
		// 이진탐색 모드
		//load_names_bsearch( fp, 2009, names);
	}

	// 정렬 (이름순 (이름이 같은 경우 성별순))
	//qsort( names->data, names->len, sizeof(tName), compare);

	fclose( fp);
		
	// 이름 구조체를 화면에 출력
	//print_names( names, MAX_YEAR_DURATION);

	// 이름 구조체 해제
	destroy_names( names);
	
	return 0;
}

// 연도별 입력 파일을 읽어 이름 정보(이름, 성별, 빈도)를 이름 구조체에 저장
// 이미 구조체에 존재하는(저장된) 이름은 해당 연도의 빈도만 저장
// 새로 등장한 이름은 구조체에 추가
// 주의사항: 동일 이름이 남/여 각각 사용될 수 있으므로, 이름과 성별을 구별해야 함
// names->capacity는 1000으로부터 시작하여 1000씩 증가 (1000, 2000, 3000, ...)
// start_year : 시작 연도 (2009)
// 선형탐색(linear search) 버전 -> 처음부터 끝까지 무지성으로 돌리기
void load_names_lsearch( FILE *fp, int start_year, tNames *names) {

	//파일포인터 열려있는상태 feof, fscanf, fgets........뭐 이런거 쓰래
	//반복문으로 찾아서 같은 이름&같은 성별일 때 freq[] 해당연도 수 1개 늘리기
	//새로 등장한 이름 len늘리고, 성별/이름 이름배열 추가, 해당연도 수 1개 늘리기
	while (1) {
		//파일의 끝이면 탈출
		if (feof(fp)) {
			break;
		}

		int year, num;
		char sex, *name;

		fscanf(fp, "%d %s %c %d", &year, name, &sex, &num);

		//linear search
		for (int i=0; i<names->len; i++) {
			
		}

		//capacity 늘리기
		if (names->capacity % 1000 == 0) {
			names->capacity += 1000;
			realloc(names->data, names->capacity);
		}

	}

	
}

// 이진탐색(binary search) 버전
// bsearch 함수 이용; qsort 함수를 이용하여 이름 구조체의 정렬을 유지해야 함
void load_names_bsearch( FILE *fp, int start_year, tNames *names) {
	//bsearch
	/*void* bsearch(
	const void* key, //찾고자 하는 목표 값 데이터의 주소 (Target)
	const void* base, // 배열의 주소 (DataSet[])
	size_t num,		//데이터 요소의 개수 (Length)
	size_t width,	//한 데이터 요소의 크기 (객체 크기)
	int(__cdecl* compare) (const void*, const void*) //비교 함수에 대한 포인터 */
	char *name_find;
	char sex_find;

	//name_find = bsearch(names->data, names, names->len, sizeof(tName));

	//qsort(names, names->len, sizeof(tName *), compare);
	
}

// 구조체 배열을 화면에 출력
// 이름순으로 loop -> 연도 10개 loop출력
void print_names( tNames *names, int num_year) {
	/*
	for (int i=0; i<names->len; i++) {
		printf("%s\t%c\t", names->data->name, names->data->sex);
		for (int j=0; j<num_year; j++) {
			printf("%d\t", names->data->freq[j]);
		}
		printf("\n");
	}
	*/
}

// qsort, bsearch를 위한 비교 함수
// 정렬 기준 : 이름(1순위), 성별(2순위)
int compare( const void *n1, const void *n2) {
	//((tNames *)n1)->data->name
	//어느 게 더 긴지 알아보기
	int n = (strlen(((tNames *)n1)->data->name)<=strlen(((tNames *)n2)->data->name)) ? strlen(((tNames *)n1)->data->name): strlen(((tNames *)n2)->data->name);
	for (int i=0; i<n; i++) {
		//이름 알파벳 순서대로 비교
		if (((tNames *)n1)->data->name == ((tNames *)n2)->data->name) {
			continue;
		}
		return ((tNames *)n1)->data->name - ((tNames *)n2)->data->name;
	}
	//여자 먼저 정렬이라고 생각했을 때
	return ((tNames *)n1)->data->sex - ((tNames *)n2)->data->sex;
}

/*
////////////////////////////////////////////////////////////////////////
// 연도별 입력 파일을 읽어 이름 정보(이름, 성별, 빈도)를 이름 구조체에 저장
// 이미 구조체에 존재하는(저장된) 이름은 해당 연도의 빈도만 저장
// 새로 등장한 이름은 구조체에 추가
// 주의사항: 동일 이름이 남/여 각각 사용될 수 있으므로, 이름과 성별을 구별해야 함
// names->capacity는 1000으로부터 시작하여 1000씩 증가 (1000, 2000, 3000, ...)
// start_year : 시작 연도 (2009)
// 선형탐색(linear search) 버전
void load_names_lsearch( FILE *fp, int start_year, tNames *names) {
	while(1) {
		if (feof(fp)) break;

		int year, count;
		char *name, sex;

		//읽기->변수에 저장->선형탐색->빈도저장 or 구조체에 추가
		fscanf("%d %s %c %d", &year, name, &sex, &count);

		for (int i=0; i<names->len; i++) {
			//일치하는 것 찾았을 때
			if (sex == (*names).data[i].sex && *name == (*names).data[i].name) {
				(*names).data[i].freq[year-2009] += count;
			}

			//끝까지 봤는데도 일치하는 게 없을 때 - 구조체에 추가
			else if (i == names->len-1 && (sex == (*names).data[i].sex || *name == (*names).data[i].name)) {
				(*names).data[i+1].sex = sex;
				names->data[i+1]->name = &name;
				for (int k=0; k<10; k++) {
					(*names).data[i+1].freq[k] = 0;
				}
				(*names).data[i+1].freq[year-2009] = count;
				(names->len)++;
			}
			else continue;
		}


		
	}
}

// 이진탐색(binary search) 버전
// bsearch 함수 이용; qsort 함수를 이용하여 이름 구조체의 정렬을 유지해야 함
void load_names_bsearch( FILE *fp, int start_year, tNames *names);

// 구조체 배열을 화면에 출력
void print_names( tNames *names, int num_year) {
	for (int i=0; i<names->len; i++) {
		printf("%s\t%c\t", (*names).data[i].name, (*names).data[i].sex);
		for (int j=0; j<num_year; j++) {
			printf("%d\t", names->data->freq[j]);
		}
		printf("\n");
	}
}

// qsort, bsearch를 위한 비교 함수
// 정렬 기준 : 이름(1순위), 성별(2순위)
int compare( const void *n1, const void *n2) {
	int n = (strlen(((tNames *)n1)->data->name)<=strlen(((tNames *)n2)->data->name)) ? strlen(((tNames *)n1)->data->name): strlen(((tNames *)n2)->data->name);
	for (int i=0; i<n; i++) {
		//이름 알파벳 순서대로 비교
		if (((tNames *)n1)->data->name == ((tNames *)n2)->data->name) {
			continue;
		}
		return ((tNames *)n1)->data->name - ((tNames *)n2)->data->name;
	}
	//여자 먼저 정렬이라고 생각했을 때
	return ((tNames *)n1)->data->sex - ((tNames *)n2)->data->sex;
}

*/