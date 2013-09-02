COMPILER = clang++
COMPILER_GPP = g++
CXXFLAGS = -std=c++11 -stdlib=libc++ -Wall -g
CXXFLAGS_GPP = -std=c++11 -Wall -g
DEBUGFLAGS = -pedantic -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused
OPTIMIZATION = -O3
OBJECTS = main.o opts.o io.o
OBJECTS_GPP = main_gpp.o opts_gpp.o io_gpp.o

INSTALL_PATH?=/usr/local

all: oc

main.o: main.cpp main.h opts.h print.h
	${COMPILER} ${CXXFLAGS} ${OPTIMIZATION} -c main.cpp

opts.o: opts.cpp opts.h print.h
	${COMPILER} ${CXXFLAGS} ${OPTIMIZATION} -c opts.cpp

io.o: io.cpp io.h print.h
	${COMPILER} ${CXXFLAGS} ${OPTIMIZATION} -c io.cpp

oc: ${OBJECTS}
	${COMPILER} ${CXXFLAGS} ${OPTIMIZATION} ${OBJECTS} -o oc

gpp: ocgpp

main_gpp.o: main.cpp main.h opts.h print.h
	${COMPILER_GPP} ${CXXFLAGS_GPP} ${OPTIMIZATION} -c main.cpp

opts_gpp.o: opts.cpp opts.h print.h
	${COMPILER_GPP} ${CXXFLAGS_GPP} ${OPTIMIZATION} -c opts.cpp

io_gpp.o: io.cpp io.h print.h
	${COMPILER_GPP} ${CXXFLAGS_GPP} ${OPTIMIZATION} -c io.cpp

ocgpp: ${OBJECTS_GPP}
	${COMPILER_GPP} ${CXXFLAGS_GPP} ${OPTIMIZATION} ${OBJECTS} -o oc

install: oc
	cp -f oc ${INSTALL_PATH}/bin/oc