COMPILER = clang++
COMPILER_GPP = g++
CXXFLAGS = -std=c++11 -stdlib=libc++ -Wall -g
CXXFLAGS_GPP = -std=c++11 -Wall -g
DEBUGFLAGS = -pedantic -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused
OPTIMIZATION = -O3
OBJECTS = src/main.o src/opts.o src/io.o
OBJECTS_GPP = src/main_gpp.o src/opts_gpp.o src/io_gpp.o

INSTALL_PATH?=/usr/local

all: oc

main.o: src/main.cpp src/main.h src/opts.h src/print.h
	${COMPILER} ${CXXFLAGS} ${OPTIMIZATION} -c src/main.cpp

opts.o: src/opts.cpp src/opts.h src/print.h
	${COMPILER} ${CXXFLAGS} ${OPTIMIZATION} -c src/opts.cpp

io.o: src/io.cpp src/io.h src/print.h
	${COMPILER} ${CXXFLAGS} ${OPTIMIZATION} -c src/io.cpp

oc: ${OBJECTS}
	${COMPILER} ${CXXFLAGS} ${OPTIMIZATION} ${OBJECTS} -o src/oc

gpp: ocgpp

main_gpp.o: src/main.cpp src/main.h src/opts.h src/print.h
	${COMPILER_GPP} ${CXXFLAGS_GPP} ${OPTIMIZATION} -c src/main.cpp

opts_gpp.o: src/opts.cpp src/opts.h src/print.h
	${COMPILER_GPP} ${CXXFLAGS_GPP} ${OPTIMIZATION} -c src/opts.cpp

io_gpp.o: src/io.cpp src/io.h src/print.h
	${COMPILER_GPP} ${CXXFLAGS_GPP} ${OPTIMIZATION} -c src/io.cpp

ocgpp: ${OBJECTS_GPP}
	${COMPILER_GPP} ${CXXFLAGS_GPP} ${OPTIMIZATION} ${OBJECTS} -o src/oc

install: oc
	cp -f src/oc ${INSTALL_PATH}/bin/oc