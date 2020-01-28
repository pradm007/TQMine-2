CC		:= g++
C_FLAGS := -std=c++17 -Wall -Wextra -g

BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= lib
SO_FLAG	:= ldl
COMMAND_TRACEGEN := tracegen
COMMAND_MAIN := main

LIBRARIES	:= src/ragelGenerator.cpp src/tracePattern.cpp

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
	$(CC) $(C_FLAGS) -I$(INCLUDE) -L$(LIB) $(SRC)/$(EXECUTABLE).cpp -o $(BIN)/$(EXECUTABLE_TRACE) $(LIBRARIES)
	./$(BIN)/$(EXECUTABLE_TRACE) $(COMMAND_TRACEGEN)

main:
	$(CC) $(C_FLAGS) -I$(INCLUDE) -L$(LIB) $(SRC)/$(EXECUTABLE).cpp -$(SO_FLAG) -o $(BIN)/$(EXECUTABLE) $(LIBRARIES) -$(SO_FLAG)
	./$(BIN)/$(EXECUTABLE) $(COMMAND_MAIN)

run: all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CC) $(C_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -$(SO_FLAG) -o $@ $(LIBRARIES)