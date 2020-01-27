CC		:= g++
C_FLAGS := -std=c++17 -Wall -Wextra -g

BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= lib
SO_FLAG	:= ldl

LIBRARIES	:=

ifeq ($(OS),Windows_NT)
EXECUTABLE	:= main.exe
EXECUTABLE_TRACE	:= generateTrace.exe
else
EXECUTABLE	:= main
EXECUTABLE_TRACE	:= generateTrace
endif

all: $(BIN)/$(EXECUTABLE)

clean:
	$(RM) $(BIN)/$(EXECUTABLE)
	$(RM) $(BIN)/$(EXECUTABLE_TRACE)

tracegen:
	$(CC) $(C_FLAGS) -I$(INCLUDE) -L$(LIB) $(SRC)/$(EXECUTABLE_TRACE).cpp -o $(BIN)/$(EXECUTABLE_TRACE)
	./$(BIN)/$(EXECUTABLE_TRACE)

run: all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CC) $(C_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -$(SO_FLAG) -o $@ $(LIBRARIES)