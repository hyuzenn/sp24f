#include "kvs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	FILE *queryFile = fopen("query.dat", "r");
  FILE *answerFile = fopen("answer.dat", "w");

	kvs_t* my_kvs = open_kvs();
	if (!my_kvs) {
			printf("Error: Failed to open KVS. \n");
			return 1; 
	}

    char command[10], key[100], value[100];
    while (fscanf(queryFile, "%9[^,],%99[^,],%99[^\n]\n", command, key, value) != EOF) {
        if (strcmp(command, "set") == 0) {
            put(my_kvs, key, value); 
        } else if (strcmp(command, "get") == 0) {
            char *retrievedValue = get(my_kvs, key);
            if (retrievedValue) {
                fprintf(answerFile, "%s\n", retrievedValue);
            } else {
                fprintf(answerFile, "-1\n");
            }
        }
    }

	kvs_close(my_kvs);
	fclose(queryFile);
	fclose(answerFile);

	return 0;
}