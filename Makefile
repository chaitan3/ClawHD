#CC = clang++
CC = g++
CFLAGS = -Wall -g -std=c++11
LFLAGS = -g
LIBS =  -lSDL2 -lSDL2_image -lSDL2_mixer

OBJ = util.o level.o display.o miniz.o animate.o dtile.o sound.o physics.o
TARGET = ./claw
TARGETOBJ = main.o $(OBJ)
ERRORS = errors.out

all: $(TARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $^

$(TARGET): $(TARGETOBJ)
	$(CC) $(LFLAGS) $^ -o $@ $(LIBS)

test: test.o $(OBJ)
	$(CC) $(LFLAGS) $^ -o $@ $(LIBS)

clean:
	rm -f $(TARGETOBJ) $(TARGET)

memtest:
	valgrind --tool=memcheck --leak-check=full $(TARGET) > $(ERRORS) 2>&1
