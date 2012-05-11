#include "ehdb_page.h"
#include "ehdb_record.h"
#include "ehdb_buffer_mgr.h"
#include "ehdb_file_mgr.h"
#include "stdio.h"
#include "stdlib.h"

char s[1000];
record_t record;
page_t page;

/* int main(){ */
/*     int offset; */
/*     ehdb_buffer_init(); */
/*     ehdb_file_init(); */
/*     page_t* page_ptr = ehdb_get_bucket_page(1); */
/*     offset = 16; */
/*     while(offset != -1) */
/*     { */
/*         offset = ehdb_page_record2record(page_ptr, offset, &record); */
/*         ehdb_record2raw(&record, s); */
/*         printf("read a new:\n  %s\n", s); */
/*         printf("new offset: %d\n", offset); */
/*     } */
/*     return 0; */
/* } */

int main(){
    ehdb_file_init();
    page.head = malloc(PAGE_SIZE);
    page.page_id = 6;
    page.page_type = BUCKET;
    ehdb_copy_from_file(&page);
    int offset = 16;
    while(offset != -1)
    {
        offset = ehdb_page_record2record(&page, offset, &record);
        ehdb_record2raw(&record, s);
        printf("read a new:\n  %s\n", s);
        printf("new offset: %d\n", offset);
    }
    return 0;
}
