HOST?=UNIX
HOST_DIR?= $(shell echo $(HOST) | tr A-Z a-z)

include $(HOST_DIR)/host.mk

# -O3 -g
# -std=gnu11
CFLAGS+=-Wall -g -std=gnu11 -pedantic -D$(HOST)_HOST -I$(HOST_DIR) -DVER=\"`git show-ref --abbrev=8 --head --hash head`\" -DTAG=\"`git describe --abbrev=0 --tags`\"

EMBEDDED = cstdlib/string.c cstdlib/stdlib.c cstdlib/errno.c cstdlib/ctype.c cstdlib/stdbool.c
STDLIB ?= cstdlib/stdio.c cstdlib/math.c cstdlib/string.c cstdlib/stdlib.c \
	cstdlib/time.c cstdlib/errno.c cstdlib/ctype.c cstdlib/stdbool.c


SRCS	+= picoc.c table.c lex.c parse.c expression.c heap.c type.c \
	variable.c clibrary.c platform.c include.c debug.c $(STDLIB) \
	$(HOST_DIR)/platform.c $(HOST_DIR)/library.c
OBJS	:= $(SRCS:%.c=%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBS) -o $(TARGET)

test:	all
	@(cd tests; make -s test)
	@(cd tests; make -s csmith)
	@(cd tests; make -s jpoirier)

clean:
	rm -f $(TARGET) $(OBJS) *~

count:
	@echo "Core:"
	@cat picoc.h interpreter.h picoc.c table.c lex.c parse.c expression.c platform.c heap.c type.c variable.c include.c debug.c | grep -v '^[ 	]*/\*' | grep -v '^[ 	]*$$' | wc
	@echo ""
	@echo "Everything:"
	@cat $(SRCS) *.h */*.h | wc

.PHONY: clibrary.c

LICENSE.h: LICENSE
	xxd -i LICENSE | sed -e 1s/LICENSE/__LICENSE/ -e 's/}/,0x00}/' -e /LICENSE_len/d > LICENSE.h

picoc.o: picoc.c picoc.h LICENSE.h
table.o: table.c interpreter.h system.h $(HOST_DIR)/platform.h
lex.o: lex.c interpreter.h system.h $(HOST_DIR)/platform.h
parse.o: parse.c picoc.h interpreter.h system.h $(HOST_DIR)/platform.h
expression.o: expression.c interpreter.h system.h $(HOST_DIR)/platform.h
heap.o: heap.c interpreter.h system.h $(HOST_DIR)/platform.h
type.o: type.c interpreter.h system.h $(HOST_DIR)/platform.h
variable.o: variable.c interpreter.h system.h $(HOST_DIR)/platform.h
clibrary.o: clibrary.c picoc.h interpreter.h system.h $(HOST_DIR)/platform.h
platform.o: platform.c picoc.h interpreter.h system.h $(HOST_DIR)/platform.h
include.o: include.c picoc.h interpreter.h system.h $(HOST_DIR)/platform.h
debug.o: debug.c interpreter.h system.h $(HOST_DIR)/platform.h
platform/platform_unix.o: platform/platform_unix.c picoc.h interpreter.h system.h $(HOST_DIR)/platform.h
platform/library_unix.o: platform/library_unix.c interpreter.h system.h $(HOST_DIR)/platform.h
cstdlib/stdio.o: cstdlib/stdio.c interpreter.h system.h $(HOST_DIR)/platform.h
cstdlib/math.o: cstdlib/math.c interpreter.h system.h $(HOST_DIR)/platform.h
cstdlib/string.o: cstdlib/string.c interpreter.h system.h $(HOST_DIR)/platform.h
cstdlib/stdlib.o: cstdlib/stdlib.c interpreter.h system.h $(HOST_DIR)/platform.h
cstdlib/time.o: cstdlib/time.c interpreter.h system.h $(HOST_DIR)/platform.h
cstdlib/errno.o: cstdlib/errno.c interpreter.h system.h $(HOST_DIR)/platform.h
cstdlib/ctype.o: cstdlib/ctype.c interpreter.h system.h $(HOST_DIR)/platform.h
cstdlib/stdbool.o: cstdlib/stdbool.c interpreter.h system.h $(HOST_DIR)/platform.h
cstdlib/unistd.o: cstdlib/unistd.c interpreter.h system.h $(HOST_DIR)/platform.h
