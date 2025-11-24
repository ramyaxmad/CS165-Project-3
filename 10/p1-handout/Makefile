#
# File          : Makefile
# Description   : Build file for CS 165 project 1
#                 


# Environment Setup
#LIBDIRS=-L. -L/opt/local/lib
#INCLUDES=-I. -I/opt/local/include
#CC=gcc 
CC=clang
CFLAGS=-c $(INCLUDES) -g -Wall -m32 -ffreestanding -fno-stack-protector
LINK=clang -g
LDFLAGS=$(LIBDIRS) -m32 -ffreestanding
AR=ar rc
RANLIB=ranlib

# Suffix rules
.c.o :
	${CC} ${CFLAGS} $< -o $@

#
# Setup builds

TARGETS=cs165-obj
#LIBS=-lcrypto -lm
LIBS=

#
# Project Protections

p1 : $(TARGETS)

cs165-obj : cs165-obj.o
	$(LINK) $(LDFLAGS) cs165-obj.o $(LIBS) -o $@

clean:
	rm -f *.o *~ $(TARGETS)

BASENAME=p2-handout
tar: 
	tar cvfz $(BASENAME).tgz -C ..\
	    $(BASENAME)/Makefile \
            $(BASENAME)/cs165-obj.c \
	    $(BASENAME)/cs165-obj.h \
	    $(BASENAME)/test.disclosure \
	    $(BASENAME)/test.confusion \
	    $(BASENAME)/test.temporal \
	    $(BASENAME)/test.overflow \
	    $(BASENAME)/out.disclosure \
	    $(BASENAME)/out.confusion \
	    $(BASENAME)/out.temporal \
	    $(BASENAME)/out.overflow \


