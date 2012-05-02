struct ehdb_col
{
    int col_type;   // Fixed-Length or Variable-Length

    int length;     // When length is fixed, it's the length
                    // of the data. Otherwise inappropriate.

    struct ehdb_col* next;  // Points to the next column
}
