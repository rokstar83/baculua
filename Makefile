CXX=gcc
OPTS=-Wall -O2 -fPIC
SRC_DIR=src
BIN_DIR=bin

vpath %.c ${SRC_DIR}
vpath %.h ${SRC_DIR}

SRC_FILES=baculua.c
OBJS=$(patsubst %.c,${BIN_DIR}/%.o, ${SRC_FILES})
LDFLAGS=-llua
LUA_SO = baculua.so

.PHONY: clean tests all

all: baculua tests TAGS

TAGS: *.c *.h
	etags --members --declarations -l c $<

baculua: ${OBJS}
	${CXX} -shared -Wl,-soname,baculua.so -o ${BIN_DIR}/baculua.so ${OBJS}

${BIN_DIR}/%.o: %.c
	echo $<
	${CXX} ${OPTS} -c $< -o $@ ${LDFLAG}

tests:
	$(MAKE) -C tests

clean:
	rm ${BIN_DIR}/*
