#include "kvs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{

	FILE *fp_query = fopen("query.dat", "r");
	FILE *fp_answer = fopen("answer.dat", "w");

	if (!fp_query || !fp_answer) {
		perror("File opening failed");
		return EXIT_FAILURE;
	}

	char line[1024];
	kvs_t* my_kvs = open_kvs();
	if (!my_kvs) {
			fprintf(stderr, "Error: Failed to open KVS\n");
			return 1; // 프로그램 종료
	}


	while (fgets(line, sizeof(line), fp_query)) {
		char *cmd = strtok(line, ",");
		char *key = strtok(NULL, ",");
		char *value = strtok(NULL, "\n");

		if (strcmp(cmd, "set") == 0) {
			put(my_kvs, key, value);
		}
		else if (strcmp(cmd, "get") == 0) {
			char *found_value = get(my_kvs, key);
			if (found_value) {
				fprintf(fp_answer, "%s\n", found_value);
			} else {
				fprintf(fp_answer, "-1\n");
			}
		}
	}
	kvs_close(my_kvs);
	fclose(fp_query);
	fclose(fp_answer);

	return 0;
}