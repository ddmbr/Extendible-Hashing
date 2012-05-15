# include "ehdb_init.h"
# include "ehdb_buffer_mgr.h"
# include "ehdb_file_mgr.h"
# include "ehdb_page.h"
# include "ehdb_record.h"
# include <stdio.h>
# include <stdlib.h>
# define BEGIN_POS 11

int snapshot_num;
int IO_count;
FILE *snapshot;
char faddr[20];

void ehdb_print_hashindex(char *path){
    FILE * fout = fopen(path, "w");
    fprintf(fout, "\"\"\"This is the hash index output and also a python source file\"\"\"\n");

    fprintf(fout, "Global_depth = %d\n", Global_depth);
    int i, n;
    int bucket_id;
    n = (1 << Global_depth);
    fprintf(fout, "hashindex = {\n");
    for(i = 0; i < n; i++){
        bucket_id = ehdb_get_index_map(i);
        fprintf(fout, "%d: %d,\n", i, bucket_id);
    }
    fprintf(fout, "}\n");
    fclose(fout);
}

char bin_buf[50];
char * itob(int x){
    int i;
    for(i = 0; i < 30; i++){
        if(i % 4 == 0)
            bin_buf[i++] = ' ';
        bin_buf[i] = '0' + (x & 1);
        x >>= 1;
    }
    bin_buf[i++] = 'H';
    return bin_buf;
}

void ehdb_print_bucket(int bucket_id){
    page_t * bucket = ehdb_get_bucket_page(bucket_id);
    int n = ehdb_get_record_num(bucket);
    int i;
    printf("Bukcet(id=%d, record_num=%d, ldepth=%d)-----------\n", bucket_id, n,
            ehdb_get_depth(bucket_id));
    record_t record;
    for(i = 16; (i = ehdb_page_record2record(bucket, i, &record)) != -1;){
        printf("%s,\n", itob(record.orderkey));
    }
    printf("-----------------------------------\n");
}

