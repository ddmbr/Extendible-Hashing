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
		quick_sorting(record_array, 0,record_length-1, record_length);
		printf("%d|%d|%d|%d|%f|%f|%f|%f|%c|%c|%d|%d|%d|%c|%c|%c|\n", record_data[while_count].orderkey, record_data[while_count].partykey, record_data[while_count].suppkey, record_data[while_count].linenumber, record_data[while_count].quantity, record_data[while_count].extendedprice, record_data[while_count].discount, record_data[while_count].tax, record_data[while_count].returnflag, record_data[while_count].linestatus, record_data[while_count].shipdate, record_data[while_count].commitdate, record_data[while_count].receiptdate, record_data[while_count].shipinstruct, record_data[while_count].shipmode, record_data[while_count].comment);
		);
		fprintf(fout, "%d|%d|%d|%d|%f|%f|%f|%f|%c|%c|%d|%d|%d|%c|%c|%c|-1\n", record_data[while_count].orderkey, record_data[while_count].partykey, record_data[while_count].suppkey, record_data[while_count].linenumber, record_data[while_count].quantity, record_data[while_count].extendedprice, record_data[while_count].discount, record_data[while_count].tax, record_data[while_count].returnflag, record_data[while_count].linestatus, record_data[while_count].shipdate, record_data[while_count].commitdate, record_data[while_count].receiptdate, record_data[while_count].shipinstruct, record_data[while_count].shipmode, record_data[while_count].comment);
		while_count++;	

	} 

	fclose(fout);    
}

void quick_sorting(record_t *record_array, int left, int right, int record_length){
	//int right =record_length-1;
	//int left =0;

	if(left < right){
		int i =left;
		int j =right +1;
		int k =0;

		while(1){

			while(i+1 < record_length && record_array[++i].partkey <record_array[left].partkey);
			while(j-1 >-1 && record_array[--j].partkey >record_array[left].partkey);
			if(i>=j)
				break;
			k =record_array[i].partykey;
			record_array[i].partkey =record_array[j].partkey;
			record_array[j].partkey =k;

		}
		k =record_array[i].partkey;
		record_array[i].partkey =record_array[j].partkey;
		record_array[j].partkey =k;

		quick_sorting(record_array, left, j-1, record_length);
		quick_sorting(record_array, j+1, right, record_length);
	}
}


/*
   int array_length(page_t *page_array[]){

   return sizeof(page_array)/sizeof(page_t);
   }
 */



