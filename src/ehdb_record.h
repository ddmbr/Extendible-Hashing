#pragma once
#include "ehdb_page.h"
#include "string.h"

#define RECORD_SIZE (8 * 4 + 2 + 3*4 + 3*2*2)

typedef int identifier_t;
typedef char flag_t;
typedef float decimal_t;
typedef int date_t;

struct record_t{
    identifier_t orderkey, 
               partkey, 
               suppkey;
    int linenumber;
    decimal_t quantity,
            extendedprice,
            discount,
            tax;
    flag_t returnflag,
           linestatus;
    date_t shipdate,
           commitdate,
           receiptdate;

    char shipinstruct[25+1];
    char shipmode[10+1];
    char comment[44+1];
};

typedef struct record_t record_t;

int
ehdb_get_key(record_t* record);

int
ehdb_get_invert_key(record_t* record);

/* read the string and convert to `record`, starting from `start`
 * return: the end position
 */
char * 
ehdb_raw2record(char * start, record_t * record);

/* convert a record to a raw string, ends with '\0'
 */
void 
ehdb_record2raw(record_t * record, char * raw);

/* read and convert a record from `page` start with `offset`
 * return: the new offset when conversion finished
 *
 * Example usage:
 *
 * int offset = 0
 * page_t * page = ...;
 * record_t record;
 * while(offset != -1){
 *      offset = ehdb_page_record2record(page, offset, &record);
 *      // do something or print out the record
 *      //...
 * }
 */
int 
ehdb_page_record2record(page_t * page, int offset, record_t * record);

/* convert record_t and write it back
 * to a page on disk
 */
void 
ehdb_record2page_record(record_t * record, int bucket_id);

/* check the total size if the record is to be converted to page_record
 */
size_t ehdb_test_record_size(record_t * record);

