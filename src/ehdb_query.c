#include <ehdb_page.h>
#include <stdio.h>

// OUTPUT FILE


void ehdb_query(int key, FILE *fout){

	int hash_value, record_length, offset, while_count=0;
	page_t *hash_bucket;
	record_t record_data;
	//page_t *page_array[];

	hash_value =ehdb_hash_func(key);
	hash_bucket =ehdb_get_bucket_page(hash_value); 
	record_length =ehdb_get_record_num(hash_value);	// data sum, quick sort maxiuman number
	record_t *record_array[record_length];

	while(offset =ehdb_page_record2record(hash_bucket, offset, &record_data) !=-1){
		record_array[while_count] =record_data;
		quick_sorting(record_array ,record_length);
		printf(record_data);
		fwrite(record_array[while_count], sizeof(record_t), 1, fout);
		while_count++;	

	} 

	fclose(fout);    
}

void quick_sorting(record_t record_array, int record_length){

	if(left < right){
		int right =record_length-1;
		int left =0;
		int i =left;
		int j =right +1;
		int k =0;

		while(1){

			while(i+1 < record_length && record_array[++i].partkey <record_data[left].partkey);
			while(j-1 >-1 && record_array[--j].partkey >record_data[left].partkey);
			if(i>=j)
				break;
			k =record_array[i].partykey;
			record_array[i].partkey =record_data[j].partkey;
			record_array[j].partkey =k;

		}
		k =record_array[i].partkey;
		record_array[i].partkey =record_data[j].partkey;
		record_array[j].partkey =k;

		quick_sorting(record_array, left, j-1);
		quick_sorting(record_array, j+1,, right);
	}
}


/*
   int array_length(page_t *page_array[]){

   return sizeof(page_array)/sizeof(page_t);
   }
 */



