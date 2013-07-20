CC = g++
TARGET = test
CFLAGS = -Wall -g
LFLAGS = -g
LIBS =  -lSDL2 -lSDL2_image
OBJ = test.o util.o level.o display.o miniz.o animate.o
all: $(TARGET)
%.o: %.cpp
	$(CC) $(CFLAGS) -c $^
$(TARGET): $(OBJ)
	$(CC) $(LFLAGS) $(OBJ) -o $@ $(LIBS)
clean:
	rm -f $(OBJ) $(TARGET)

