#include "kvs.h"

int main()
{
	kvs_t* kvs = open();

	if(!kvs) {
		printf("Failed to open kvs\n");
		return -1;
	}

	FILE* queryFile = fopen("query.dat", "r");
    FILE* answerFile = fopen("answer.dat", "w");
	
	if (!queryFile || !answerFile){
		printf("File not Found\n");
		close(kvs);
		return -1;
	}
	printf("Opening Files Successed\n");

	char op[10], key_q[100], value_q[100]; //buffer

    while (fscanf(queryFile, "%[^,],%[^,],%s\n", op, key_q, value_q) != EOF) {
		if (strcmp(op, "get") == 0){
			//get으로 찾은 결과(value)를 answer.dat에 저장, 존재하지 않는 key면 -1로 저장.
			char* return_val = get(kvs, key_q);
			if (!return_val){
				fprintf(answerFile, "%s\n", "-1");
				continue;
			}
			fprintf(answerFile, "%s\n", return_val);
		
		}
		else if (strcmp(op, "set") == 0){
			//set(get)이면 새로운 key, value 등록
			put(kvs, key_q, value_q);
		}
    }

    fclose(queryFile);
    fclose(answerFile);

	close(kvs);
	
	return 0;
}
	// workload execution  
	
	// 1) 	file read 
	// 2) 	if put, insert (key, value) into kvs.
	// 		if get, seach the key in kvs and return the value. 
	//		Return -1 if the key is not found  