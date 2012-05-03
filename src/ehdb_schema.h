/*
 * Here is the structure to define a schema,
 * which is represented as a link list.
 */
struct ehdb_schema
{
    int col_num;            // Length of the link list

    struct ehdb_col* head;  // The beginning of the link list
};
