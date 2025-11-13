#
# File          : Makefile
# Description   : Build file for CS 165 project 1
#                 


# Environment Setup
#LIBDIRS=-L. -L/opt/local/lib
#INCLUDES=-I. -I/opt/local/include
#CC=gcc 
CC=clang
CFLAGS=-c $(INCLUDES) -g -Wall
LINK=clang -g
LDFLAGS=$(LIBDIRS)
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

BASENAME=p1-handout
tar: 
	tar cvfz $(BASENAME).tgz -C ..\
	    $(BASENAME)/Makefile \
            $(BASENAME)/cs165-obj.c \
	    $(BASENAME)/cs165-obj.h \
	    $(BASENAME)/test1 \
	    $(BASENAME)/test2 \
	    $(BASENAME)/test3 \
	    $(BASENAME)/test4 \
	    $(BASENAME)/test5 \
	    $(BASENAME)/test6 \
	    $(BASENAME)/out1 \
	    $(BASENAME)/out2 \
	    $(BASENAME)/out3 \
	    $(BASENAME)/out4 \
	    $(BASENAME)/out5 \
	    $(BASENAME)/out6 \
	    $(BASENAME)/example-out1\
	    $(BASENAME)/example-out2 \
	    $(BASENAME)/example-out3 \
	    $(BASENAME)/example-out4 \
	    $(BASENAME)/example-out5 \
	    $(BASENAME)/example-out6 \


