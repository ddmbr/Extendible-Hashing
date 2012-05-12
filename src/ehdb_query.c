#include "ehdb_page.h"
#include "ehdb_record.h"
#include "ehdb_buffer_mgr.h"
#include <stdio.h>

char buf[500];
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
        //DO NOT write code when you feel sleepy.
        /*
        temp = a[i];
        a[i] = a[j];
        a[j] = temp;
        */
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
    int n, key;
    fscanf(fin, "%d", &n);
#ifdef DEBUG
    fprintf(stderr, "Barking!!!!!>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
#endif
    while(n--)
    {
        fscanf(fin, "%d", &key);
        ehdb_query(key, stderr); //TODO
    }
    fclose(fin);
}

/* void quick_sorting(record_t *record_array, int left, int right, int record_length){ */
/* 	//int right =record_length-1; */
/* 	//int left =0; */
/*  */
/* 	if(left < right){ */
/* 		int i =left; */
/* 		int j =right +1; */
/* 		int k =0; */
/*  */
/* 		while(1){ */
/*  */
/* 			while(i+1 < record_length && record_array[++i].partkey <record_array[left].partkey); */
/* 			while(j-1 >-1 && record_array[--j].partkey >record_array[left].partkey); */
/* 			if(i>=j) */
/* 				break; */
/* 			k =record_array[i].partykey; */
/* 			record_array[i].partkey =record_array[j].partkey; */
/* 			record_array[j].partkey =k; */
/*  */
/* 		} */
/* 		k =record_array[i].partkey; */
/* 		record_array[i].partkey =record_array[j].partkey; */
/* 		record_array[j].partkey =k; */
/*  */
/* 		quick_sorting(record_array, left, j-1, record_length); */
/* 		quick_sorting(record_array, j+1, right, record_length); */
/* 	} */
/* } */
/*  */
/*  */
/*  */
/*    int array_length(page_t *page_array[]){ */
/*  */
/*    return sizeof(page_array)/sizeof(page_t); */
/*    } */
/*   */



