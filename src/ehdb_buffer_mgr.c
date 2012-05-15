#include "ehdb_buffer_mgr.h"
#include "ehdb_file_mgr.h"
#include "ehdb_hash.h"
#include "ehdb_page.h"
#include "ehdb_init.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

struct clock_list_node_t{
    page_t * page;
    int refbit;
};

typedef struct clock_list_node_t clock_list_node_t;

clock_list_node_t clock_list[PAGE_NUM];
#define CACHE_SIZE 2
#ifdef CACHE
int cache_id[CACHE_SIZE];
page_type_t cache_type[CACHE_SIZE];
page_type_t cache_pos[CACHE_SIZE];
int cache_hand;
#endif


/* clock_hand: the index of hand of the Clock page replacement algorithm
 * clock_head: the head index of the list. The available items are 
 *      [clock_head, clock_head + clock_size - 1]. Here clock_head = 0
 *      At any time, keep clock_head between [0, Page_num)
 * clock_size: the list's size
 */
int clock_hand, 
    clock_size;

int total_count, 
    total_hit,
    cache_hit,
    io_count;

void
ehdb_buffer_init(){
#ifdef DEBUG
    fprintf(stderr, "buffer mgr initing...\n");
#endif
    clock_size = 0;
    clock_hand = 0;
    total_count = total_hit = cache_hit = 0;
    io_count = 0;
#ifdef CACHE
    cache_pos[0] = cache_pos[1] = -1;
    cache_hand = 0;
#endif
}

static void
swap_out_page(page_t* page){
#ifdef TRACKIO
    char * ffk[2] = {"INDEX", "BUCKET"};
    fprintf(stderr, "swap out page(id=%d, type=%s)\n", page->page_id, ffk[page->page_type]);
#endif
#ifdef CACHE
    int i;
    for(i = 0; i < CACHE_SIZE; i++)
        if(cache_pos[i] != -1 && cache_id[i] == page->page_id && cache_type[i] == page->page_type)
            cache_pos[i] = -1;
#endif
    if(page->modified){
#ifdef TRACKIO
        fprintf(stderr, "need write back to file\n");
#endif
        ehdb_save_to_file(page);
    }
}

#ifdef DEBUG
void print_clock_state(){
    fprintf(stderr, "clock{start:%d, size:%d, hand:%d}\n", 0, clock_size, clock_hand);
}
#endif

/* Find the available position in the clock_list. Will swap out old page if
 * neccessery.
 */
int 
available_page_pos(){
    int pos;
    if(clock_size < PAGE_NUM){
        pos = clock_size ;
        clock_size++;
        // allocate the page space for the page
        clock_list[pos].page = (page_t*)malloc(sizeof(page_t));
        clock_list[pos].page->head = malloc(PAGE_SIZE);
        /* clock_list[pos].page->head = NULL; */
        /* clock_hand = pos; */
    }else{
        while(clock_list[clock_hand].refbit == 1){
            clock_list[clock_hand].refbit = 0;
            clock_hand = (clock_hand + 1) % PAGE_NUM;
        }
        swap_out_page(clock_list[clock_hand].page);
        pos = clock_hand;
    }
#ifdef TRACKIO
    fprintf(stderr, "available_page_pos = %d\n", pos);
#endif
    return pos;
}

/* load page from disk into page list
 */
page_t*
load_page(int page_id, page_type_t page_type){

    int page_pos;
    clock_list_node_t * node;
    // find page in mem
    page_pos = find_page(page_type, page_id);
    total_count += 1;
    if(page_pos != -1){
        total_hit += 1;
        // if found page on mem
        clock_list[page_pos].refbit = 1;
#ifdef CACHE
        cache_type[cache_hand] = page_type;
        cache_id[cache_hand] = page_id;
        cache_pos[cache_hand] = page_pos;
        cache_hand = (cache_hand + 1) % CACHE_SIZE;
#endif
#ifdef TRACKIO
        if(page_type == INDEX)
            fprintf(stderr, "fetch page{id:%d, type:%s} in mem\n", page_id, "INDEX");
        else if(page_type == BUCKET)
            fprintf(stderr, "fetch page{id:%d, type:%s} in mem\n", page_id, "BUCKET");
        fprintf(stderr, "hit rate: %.4f\n", (double)total_hit*100/total_count);
#endif
        return clock_list[page_pos].page;
    }
    // if can not found
#ifdef TRACKIO
    fprintf(stderr, "I can not found page(id=%d)\n", page_id);
#endif
    page_pos = available_page_pos();
    node = &clock_list[page_pos];
    node->page->page_type = page_type;
    node->page->page_id = page_id;
    ehdb_copy_from_file(node->page);
    io_count ++;
#ifdef TRACKIO
    if(page_type == INDEX)
        fprintf(stderr, "page{id:%d, type:%s} loaded into %d\n", page_id, "INDEX", page_pos);
    else if(page_type == BUCKET)
        fprintf(stderr, "page{id:%d, type:%s} loaded into %d\n", page_id, "BUCKET", page_pos);
    fprintf(stderr, "hit rate: %.4f\n", (double)total_hit*100/total_count);

#endif
    node->refbit = 1;
    return node->page;
}


/* find the page in clock_list. If found, return the position index in the list, otherwise 
 * return -1
 */
int 
find_page(page_type_t page_type, int page_id){
    int i, j;
#ifdef CACHE
    // find in the cache first
    for(i = 0; i < CACHE_SIZE; i++)
        if(cache_pos[i] != -1){
            if(cache_id[i] == page_id && cache_type[i] == page_type){
                cache_hit ++;
                return cache_pos[i];
            }
        }
#endif
    for(i = 0; i < clock_size; i++){
        j = (clock_hand + i) % clock_size;
        if(clock_list[j].page->page_id == page_id 
                && clock_list[j].page->page_type == page_type){
            return j;
        }
    }
    return -1;
}

page_t*
ehdb_get_bucket_page(int bucket_id){
#ifdef DEBUG
    if(bucket_id == 57)
    {
        fprintf(stderr, "Bark\n");
    }
#endif
    return load_page(bucket_id, BUCKET);
}

page_t*
ehdb_get_index_page(int index_id){
    return load_page(index_id, INDEX);
}

page_t*
ehdb_get_bucket_page_by_hvalue(int hash_value){
    int index_id, offset, bucket_id;
    page_t * index_page, 
           * bucket_page;

    bucket_id = ehdb_get_index_map(hash_value);


    bucket_page = load_page(bucket_id, BUCKET);
    return bucket_page;
}

page_t* 
ehdb_make_available_page(){
    int pos = available_page_pos();
    clock_list[pos].refbit = 1;
    return clock_list[pos].page;
}

void
ehdb_save_pages(){
    int i, j;
    for(i = 0; i < clock_size; i++){
        j = i % PAGE_NUM;
        swap_out_page(clock_list[j].page);
    }
    fprintf(stderr, "hit rate: %.4f\n", (double)total_hit*100/total_count);
    fprintf(stderr, "cache hit rate: %.4f\n", (double)cache_hit*100/total_count);
    fprintf(stderr, "cache/hit : %.4f\n", (double)cache_hit*100/total_hit);
    fprintf(stderr, "I/O count: %d\n", io_count);
}
