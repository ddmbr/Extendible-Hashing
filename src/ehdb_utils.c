# include "ehdb_init.h"
# include "ehdb_utils.h"
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

void
ehdb_statistics()
{
    printf("STAT: taking snapshot...\n");
    faddr_inc();
    snapshot = fopen(faddr, "w");
    fprintf(snapshot, "Global_depth=%d\n", Global_depth);
    fprintf(snapshot, "Total IO times=%d\n", IO_count);
    ehdb_print_hashindex(faddr);
    fclose(snapshot);
    printf("STAT: Done.\n");
}

void
ehdb_print_hashindex(char *out_path)
{
    printf("generating hash index...\n");
    FILE * fout;
    fout = fopen(out_path, "w");

    page_t *index_page;
    page_t *bucket_page;
    int bucket_id;

    int i;
    int n = (1 << Global_depth);
    int size;

    fprintf(fout, "[hash value] -> [bucket id] : [n(record) in that bucket]\n");
    for(i = 0; i < n; i++)
    {
        index_page = ehdb_get_index_page(i / Dictpair_per_page);
        bucket_id = ((int*)(index_page->head))[i % Dictpair_per_page];
        //bucket_page = ehdb_get_bucket_page(bucket_id);
        //size = ehdb_get_record_num(bucket_page);
        //fprintf(fout, "%d -> %d, record_num: %d\n", i, bucket_id, size);
        fprintf(fout, "%d -> %d\n", i, bucket_id);
    }
    printf("Done.");
}
