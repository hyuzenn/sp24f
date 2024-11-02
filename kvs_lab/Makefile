SRCS = main.c
LIBSRCS = open.c close.c put.c get.c skiplist.c
TARG = kvs_static
TARG_DYNAMIC = kvs_dynamic

CC = gcc
OPTS = -Wall -g
LIBS = -L. -lkvs

OBJS = $(SRCS:.c=.o)
LOBJS = $(LIBSRCS:.c=.o)
LOBJS_PIC = $(LIBSRCS:.c=_pic.o)
LARS = libkvs.a
LADYN = libkvs.so

all: static dynamic

static: $(TARG)

$(TARG) : $(OBJS) $(LARS)
	$(CC) -o $(TARG) $(OBJS) $(LIBS) -static

$(LARS) : $(LOBJS)
	ar rs $(LARS) $(LOBJS)

dynamic: $(TARG_DYNAMIC)

$(TARG_DYNAMIC) : $(OBJS) $(LADYN)
	$(CC) -o $(TARG_DYNAMIC) $(OBJS) $(LIBS) -Wl,-rpath,/mnt/c/Users/huj22/source/repos/sp24f/kvs_lab

$(LADYN): $(LOBJS_PIC)
	$(CC) -shared -o $(LADYN) $(LOBJS_PIC)

# 각 소스 파일을 개별적으로 컴파일
%.o: %.c
	$(CC) $(OPTS) -c $< -o $@

# 객체 파일 삭제
clean:
	rm -f $(OBJS) $(LOBJS) $(LARS) $(LADYN) $(TARG) $(TARG_DYNAMIC)
