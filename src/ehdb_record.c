#include "ehdb_record.h"
#include "stdio.h"
#include "string.h"
#include "ehdb_page.h"
#include "ehdb_init.h"
#include <assert.h>
#define MAX_DEPTH 30

int
ehdb_get_key(record_t* record){
    return record->orderkey;
}

int
ehdb_get_invert_key(record_t* record){
    int key = ehdb_get_key(record);
    int invert = 0, i;

    for(i = MAX_DEPTH - 1; i >= 0; i--)
        if((key & (1 << i)) > 0)
            break;
    int sig = i;

    key &= ((1 << sig) - 1);

    for(i = 1; i <= sig; i++)
        invert += ((key & (1 << (i - 1))) > 0) << (sig - i);
    return invert;
}

// convert ints to date_t
date_t ints2date(int y, int m, int d){
    return (y << 8) + (m << 4) + d;
}

// convert date_t to ints
void date2ints(date_t date, int ymds[3]){
    ymds[0] = date >> 8;
    ymds[1] = (date >> 4) & 0xf;
    ymds[2] = (date) % 0xf;
}

char * 
ehdb_raw2record(char * start, record_t * record){
    int ymds[3][3];
    memset(record->shipinstruct, 0, sizeof record->shipinstruct);
    memset(record->shipmode, 0, sizeof record->shipmode);
    memset(record->comment, 0, sizeof record->comment);
    sscanf(start, 
            //"%d|%d|%d|%d|%f|%f|%f|%f|%c|%c|%d-%d-%d|%d-%d-%d|%d-%d-%d|%[^|]|%[^|]|%[^|]|\n",
            "%d|%d|%d|%d|%f|%f|%f|%f|%c|%c|%d-%d-%d|%d-%d-%d|%d-%d-%d|%[^|]|%[^|]|%[^|]|\n",
            &record->orderkey, 
            &record->partkey,
            &record->suppkey,

            &record->linenumber,

            &record->quantity,
            &record->extendedprice,
            &record->discount,
            &record->tax,

            &record->returnflag,
            &record->linestatus,

            ymds[0], ymds[0]+1, ymds[0]+2,
            ymds[1], ymds[1]+1, ymds[1]+2,
            ymds[2], ymds[2]+1, ymds[2]+2,

            record->shipinstruct,
            record->shipmode,
            record->comment
                );
    record->shipdate = ints2date(ymds[0][0], ymds[0][1], ymds[0][2]);
    record->commitdate = ints2date(ymds[1][0], ymds[1][1], ymds[1][2]);
    record->receiptdate = ints2date(ymds[2][0], ymds[2][1], ymds[2][2]);
    char * p = start;
    while(*p && *p != '\n') p++;
    if(*p)
        return ++p;
    else
        return p;
}

void 
ehdb_record2raw(record_t * record, char * raw){
    int ymds[3][3];
    date2ints(record->shipdate, ymds[0]);
    date2ints(record->commitdate, ymds[1]);
    date2ints(record->receiptdate, ymds[2]);
    sprintf(raw, 
            "%d|%d|%d|%d|%.2f|%.2f|%.2f|%.2f|%c|%c|%d-%d-%d|%d-%d-%d|%d-%d-%d|%s|%s|%s|\n",
            record->orderkey, 
            record->partkey,
            record->suppkey,

            record->linenumber,

            record->quantity,
            record->extendedprice,
            record->discount,
            record->tax,

            record->returnflag,
            record->linestatus,

            ymds[0][0], ymds[0][1], ymds[0][2],
            ymds[1][0], ymds[1][1], ymds[1][2],
            ymds[2][0], ymds[2][1], ymds[2][2],

            record->shipinstruct,
            record->shipmode,
            record->comment
                );
}

void* 
next_int(void * start, int * i){
    *i = ((int*)start)[0];
    return (int*)start + 1;
}

void*
next_float(void * start, float* f){
    *f = ((float*)start)[0];
    return (float*)start + 1;
}

void*
next_char(void * start, char* f){
    *f = ((char*)start)[0];
    return (char*)start + 1;
}

void*
next_str(page_t* page, void* start, char* s){
    short s_start, s_len;
    s_start = ((short*)start)[0];
    s_len = ((short*)start)[1];
    strncpy(s, (char*)(page->head+s_start), s_len);
    s[s_len] = '\0';
    return (short*)start + 2;
}

int 
ehdb_page_record2record(page_t * page, int offset, record_t * record){
    if(offset < 0) return -1;
    // read and convert a record from the page
    void * start = page->head + offset;
    start = next_int(start, &record->orderkey);
    start = next_int(start, &record->partkey);
    start = next_int(start, &record->suppkey);
    start = next_int(start, &record->linenumber);

    start = next_float(start, &record->quantity);
    start = next_float(start, &record->extendedprice);
    start = next_float(start, &record->discount);
    start = next_float(start, &record->tax);

    start = next_char(start, &record->returnflag);
    start = next_char(start, &record->linestatus);

    start = next_int(start, &record->shipdate);
    start = next_int(start, &record->commitdate);
    start = next_int(start, &record->receiptdate);

    start = next_str(page, start, record->shipinstruct);
    start = next_str(page, start, record->shipmode);
    start = next_str(page, start, record->comment);

    if(ehdb_free_begin(page) <= start)
        return -1;
    else
        return (int)(start - page->head);
        //return start - page->head - offset;
}

void*
write_int(void* p, int i){
    ((int*)p)[0] = i;
    return (int*)p+1;
}
void*
write_float(void* p, float i){
    ((float*)p)[0] = i;
    return (float*)p+1;
}
void*
write_char(void* p, char i){
    ((char*)p)[0] = i;
    return (char*)p+1;
}

void 
write_str(page_t* page, void**begin, void**end, char*s){
    int len = strlen(s);
    strncpy((char*)(*end - len), s, strlen(s));
    //write string offset
    ((short*)(*begin))[0] = (short)(*end - len - page->head);
    ((short*)(*begin))[1] = (short)len;
    *end = (char*)(*end) - len;
    *begin = (short*)(*begin) + 2;
}
void
//ehdb_record2page_record(record_t * record, page_t * page){
ehdb_record2page_record(record_t * record, int bucket_id){
    void * begin,
         * end;
    page_t *page_ptr = ehdb_get_bucket_page(bucket_id);
    assert(page_ptr->page_type == BUCKET);

    begin = ehdb_free_begin(page_ptr);
    end = ehdb_free_end(page_ptr);
    page_ptr->modified = 1;

#ifdef DEBUG
    fprintf(stderr, "insert record (key=%d, hv=%d, into bucket page(id=%d), begin %d, end %d\n",
            record->orderkey, ehdb_hash_func(record->orderkey, Global_depth),
            page->page_id, (begin - page->head), (end - page->head));
    FILE * logf = fopen("insert.log", "a");
    fprintf(logf, "rid(%d) -> (%d, %d)\n", record->orderkey, page->page_type, page->page_id);
    fclose(logf);
#endif

    begin = write_int(begin, record->orderkey);
    begin = write_int(begin, record->partkey);
    begin = write_int(begin, record->suppkey);
    begin = write_int(begin, record->suppkey);

    begin = write_float(begin, record->quantity);
    begin = write_float(begin, record->extendedprice);
    begin = write_float(begin, record->discount);
    begin = write_float(begin, record->tax);

    begin = write_char(begin, record->returnflag);
    begin = write_char(begin, record->linestatus);

    begin = write_int(begin, record->shipdate);
    begin = write_int(begin, record->commitdate);
    begin = write_int(begin, record->receiptdate);

    write_str(page_ptr, &begin, &end, record->shipinstruct);
    write_str(page_ptr, &begin, &end, record->shipmode);
    write_str(page_ptr, &begin, &end, record->comment);

    //TODO:wrap
    ehdb_set_page_record_num(page_ptr, ehdb_get_record_num(page_ptr)+1);
    ehdb_set_free_end(page_ptr, end);
#ifdef DEBUG
    fprintf(stderr, "after insert, begin %d, end %d\n\n",(begin - page_ptr->head), (end - page_ptr->head));
#endif
}

size_t ehdb_test_record_size(record_t * record){
    size_t sum = 0;
    /* sum += sizeof(identifier_t) * 3; */
    /* sum += sizeof(int) * 1; */
    /* sum += sizeof(decimal_t) * 4; */
    /* sum += sizeof(flag_t) * 2; */
    /* sum += sizeof(date_t) * 3; */
    sum += RECORD_SIZE;
    sum += strlen(record->shipinstruct);
    sum += strlen(record->shipmode);
    sum += strlen(record->comment);
    return sum;
}
