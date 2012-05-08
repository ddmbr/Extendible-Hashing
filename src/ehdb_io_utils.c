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
    source_ptr = ehdb_new_page();
    fread(source_ptr, 8 * 1024, 1, source_tbl_file);

    while() //TODO while page not full
    {
        //seek for the key
        for(i = 1; i <= schema_length; i++)
        {
            if(i != key_col)
            {
                while(*source_ptr != '|') source_ptr++;
                source_ptr++;
                continue;
            }
            source_ptr = ehdb_read_int(source_ptr, &key);
        }

        //TODO with the key, fetch the corresponding page
        ehdb_fetch_page(key, &is_full);

        //check whether page is full while reading data
        //with the `is_full' flag
        for(i = 1; i <= schema_length; i++)
        {
            switch(ehdb_data_type[i])
            {
                case INT:
                    //TODO
                    source_ptr = ehdb_save_int(source_ptr, &is_full);
                    break;
                case FLOAT:
                    //TODO
                    source_ptr = ehdb_save_float(source_ptr, &is_full);
                    break;
                case STRING:
                    //TODO
                    source_ptr = ehdb_save_string(source_ptr, &is_full);
                    break;
                case CHAR:
                    //TODO
                    source_ptr = ehdb_save_char(source_ptr, &is_full);
                case DATE:
                    //TODO date's format:
                    //2 bytes for the year, 1 byte for the month
                    //and 1 byte for the day
                    source_ptr = ehdb_save_date(source_ptr, &is_full);
                    break;
            }
        }
        if(!is_full)
        {
            //if success, modify the counter
            ;
        }
        else
        {
            //else create a new page
            ehdb_fetch_page(key, &is_full);
        }
    }
}
