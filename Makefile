SRC_DIRS := ./src
BUILD_DIR := ./build
TARGET_EXEC := airport-traffic.out

CXX := g++

# Find all the C++ files we want to compile
SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.s') # .s is static file
# Prepends BUILD_DIR and appends .o to every src file

OBJS := $(patsubst $(SRC_DIRS)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
# OBJS := $(filter-out main.o, $(patsubst $(SRC_DIRS)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))) 


VPATH = src

$(BUILD_DIR)/%.o: %.cpp %.hpp
	@echo Buidling $@
	$(CXX) -c $< -o $@ -std=c++11

all: | $(OBJS)

$(BUILD_DIR)/main.o: main.cpp
	$(CXX) $< -o $@ -std=c++11

$(OBJS): | $(BUILD_DIR)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

clean:
	rm -r $(BUILD_DIR)
.PHONY: clean