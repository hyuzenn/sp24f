SRCS = main.c
LIBSRCS = open.c close.c put.c get.c skiplist.c
TARG = kvs_static
TARG_DYNAMIC = kvs_dynamic
OBJ_FILES = main.o open.o close.o put.o get.o skiplist.o
CFLAGS = -Wall -g -Wno-unused-but-set-variable

CC = gcc
OPTS = -Wall -g
LIBS = -L. -lkvs

OBJS = $(SRCS:.c=.o)
LOBJS = $(LIBSRCS:.c=.o)
LOBJS_PIC = $(LIBSRCS:.c=_pic.o)  # PIC 파일 목록
LARS = libkvs.a
LADYN = libkvs.so

all: static dynamic

# 정적 라이브러리 타겟
static: $(TARG)

$(TARG): $(OBJS) $(LARS)
	$(CC) -o $(TARG) $(OBJS) $(LIBS) -static

# 정적 라이브러리 생성
$(LARS): $(LOBJS)
	ar rs $(LARS) $(LOBJS)

# 동적 라이브러리 타겟
dynamic: $(TARG_DYNAMIC)

$(TARG_DYNAMIC): $(OBJS) $(LADYN)
	$(CC) -o $(TARG_DYNAMIC) $(OBJS) $(LIBS) -Wl,-rpath,/mnt/c/Users/huj22/source/repos/sp24f/kvs_lab

# 동적 라이브러리 생성
$(LADYN): $(LOBJS_PIC)
	$(CC) -shared -o $(LADYN) $(LOBJS_PIC)

# 각 소스 파일을 개별적으로 컴파일
%.o: %.c
	$(CC) $(OPTS) -c $< -o $@

# 위치 독립 코드 파일을 개별적으로 컴파일
%.pic.o: %.c
	$(CC) $(OPTS) -fPIC -c $< -o $@

# 각 소스 파일을 개별적으로 컴파일하는 규칙 추가
open_pic.o: open.c
	$(CC) $(OPTS) -fPIC -c $< -o $@

close_pic.o: close.c
	$(CC) $(OPTS) -fPIC -c $< -o $@

put_pic.o: put.c
	$(CC) $(OPTS) -fPIC -c $< -o $@

get_pic.o: get.c
	$(CC) $(OPTS) -fPIC -c $< -o $@

skiplist_pic.o: skiplist.c
	$(CC) $(OPTS) -fPIC -c $< -o $@

# 객체 파일 삭제
clean:
	rm -f $(OBJS) $(LOBJS) $(LOBJS_PIC) $(LARS) $(LADYN) $(TARG) $(TARG_DYNAMIC)
