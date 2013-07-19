CC = g++
TARGET = test
FLAGS = -Wall -g
LIBS = -lz -lSDL2 -lSDL2_image
OBJ = test.o util.o level.o display.o
all: $(TARGET)
%.o: %.cpp
	$(CC) $(FLAGS) -c $^
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LIBS)
clean:
	rm -f $(OBJ) $(TARGET)

