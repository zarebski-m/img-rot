CC=g++
CFLAGS=-Wall -Wextra -pedantic -std=c++0x -O2
LDFLAGS=-lpng -lz

SRCS=ImgRotate.cpp PngReader.cpp Image.cpp ImageComparer.cpp
OBJS=$(SRCS:.cpp=.o)

EXE=img.exe

all: $(SRCS) $(EXE)
	
$(EXE): $(OBJS)
	$(CC) $(OBJS) -o out/$@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@
