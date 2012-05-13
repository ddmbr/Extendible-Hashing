#include "ehdb_page.h"
#include "ehdb_record.h"
#include "ehdb_buffer_mgr.h"
#include <stdio.h>

char buf[300];
record_t record_array[10];

void select_sort(record_t * a, int n){
    int i, j, k;
    record_t temp;
    for(i = 0; i < n; i++){
        k = i;
        for(j = i + 1; j < n; j++){
            if(a[j].partkey < a[k].partkey)
                k = j;
        }
        temp = a[i];
        a[i] = a[k];
        a[k] = temp;
    }
}

void ehdb_query(int key, FILE *fout){

	int i, hash_value, offset, while_count = 0;
	page_t * hash_bucket;

	hash_value = ehdb_hash_func(key, Global_depth);
	hash_bucket = ehdb_get_bucket_page_by_hvalue(hash_value); 

    //DO NOT write code when you feel sleepy.
	//while(offset = ehdb_page_record2record(hash_bucket, offset, record_array + while_count) && offset != -1){ *Notice PIORITY!*
	//while((offset = ehdb_page_record2record(hash_bucket, offset, record_array + while_count)) && offset != -1){
	if(ehdb_get_record_num(hash_bucket) != 0)
    {
        offset = 16;
        while(1){
            offset = ehdb_page_record2record(hash_bucket, offset, record_array + while_count);
            if(ehdb_get_key(record_array+while_count) == key)
                while_count++;	
            if(offset == -1) break;
        } 
    }

/* #ifdef DEBUG */
    else{
        fprintf(stderr, "Record key=%d not found \n", key);
    }
/* #endif */
    /* quick_sorting(record_array, 0, while_count-1, record_length); */
    select_sort(record_array, while_count);
    for(i=0; i < while_count; i++){
        //DO NOT write code when you feel sleepy.
        //ehdb_record2raw(record_array+while_count, buf);
        ehdb_record2raw(record_array + i, buf);
        fprintf(fout, "record: %s\n", buf);
    }
    fprintf(fout, "-1\n");
}

void
ehdb_bulk_query(char * faddr){
    FILE *fin = fopen(faddr, "r");
    /* FILE *fin = stdin; */
    int n, key;
    fscanf(fin, "%d", &n);
#ifdef DEBUG
    fprintf(stderr, "Barking!!!!!>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
#endif
    while(n--)
    {
        fscanf(fin, "%d", &key);
        ehdb_query(key, stdout); //TODO: ouput to file
    }
    fclose(fin);
}

