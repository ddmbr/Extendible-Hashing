void
ehdb_bulk_insert(file_loc)
    char *file_loc
{
    FILE *source_tbl_file;
    char *source_ptr;
    char *temp_page_ptr;
    int key;
    int record_num = 0;
    int local_depth = 1;
    int global_depth = 1;
    int next_page = 0;

    source_tbl_file = fopen(file_loc, "r");
    source_ptr = (char*)malloc((size_t)(8 * 1024));
    fread(source_ptr, 8 * 1024, 1, source_tbl_file);

    while() //TODO while page not full
    {
        //seek for the key
        for(i = 1; i <= schema_length; i++)
        {
            if(i != ehdb_key)
            {
                while(*source_ptr != '|') source_ptr++;
                source_ptr++;
                continue;
            }
            ptr = ehdb_read_int(ptr, key);
        }

        //with the key, fetch the corresponding page
        //read data
        //check whether page is full while reading data
        //if full, set the pointer beyond the free space pointer
        for(i = 1; i <= schema_length; i++)
        {
            switch(ehdb_data_type[i])
            {
                case INT:
                    source_ptr = ehdb_read_int(source_ptr, int_num);
                    break;
                case FLOAT:
                    source_ptr = ehdb_read_float(source_ptr);
                    break;
                case STRING:
                    ehdb_read_string(ptr);
                    break;
            }
        }
        //if success, modify the counter
        //else create a new page
    }
}
