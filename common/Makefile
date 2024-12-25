CXX = g++
CXXFLAGS = -Wall -std=c++17 -I./include

SRC_DIR = src
OBJ_DIR = obj

SOURCES = $(SRC_DIR)/socket_error.cpp \
	$(SRC_DIR)/socket.cpp \
	$(SRC_DIR)/udp_socket.cpp \
	$(SRC_DIR)/tcp_socket.cpp

OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TARGET = libcommon.a

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@ar rcs $@ $(OBJECTS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)