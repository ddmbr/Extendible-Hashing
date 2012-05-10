
// OUTPUT FILE


void ehdb_query(int key, FILE *fout){

    int hash_value, record_length;
    page_t *hash_page;
    page_t *page_array[];

    hash_value =ehdb_hash_func(key);
     
    record_length =ehdb_get_record_num(hash_value);	// data sum, quick sort maxiuman number    || wait to confirm

    if(ehdb_get_next_bucket(hash_value) ==NULL){	// Extendible Chain not exist
	
	//hash_page =ehdb_get_bucket_page(hash_value);	
	//quick sort
	//export fout
	//ehdb_raw2record	
		
    }else{
	
	//hash_page =ehdb_get_bucket_page(hash_value); & next
	//the same
    }
    fclose(fout);    
}

void quick_sorting(page_t *page_array[], int record_length){

    if(left < right){
	int right =record_length-1;
	int left =0;

	int i =left;
	int j =right +1;
	int k =0;

	while(1){

	    while(i+1 < record_length && page_array[++i].page_id <page_array[left].page_id);
	    while(j-1 >-1 && page_array[--j].page_id >page_array[left].page_id);
    	    if(i>=j)
		break;
	    k =page_array[i].page_id;
	    page_array[i].page_id =page_array[j].page_id;
	    page_array[j].page_id =k;

	}
	k =page_array[i].page_id;
	page_array[i].page_id =page_array[j].page_id;
	page_array[j].page_id =k;

	quick_sorting(page_array, left, j-1);
	quick_sorting(page_array, j+1,, right);
    }
}


/*
int array_length(page_t *page_array[]){

    return sizeof(page_array)/sizeof(page_t);
}
*/



