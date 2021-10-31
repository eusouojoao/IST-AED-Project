TARGET=roap
CC=gcc
CFLAGS=-std=c99 -Wall -O3
LDFLAGS=-lm
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c, %.o, $(SRCS))
HEADERS=$(wildcard *.h)

# -> opção geral, i.e., quando se executa 'make' no terminal
all: $(TARGET)

$(TARGET): $(OBJS) Makefile
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

%.o: %.c $(HEADERS) Makefile
	$(CC) -c $(CFLAGS) -o $@ $<

# -> opção clean para limpar os object files; uso -> 'make clean'
clean:
	rm -f *.sol1 *.sol *.o log* $(TARGET)

FILES1 = $(shell ls ../testes/*.in1)
VALG = valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all -s --log-file="valgrind_$$(date +"%Y_%m_%d_%T").log" 
testes1:
	for F in ${FILES1}; do sleep 1 ; ${VALG} ./roap -s $${F} ; done;

FILES2 = $(shell ls ../testes/*.sol1)
DIFF = diff
diff1:
	for F in ${FILES2} ; ${DIFF} $${F} ../sol/$${F} ; done ;

# -> opção DEBUG, acrescenta a flag -g ao compilador para ajudar a usar no gdb e valgrind; uso -> 'make debug'
debug: CFLAGS+= -ggdb -Wextra
debug: $(TARGET)
