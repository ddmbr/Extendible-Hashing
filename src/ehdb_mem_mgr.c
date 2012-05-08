static void *page_ptr;
static void *page_head;
int page_num;

void
inc_page_ptr(void)
{

}

void
ehdb_mem_init(void)
{
    page_head = (void*)malloc(page_num * 8 * 1024);
}
