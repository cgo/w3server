# Generated automatically from Makefile.in by configure.
# Basic makefile. Takes all .c files in src/ directory and compiles them into one executable $(TARGET).
# Author: Christian Gosch

CC     = gcc
CCARGS = -c -g -Wall -pedantic
LD     = gcc
LDARGS = -g

DEFINES = -DHAVE_CONFIG_H -DNO_FLEX -D_REENTRANT -D_MT

CLIENTDIR := ../client
# According to the manpage on SunOS, libnsl is threadsafe WITH EXCEPTIONS .. 
# I don't know what that means (Sun ....)
LIBS = -lnsl -lpthread 
INCLUDE = -I./include -I$(CLIENTDIR)/include

TARGET = server

.c.o:
	$(CC) $(CCARGS) $(INCLUDE) $(DEFINES) -o $@ $<


CLIENTSRC := $(CLIENTDIR)/src/w3thread.c $(CLIENTDIR)/src/w3error.c $(CLIENTDIR)/src/w3client.c $(CLIENTDIR)/src/w3comm.c $(CLIENTDIR)/src/w3string.c $(CLIENTDIR)/src/w3uri.c
# CLIENTDIRS := $(CLIENTDIR)/src/.
# CLIENTSRC := $(foreach DIR,$(CLIENTDIRS),$(wildcard $(DIR)/*.c))
DIRS   := .
SRC    := $(foreach DIR,$(DIRS),$(wildcard src/$(DIR)/*.c)) $(CLIENTSRC)
OBJS   := $(subst .c,.o,$(SRC))
REMOVE := $(foreach DIR,$(DIRS),$(wildcard src/$(DIR)/*~) $(wildcard include/$(DIR)/*~) $(wildcard ./*~))

ALL: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(LDARGS) $(LIBS) -o $(TARGET) $(OBJS)

clean:
	rm -rf $(OBJS)
	rm -rf $(REMOVE)

stat:
	wc -l $(SRC)




