# $Id: Makefile,v 1.8 2015-4-02 17:37:02-07 - - $

GPP = g++ -g -O0 -Wall -Wextra -std=gnu++11
MKDEP = g++ -MM -std=gnu++11

CHECKINS  = main.cpp stringset.h auxlib.h auxlib.cpp 
DEPFILE = Makefile.deps

all : main.o stringset.o auxlib.o
	${GPP} main.o stringset.o auxlib.o -o oc

main.o : main.cpp stringset.h auxlib.h
	${GPP} -c main.cpp

stringset.o : stringset.cpp stringset.h
	${GPP} -c stringset.cpp

auxlib.o : auxlib.cpp auxlib.h
	${GPP} -c auxlib.cpp

clean :
	rm auxlib.o stringset.o main.o

spotless : clean
	rm oc

submit : 
	submit cmps104a-wm.s15 asg1 main.cpp stringset.h stringset.cpp auxlib.h auxlib.cpp README Makefile
ci :
	cid + ${CHECKINS}
	checksource ${CHECKINS}
	

${DEPFILE} :
	${MKDEP} main.cpp stringset.cpp auxlib.cpp >${DEPFILE}

deps : 
	- rm ${DEPFILE}
	${MAKE} --no-print-directory ${DEPFILE}

include Makefile.deps
