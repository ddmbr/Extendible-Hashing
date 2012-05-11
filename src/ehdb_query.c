#include "ehdb_page.h"
#include "ehdb_record.h"
#include "ehdb_buffer_mgr.h"
#include <stdio.h>
#define For(i, n) for(i = 0; i < n; i++)
#define Forr(i, l, n) for(i = l; i < n; i++)

// OUTPUT FILE

char buf[500];
record_t record_array[10];

void select_sort(record_t * a, int n){
    int i, j, k;
    record_t temp;
    For(i, n){
        k = i;
        Forr(j, i+1, n){
            if(a[j].partkey < a[k].partkey)
                k = j;
        }
        temp = a[i]; a[i] = a[j]; a[j] = temp;
    }
}

void ehdb_query(int key, FILE *fout){

	int i, hash_value, offset, while_count=0;
	page_t * hash_bucket;

	hash_value = ehdb_hash_func(key, Global_depth);
	hash_bucket = ehdb_get_bucket_page_by_hvalue(hash_value); 

    offset = 16;
	while(offset = ehdb_page_record2record(hash_bucket, offset, record_array + while_count) !=-1){
        if(ehdb_get_key(record_array+while_count) != key) continue;
		while_count++;	
	} 
    /* quick_sorting(record_array, 0, while_count-1, record_length); */
    select_sort(record_array, while_count);
    for(i=0; i < while_count; i++){
        ehdb_record2raw(record_array+while_count, buf);
        fprintf(fout, "%s\n", buf);
    }
    fprintf(fout, "-1\n");
}

void
ehdb_buld_query(char * faddr){
    FILE *fin = fopen(faddr, "r");
    int n, key;
    fscanf(fin, "%d", &n);
#ifdef DEBUG
    fprintf(stderr, "Barking!!!!!>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
#endif
    while(n--)
    {
        fscanf(fin, "%d", &key);
        ehdb_query(key, stdout); //TODO
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



