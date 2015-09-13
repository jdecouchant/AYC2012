# name of the file to build
EXE = datagen

# compilation options
LDFLAGS = -fopenmp -O3 -fomit-frame-pointer
CPPFLAGS = -W -Wall -ansi -fopenmp -O3 -fomit-frame-pointer

# source files suffix (all source files must have the same suffix)
SOURCE_SUFFIX = cpp

# setting the compiler

CXX = g++

# source files in this project
SRC := $(wildcard src/*.cpp)

# object files in this project
OBJ := $(SRC)
OBJ := $(OBJ:src/.$(SOURCE_SUFFIX)=obj/.o)

all: $(EXE)

install: all
	cp $(EXE) ../testdata

$(EXE): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^
	
%.o: %.c
	$(CXX) $(CPPFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o $(EXE)