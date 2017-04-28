
# ==============================================================================
#                          Unix Makefile for libndtypes
# ==============================================================================

LIBSTATIC = libndtypes.a

CC ?= gcc
LD ?= gcc
AR ?= ar
RANLIB ?= ranlib
CFLAGS ?= -Wall -Wextra -std=c11 -pedantic -O2 -g

ifeq ($(MAKECMDGOALS), check)
  CFLAGS = -Wall -Wextra -Werror -std=c11 -pedantic -O2 -g
endif

ifeq ($(MAKECMDGOALS), memcheck)
  CFLAGS = -Wall -Wextra -Werror -std=c11 -pedantic -O2 -g
endif

ifeq ($(MAKECMDGOALS), coverage)
  CFLAGS = -O0 -g -fno-inline -fprofile-arcs -ftest-coverage -fpic
  LDFLAGS = -fprofile-arcs
endif


default: $(LIBSTATIC)


OBJS = alloc.o attr.o display.o display_meta.o equal.o grammar.o lexer.o match.o ndtypes.o \
       parsefuncs.o parser.o seq.o symtable.o

$(LIBSTATIC):\
Makefile $(OBJS)
	$(AR) rc $(LIBSTATIC) $(OBJS)
	$(RANLIB) $(LIBSTATIC)

alloc.o:\
Makefile alloc.c ndtypes.h
	$(CC) $(CFLAGS) -c alloc.c

attr.o:\
Makefile attr.c attr.h ndtypes.h
	$(CC) $(CFLAGS) -c attr.c

display.o:\
Makefile display.c ndtypes.h
	$(CC) $(CFLAGS) -c display.c

display_meta.o:\
Makefile display_meta.c ndtypes.h
	$(CC) $(CFLAGS) -c display_meta.c

equal.o:\
Makefile equal.c ndtypes.h
	$(CC) $(CFLAGS) -c equal.c

grammar.o:\
Makefile grammar.c grammar.h lexer.h ndtypes.h parsefuncs.h seq.h
	$(CC) $(CFLAGS) -c grammar.c

lexer.o:\
Makefile lexer.c grammar.h lexer.h parsefuncs.h
	$(CC) $(CFLAGS) -c lexer.c

match.o:\
Makefile match.c ndtypes.h symtable.h
	$(CC) $(CFLAGS) -c match.c

ndtypes.o:\
Makefile ndtypes.c ndtypes.h
	$(CC) $(CFLAGS) -c ndtypes.c

parsefuncs.o:\
Makefile parsefuncs.c ndtypes.h parsefuncs.h seq.h
	$(CC) $(CFLAGS) -c parsefuncs.c

parser.o:\
Makefile parser.c grammar.h lexer.h ndtypes.h seq.h
	$(CC) $(CFLAGS) -c parser.c

seq.o:\
Makefile seq.c ndtypes.h seq.h
	$(CC) $(CFLAGS) -c seq.c

symtable.o:\
Makefile symtable.c ndtypes.h symtable.h
	$(CC) $(CFLAGS) -c symtable.c


# Flex generated files
lexer.h:\
Makefile lexer.c

lexer.c:\
Makefile lexer.l ndtypes.h parsefuncs.h
	flex -o lexer.c --header-file=lexer.h lexer.l

# Bison generated files
grammar.h:\
Makefile grammar.c

grammar.c:\
Makefile grammar.y ndtypes.h parsefuncs.h ndtypes.h
	bison -Wall -o grammar.c --defines=grammar.h grammar.y


# Tests
runtest:\
Makefile tests/runtest.c tests/alloc_fail.c tests/test_parse.c tests/test_parse_error.c \
tests/test_parse_roundtrip.c tests/test_indent.c tests/test_typedef.c tests/test_match.c \
tests/test_record.c ndtypes.h tests/test.h tests/alloc_fail.h $(LIBSTATIC)
	$(CC) -I. -Wno-gnu $(CFLAGS) -DTEST_ALLOC -o tests/runtest tests/runtest.c \
            tests/alloc_fail.c tests/test_parse.c tests/test_parse_error.c \
            tests/test_parse_roundtrip.c tests/test_indent.c tests/test_typedef.c \
            tests/test_match.c tests/test_record.c \
            $(LIBSTATIC)

check:\
Makefile runtest
	./tests/runtest

memcheck:\
Makefile runtest
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=tests/valgrind.supp ./tests/runtest


# Coverage
coverage:\
Makefile clean runtest
	./tests/runtest
	for file in *.c; do gcov -l "$$file" > /dev/null 2>&1; done

# Benchmark
bench:\
Makefile tools/bench.c ndtypes.h $(LIBSTATIC)
	$(CC) -I. $(CFLAGS) -o bench tools/bench.c $(LIBSTATIC)


# Print the AST
print_ast:\
Makefile tools/print_ast.c ndtypes.h $(LIBSTATIC)
	$(CC) -I. $(CFLAGS) -o print_ast tools/print_ast.c $(LIBSTATIC)


# Indent a file that contains a datashape type
indent:\
Makefile tools/indent.c ndtypes.h $(LIBSTATIC)
	$(CC) -I. $(CFLAGS) -o indent tools/indent.c $(LIBSTATIC)


clean: FORCE
	rm -f *.o *.gcov *.gcda *.gcno bench indent print_ast tests/runtest $(LIBSTATIC)

distclean: clean
	rm -f grammar.c grammar.h lexer.c lexer.h


FORCE:


