FLAGS=-std=c++17 -Wall -Wextra -pedantic
LIBS=-lgdi32
EXEC=DotaAcceptor.exe

BIN=./bin
SRC=./src
OBJ=./bin/obj
RC_FILE=./res/resources.rc

SOURCES := $(wildcard $(SRC)/*.cpp)
OBJECTS := $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SOURCES))
HEADERS := $(wildcard $(SRC)/*.h)

.PHONY:
all: directories $(RC_FILE) $(HEADERS) $(OBJECTS)
	g++ $(FLAGS) -o $(BIN)/$(EXEC) $(OBJECTS) $(OBJ)/resources.res $(LIBS)

$(OBJ)/%.o: $(SRC)/%.cpp
	g++ $(FLAGS) -I$(SRC) -c $^ -o $@ $(LIBS)

$(RC_FILE):
	windres $(RC_FILE) -O coff -o $(OBJ)/resources.res

.PHONY:
directories:
	mkdir -p $(OBJ)

.PHONY:
clean:
	rm -rv $(BIN)