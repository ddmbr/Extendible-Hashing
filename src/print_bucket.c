#include "ehdb_page.h"
#include "ehdb_record.h"
#include "ehdb_buffer_mgr.h"
#include "ehdb_file_mgr.h"
#include "stdio.h"

char s[1000];
record_t record;

int main(){
    int offset;
    ehdb_buffer_init();
    ehdb_file_init();
    page_t* page_ptr = ehdb_get_bucket_page(0);
    offset = 16;
    while(offset != -1)
    {
        offset = ehdb_page_record2record(page_ptr, offset, &record);
        ehdb_record2raw(&record, s);
        printf("read a new:\n  %s\n", s);
    }
    return 0;
}
