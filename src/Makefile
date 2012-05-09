Compiler=gcc
Flags=-g -DDEBUG -DL_HASH
Objects=ehdb_parser.o ehdb_record.o ehdb_buffer_mgr.o


ehdb_buffer_mgr.o: ehdb_buffer_mgr.c\
	ehdb_buffer_mgr.h\
	ehdb_page.h\
	ehdb_init.h\
	ehdb_file_mgr.h
	$(Compiler) $(Flags) -c ehdb_buffer_mgr.c

ehdb_hash.o: ehdb_hash.c\
	ehdb_file_mgr.h\
	ehdb_init.h\
	ehdb_hash.h\
	ehdb_buffer_mgr.h\
	ehdb_page.h\
	ehdb_record.h
	$(Compiler) $(Flags) -c ehdb_hash.c

ehdb_file_mgr.o: ehdb_file_mgr.c\
	ehdb_file_mgr.h\
	ehdb_page.h\
	ehdb_init.h\
	ehdb_file_mgr.h
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

test_parser: test_parser.c\
	ehdb_page.h\
	ehdb_parser.h\
	ehdb_record.h\
	ehdb_parser.o\
	ehdb_record.o
	$(Compiler) $(Flags) ehdb_parser.o ehdb_record.o test_parser.c -o test_parser

clean:
	rm -vf $(Objects) test_parser
