# define PAGE_SIZE (8 * 1024)

static void *page_ptr;
static void *first_page;
int page_num;

void
inc_page_ptr(void)
{
    page_ptr = page_ptr + PAGE_SIZE;
    if(page_ptr > first_page + (page_num * PAGE_SIZE))
        page_ptr -= (page_num * PAGE_SIZE);
}

void
ehdb_mem_init(void)
{
    first_page = (void*)malloc(page_num * PAGE_SIZE);
    page_ptr = first_page;
}

void*
ehdb_save_string(source_ptr, is_full)
    char *source_ptr,
    int *is_full
{
    char *start_pos = source_ptr;
    char *temp;
    char *end_ptr;
    char *begin_ptr;
    double num;

    while(*source_ptr != '\n')
        source_ptr++;

    begin_ptr = free_begin();
    end_ptr = free_end();
    end_ptr -= (source_ptr - start_pos);
    if(end_ptr > begin_ptr)
    {
        strncpy(end_ptr, start_pos, (source_ptr - start_pos));
        return source_ptr + 1;
    }
    else
    {
        *is_full = 1;
        return start_pos;
    }
}
