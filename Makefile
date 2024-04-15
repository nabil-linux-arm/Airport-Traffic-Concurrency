# Reference: https://makefiletutorial.com/#getting-started

SRC_DIRS := ./src
BUILD_DIR := ./build
TARGET_EXEC := airport-traffic.out

CXX := g++

# Find all the C++ files we want to compile
SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.s') # .s is static file
# Prepends BUILD_DIR and appends .o to every src file

# OBJS := $(patsubst $(SRC_DIRS)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
OBJS := $(filter-out $(BUILD_DIR)/main.o, $(patsubst $(SRC_DIRS)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))) 
MAIN := $(BUILD_DIR)/main.o


VPATH = src

all: $(BUILD_DIR)/$(TARGET_EXEC)

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS) $(MAIN)
	$(CXX) $^ -o $@

$(BUILD_DIR)/main.o: main.cpp
	$(CXX) $< -c -o $@ -std=c++11

$(BUILD_DIR)/%.o: %.cpp %.hpp
	@echo Buidling $@
	$(CXX) -c $< -o $@ -std=c++11


$(OBJS): | $(BUILD_DIR)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

clean:
	rm -r $(BUILD_DIR)
.PHONY: clean