#include "ehdb_page.h"
#include "ehdb_hash.h"
#include "ehdb_record.h"
#include "ehdb_query.h"
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

	int i, hv, offset, while_count = 0;
    int next_bucket;
	page_t * hash_bucket;

	hv = ehdb_hash_func(key, Global_depth);
	hash_bucket = ehdb_get_bucket_page_by_hvalue(hv); 

    while(1)
    {
        if(ehdb_get_record_num(hash_bucket) != 0)
        {
            offset = 16;
            while(1){
                offset = ehdb_page_record2record(hash_bucket, offset, record_array + while_count);
    #ifdef DEBUG1
                fprintf(stderr, "in hv: %d, expect key %d, found %d\n", hv, key, ehdb_get_key(record_array+while_count));
    #endif
                if(ehdb_get_key(record_array+while_count) == key)
                    while_count++;	
                if(offset == -1) break;
            } 
        }
        next_bucket = ehdb_get_next_bucket(hash_bucket->page_id);
        if(next_bucket != 0)
        {
            hash_bucket = ehdb_get_bucket_page(next_bucket);
        }
        else
        {
            break;
        }
    }

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
ehdb_bulk_query(char * faddr_in, char* faddr_out){
    FILE *fin = fopen(faddr_in, "r");
    FILE *fout = fopen(faddr_out, "w");
    int n, key;
    fscanf(fin, "%d", &n);
    while(n--)
    {
        fscanf(fin, "%d", &key);
        ehdb_query(key, fout); 
    }
    fclose(fin);
    fclose(fout);
}

