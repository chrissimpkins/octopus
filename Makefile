COMPILER = clang++
CXXFLAGS = -std=c++11 -stdlib=libc++ -Wall -g
EXTRAFLAGS = -pedantic -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused
OPTIMIZATION = -O
OBJECTS = main.o opts.o io.o

INSTALL_PATH?=/usr/local

all: oc

main.o: main.cpp main.h opts.h print.h
	${COMPILER} ${CXXFLAGS} ${EXTRAFLAGS} ${OPTIMIZATION} -c main.cpp

opts.o: opts.cpp opts.h print.h
	${COMPILER} ${CXXFLAGS} ${EXTRAFLAGS} ${OPTIMIZATION} -c opts.cpp

io.o: io.cpp io.h print.h
	${COMPILER} ${CXXFLAGS} ${EXTRAFLAGS} ${OPTIMIZATION} -c io.cpp

oc: ${OBJECTS}
	${COMPILER} ${CXXFLAGS} ${EXTRAFLAGS} ${OPTIMIZATION} ${OBJECTS} -o oc

install: oc
	cp -f oc ${INSTALL_PATH}/bin/oc