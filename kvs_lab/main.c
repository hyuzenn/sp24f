#include "kvs.h"

int main()
{
	kvs_t* my_kvs = open_kvs();


	if(!kvs) {
		printf("Failed to open kvs\n");
		return -1;
	}

	// workload execution  
	
	// 1) 	file read 
	// 2) 	if put, insert (key, value) into kvs.
	// 		if get, seach the key in kvs and return the value. 
	//		Return -1 if the key is not found  


	close_kvs(my_kvs);
	
	return 0;
}
