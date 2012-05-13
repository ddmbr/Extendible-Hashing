#include "ehdb_parser.h"
#include "ehdb_page.h"
#include "ehdb_record.h"
#include "stdio.h"
#include "ehdb_init.h"
#include "stdlib.h"

char str[300];
page_t page;
record_t record;

int main(){
    ehdb_file_init();
    page.head = malloc(PAGE_SIZE);
    page.page_type = BUCKET;
    ehdb_init_page_free_end(&page);
    ehdb_init_page_record_num(&page);
    ehdb_init_page_link(&page);
    ehdb_set_page_depth(&page, 1);

    ehdb_parse_start("in");

    while(!ehdb_test_eof()){
        ehdb_next_line(&record);
        ehdb_record2page_record(&record, &page);
        printf("read a line------------------------\n");
        ehdb_record2raw(&record, str);
        printf(str);
    }

    printf("-----------------------------------------------------\n");
    int offset = 16;
    while(offset != -1){
        offset = ehdb_page_record2record(&page, offset, &record);
        printf("read a line back------------------------\n");
        ehdb_record2raw(&record, str);
        printf(str);
    }
    return 0;
}
