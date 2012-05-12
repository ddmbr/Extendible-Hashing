Compiler=gcc
HashFunc=L_HASH
Flags=-g -DDEBUG -D$(HashFunc)
# Flags=-O2 -D$(HashFunc)
Objects=ehdb_parser.o\
		ehdb_record.o\
		ehdb_buffer_mgr.o\
		ehdb_hash.o\
		ehdb_file_mgr.o\
		ehdb_page.o\
		ehdb_init.o\
		ehdb_query.o\
		ehdb_utils.o
TestObjects=test_parser print_bucket

ehdb: $(Objects) main.o
	$(Compiler) $(Flags) $(Objects) main.o -o ehdb

ehdb_query.o: ehdb_query.c ehdb_page.h ehdb_record.h ehdb_buffer_mgr.h ehdb_file_mgr.h ehdb_init.h
	$(Compiler) $(Flags) -c ehdb_query.c

main.o: main.c ehdb_init.h ehdb_page.h ehdb_file_mgr.h ehdb_buffer_mgr.h ehdb_utils.h
	$(Compiler) $(Flags) -c main.c

ehdb_utils.o: ehdb_utils.c ehdb_init.h ehdb_page.h ehdb_file_mgr.h ehdb_buffer_mgr.h ehdb_record.h
	$(Compiler) $(Flags) -c ehdb_utils.c

ehdb_buffer_mgr.o: ehdb_buffer_mgr.c\
	ehdb_buffer_mgr.h\
	ehdb_page.h\
	ehdb_init.h\
	ehdb_file_mgr.h
	$(Compiler) $(Flags) -c ehdb_buffer_mgr.c

ehdb_init.o: ehdb_init.c ehdb_page.h ehdb_file_mgr.h ehdb_buffer_mgr.h ehdb_init.h ehdb_utils.h
	$(Compiler) $(Flags) -c ehdb_init.c

ehdb_page.o: ehdb_page.c ehdb_page.h ehdb_init.h ehdb_file_mgr.h ehdb_buffer_mgr.h ehdb_record.h
	$(Compiler) $(Flags) -c ehdb_page.c

ehdb_hash.o: ehdb_hash.c\
	ehdb_file_mgr.h\
	ehdb_init.h\
	ehdb_hash.h\
	ehdb_buffer_mgr.h\
	ehdb_page.h\
	ehdb_record.h\
	ehdb_utils.h
	$(Compiler) $(Flags) -c ehdb_hash.c

ehdb_file_mgr.o: ehdb_file_mgr.c\
	ehdb_file_mgr.h\
	ehdb_page.h\
	ehdb_init.h\
	ehdb_buffer_mgr.h\
	ehdb_utils.h\
	ehdb_record.h
	$(Compiler) $(Flags) -c ehdb_file_mgr.c

ehdb_parser.o: ehdb_parser.c\
	ehdb_parser.h\
	ehdb_buffer_mgr.h\
	ehdb_file_mgr.h\
	ehdb_page.h\
	ehdb_init.h\
	ehdb_record.h
	$(Compiler) $(Flags) -c ehdb_parser.c

ehdb_record.o: ehdb_record.c ehdb_record.h ehdb_page.h
	$(Compiler) $(Flags) -c ehdb_record.c

test_parser: $(Objects) test_parser.c\
	ehdb_page.h\
	ehdb_parser.h\
	ehdb_init.h\
	ehdb_record.h
	$(Compiler) $(Flags) $(Objects) test_parser.c -o test_parser

print_bucket: $(Objects) print_bucket.c ehdb_page.h ehdb_file_mgr.h ehdb_buffer_mgr.h ehdb_init.h ehdb_record.h
	$(Compiler) $(Flags) $(Objects) print_bucket.c -o print_bucket

clean:
	rm -vf $(Objects) $(TestObjects)
