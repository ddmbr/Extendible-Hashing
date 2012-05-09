Compiler=gcc
Flags=-g 
Objects=ehdb_parser.o ehdb_record.o

ehdb_parser.o: ehdb_parser.c ehdb_parser.h ehdb_buffer_mgr.h ehdb_record.h
	$(Compiler) $(Flags) -c ehdb_parser.c

ehdb_record.o: ehdb_record.c ehdb_record.h ehdb_page.h
	$(Compiler) $(Flags) -c ehdb_record.c

test_parser: test_parser.c ehdb_parser.h ehdb_page.h ehdb_record.h ehdb_parser.o ehdb_record.o
	$(Compiler) $(Flags) ehdb_parser.o ehdb_record.o test_parser.c -o test_parser

clean:
	rm -vf $(Objects) test_parser
