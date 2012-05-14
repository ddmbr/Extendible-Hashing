# include "ehdb_init.h"
# include "ehdb_buffer_mgr.h"
# include "ehdb_page.h"
# include "ehdb_record.h"
# include <stdio.h>
# include <stdlib.h>
# define BEGIN_POS 11

int snapshot_num;
int IO_count;
FILE *snapshot;
char faddr[20];

void
ehdb_statistics_init()
{
    strcpy(faddr, "snapshot0000.log\0");
    IO_count = 0;
}

void
ehdb_inc_IO_record()
{
    IO_count++;
}

void
faddr_inc()
{
    int i = 0;
    while(1)
    {
        if(faddr[BEGIN_POS - i] < '9')
        {
            faddr[BEGIN_POS - i]++;
            break;
        }
        else faddr[BEGIN_POS - i] = '0'; 
        i++;
    }
}

void ehdb_print_hashindex(char *path){
    FILE * fout = fopen(path, "w");
    fprintf(fout, "\"\"\"This is the hash index output and also a python source file\"\"\"\n");

    fprintf(fout, "Global_depth = %d\n", Global_depth);
    int i, n;
    page_t * index_page;
    int bucket_id;
    n = (1 << Global_depth);
    fprintf(fout, "hashindex = {\n");
    for(i = 0; i < n; i++){
        index_page = ehdb_get_index_page(i / Dictpair_per_page);
        bucket_id = ((int*)(index_page->head))[i % Dictpair_per_page];
        fprintf(fout, "%d: %d,\n", i, bucket_id);
    }
    fprintf(fout, "}\n");
    fclose(fout);
}

void
ehdb_statistics()
{
    int i;
    int n = (1 << Global_depth);
    int size;
    page_t *index_page;
    page_t *bucket_page;
    int bucket_id;

    faddr_inc();
    snapshot = fopen(faddr, "w");
    fprintf(snapshot, "Global_depth=%d\n", Global_depth);
    fprintf(snapshot, "Total IO times=%d\n", IO_count);
    for(i = 0; i < n; i++)
    {
        index_page = ehdb_get_index_page(i / Dictpair_per_page);
        bucket_id = ((int*)(index_page->head))[i % Dictpair_per_page];
        bucket_page = ehdb_get_bucket_page(bucket_id);
        size = ehdb_get_record_num(bucket_page);
        fprintf(snapshot, "%d -> %d, record_num: %d\n", i, bucket_id, size);
    }
    fclose(snapshot);
}
