# Generated automatically from Makefile.in by configure.
# Basic makefile. Takes all .c files in src/ directory and compiles them into one executable $(TARGET).
# Author: Christian Gosch

CC     = gcc
CCARGS = -c -g -Wall -pedantic
LD     = gcc
LDARGS = -g

DEFINES = -DHAVE_CONFIG_H -D_REENTRANT -D_MT

LIBS = -lnsl -lpthread 
INCLUDE = -I./include

TARGETDIR := $(shell pwd)


TARGET = client

.c.o:
	$(CC) $(CCARGS) $(INCLUDE) $(DEFINES) -o $@ $<

DIRS   := .
SRC    := $(foreach DIR,$(DIRS),$(wildcard src/$(DIR)/*.c))
OBJS   := $(subst .c,.o,$(SRC))
REMOVE := $(foreach DIR,$(DIRS),$(wildcard src/$(DIR)/*~) $(wildcard include/$(DIR)/*~) $(wildcard ./*~))

ALL: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(LDARGS) $(LIBS) -o $(TARGET) $(OBJS) -lfl
	rm -f ../browser/client
	# ln -s $(TARGETDIR)/$(TARGET) ../browser/client


correct: utils/lex.yy.c
	gcc utils/lex.yy.c -o correct -lfl

clean:
	rm -rf $(OBJS)
	rm -rf $(REMOVE)

stat:
	wc -l $(SRC)













