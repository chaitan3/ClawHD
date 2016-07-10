#CC = clang++
CC = g++
CFLAGS = -Wall -g -std=c++11
LFLAGS = -g
LIBS =  -lSDL2 -lSDL2_image -lSDL2_mixer
SRC = src
OBJ = build
SOURCES = $(notdir $(wildcard $(SRC)/*.cpp))
OBJECTS = $(addprefix $(OBJ)/,$(SOURCES:%.cpp=%.o))
TARGET = ./claw
ERRORS = errors.out

all: $(TARGET)


$(TARGET): $(OBJ) $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o $@ $(LIBS)

$(OBJ):
	mkdir $(OBJ)

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -rf $(OBJECTS) $(OBJ) $(TARGET)

memtest:
	valgrind --tool=memcheck --leak-check=full $(TARGET) > $(ERRORS) 2>&1
