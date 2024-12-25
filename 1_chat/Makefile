CXX = g++
CXXFLAGS = -Wall -std=c++17 -I./include -I../common/include
LDFLAGS = -L../common -lcommon -pthread

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SOURCES = $(SRC_DIR)/client.cpp main.cpp

OBJECTS = $(SOURCES:%.cpp=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/chat_client

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)/$(SRC_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR) $(OBJ_DIR)/$(SRC_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
