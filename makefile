FLAGS=-std=c++17 -Wall -Wextra -pedantic
BIN=./bin
EXEC=DotaAcceptor.exe
SRC=./src
OBJ=./bin/obj

SOURCES := $(wildcard $(SRC)/*.cpp)
OBJECTS := $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SOURCES))
HEADERS := $(wildcard $(SRC)/*.h)

.PHONY:
all: directories $(HEADERS) $(OBJECTS)
	g++ $(FLAGS) -o $(BIN)/$(EXEC) $(OBJECTS)

$(OBJ)/%.o: $(SRC)/%.cpp
	g++ $(FLAGS) -I$(SRC) -c $^ -o $@

.PHONY:
directories:
	mkdir -p $(OBJ)

.PHONY:
clean:
	rm -rv $(BIN)