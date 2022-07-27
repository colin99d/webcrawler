WARNING = -Wall -Wshadow --pedantic
ERROR = -Wvla -Werror
GCC = gcc -std=c99 -g $(WARNING) $(ERROR)

SRCS = main.c requests.c
OBJS = $(SRCS:%.c=%.o)

# object files are created by converting .c to .o
main: $(OBJS)
		$(GCC) $(OBJS) -lcurl -o main

# convert .c to .o
.c.o:
		$(GCC) $(TESTFLAGS) -c $*.c
