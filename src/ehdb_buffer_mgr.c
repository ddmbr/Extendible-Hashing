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

/* clock_hand: the index of hand of the Clock page replacement algorithm
 * clock_head: the head index of the list. The available items are 
 *      [clock_head, clock_head + clock_size - 1]. 
 *      At any time, keep clock_head between [0, Page_num)
 * clock_size: the list's size
 */
int clock_hand, 
    clock_head, 
    clock_size;

void
ehdb_buffer_init(){
#ifdef DEBUG
    fprintf(stderr, "buffer mgr initing...\n");
#endif
    clock_head = clock_size = 0;
    clock_hand = 0;
}

void
swap_out_page(page_t* page){
#ifdef DEBUG
    char * ffk[2] = {"INDEX", "BUCKET"};
    fprintf(stderr, "swap out page(id=%d, type=%s)\n", page->page_id, ffk[page->page_type]);
#endif
    if(page->modified){
#ifdef DEBUG
        fprintf(stderr, "need write back to file\n");
#endif
        ehdb_save_to_file(page);
    }
}

#ifdef DEBUG
void print_clock_state(){
    fprintf(stderr, "clock{start:%d, size:%d, hand:%d}\n", clock_head, clock_size, clock_hand);
}
#endif

/* Find the available position in the clock_list. Will swap out old page if
 * neccessery.
 */
int 
available_page_pos(){
    int pos;
    if(clock_size < PAGE_NUM){
        pos = (clock_head + clock_size) % PAGE_SIZE;
        clock_size++;
        // allocate the page space for the page
        clock_list[pos].page = (page_t*)malloc(sizeof(page_t));
        clock_list[pos].page->head = malloc(PAGE_SIZE);
    }else{
        while(clock_list[clock_hand].refbit == 1){
            clock_list[clock_hand].refbit = 0;
            // increace clock_hand
            if(clock_hand == (clock_head + clock_size - 1) % PAGE_SIZE){
                clock_hand = clock_head;
            }else{
                clock_hand = (clock_hand + 1) % PAGE_SIZE;
            }
        }
        swap_out_page(clock_list[clock_hand].page);
        pos = clock_hand;
    }
#ifdef DEBUG
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
    if(page_pos != -1){
        // if found page on mem
        clock_list[page_pos].refbit = 1;
#ifdef DEBUG
        if(page_type == INDEX)
            fprintf(stderr, "page{id:%d, type:%s} loaded\n", page_id, "INDEX");
        else if(page_type == BUCKET)
            fprintf(stderr, "page{id:%d, type:%s} loaded\n", page_id, "BUCKET");
#endif
        return clock_list[page_pos].page;
    }
    // if can not found
#ifdef DEBUG
    fprintf(stderr, "I can not found page(id=%d)\n", page_id);
#endif
    page_pos = available_page_pos();
    node = &clock_list[page_pos];
    node->page->page_type = page_type;
    node->page->page_id = page_id;
    ehdb_copy_from_file(node->page);
#ifdef DEBUG
    if(page_type == INDEX)
        fprintf(stderr, "page{id:%d, type:%s} loaded\n", page_id, "INDEX");
    else if(page_type == BUCKET)
        fprintf(stderr, "page{id:%d, type:%s} loaded\n", page_id, "BUCKET");
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
    for(i = clock_head; i < clock_head + clock_size; i++){
        j = i % PAGE_NUM;
        if(clock_list[j].page->page_id == page_id 
                && clock_list[j].page->page_type == page_type){
            return j;
        }
    }
    return -1;
}

page_t*
ehdb_get_bucket_page(int bucket_id){
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

    bucket_id = ehdb_get_bucket_id_by_hvalue(hash_value);

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
    for(i = clock_head; i < clock_head + clock_size; i++){
        j = i % PAGE_NUM;
        swap_out_page(clock_list[j].page);
    }
}
