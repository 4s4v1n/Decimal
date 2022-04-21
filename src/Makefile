CC=gcc
HEADERS=decimal.h
SOURCES=big_decimal_core.c \
		decimal_arithmetics.c \
		decimal_convertors.c \
		decimal_core.c \
		decimal_equal.c \
		decimal_special_functions.c \
		help_functions.c
OBJECTS=$(SOURCES:.c=.o)
CFLAGS=-c -Wall -Wextra -Werror
LDFLAGS=-lcheck -lrt -lm -lpthread -lsubunit -L. $(LIB)
LIB=decimal.a
TFILE=test.c


all: $(LIB)

$(OBJECTS): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES)

$(LIB): $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	rm -rf *.o

test: $(TFILE) $(LIB)
	$(CC) $(TFILE) $(LIB) -o test.out $(LDFLAGS)
	./test.out 

clean:
	rm -rf *.o *.a *.out
