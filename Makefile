CC = g++
TARGET = ./claw
ERRORS = errors.out
CFLAGS = -Wall -g
LFLAGS = -g
LIBS =  -lSDL2 -lSDL2_image
OBJ = main.o util.o level.o display.o miniz.o animate.o dtile.o game.o
all: $(TARGET)
%.o: %.cpp
	$(CC) $(CFLAGS) -c $^
$(TARGET): $(OBJ)
	$(CC) $(LFLAGS) $(OBJ) -o $@ $(LIBS)
clean:
	rm -f $(OBJ) $(TARGET)
memtest:
	valgrind --tool=memcheck --leak-check=full $(TARGET) > $(ERRORS) 2>&1
