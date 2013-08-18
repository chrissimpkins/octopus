COMPILER = clang++
CXXFLAGS = -std=c++11 -stdlib=libc++ -Wall
OPTIMIZATION = -O3
OBJECTS = main.o opts.o

INSTALL_PATH?=/usr/local

all: oc

main.o: main.cpp main.h opts.h print.h
	${COMPILER} ${CXXFLAGS} ${OPTIMIZATION} -c main.cpp

opts.o: opts.cpp opts.h print.h
	${COMPILER} ${CXXFLAGS} ${OPTIMIZATION} -c opts.cpp

oc: ${OBJECTS}
	${COMPILER} ${CXXFLAGS} ${OPTIMIZATION} ${OBJECTS} -o oc

install: oc
	cp -f oc ${INSTALL_PATH}/bin/oc