TARGET=roap
CC=gcc
CFLAGS=-std=c99 -Wall
LDFLAGS=-lm
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c, %.o, $(SRCS))
HEADERS=$(wildcard *.h)

# -> opção geral, i.e., quando se executa 'make' no terminal
all: CFLAGS+= -O3
all: $(TARGET)

$(TARGET): $(OBJS) Makefile
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

%.o: %.c $(HEADERS) Makefile
	$(CC) -c $(CFLAGS) -o $@ $<

# -> opção clean para limpar os object files; uso -> 'make clean'
clean:
	rm -f *.sol1 *.sol *.o log* val* $(TARGET)

# -> opção DEBUG, acrescenta a flag -g ao compilador para ajudar a usar no gdb e valgrind; uso -> 'make debug'
debug: CFLAGS+= -ggdb -Wextra
debug: $(TARGET)
