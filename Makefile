# Compiler
CXX = g++

# Eigen Library Location
EIGEN = ../eigen-3.4.0

# Compiler flags
CXXFLAGS = -Wall -g -I $(EIGEN)

# Target executable
TARGET = main

# For deleting the target
TARGET_DEL = main.exe

# Source files
define SRCS 
source/main.cpp source/color.cpp source/point.cpp source/vector.cpp 
source/ray.cpp source/sphere.cpp source/world.cpp source/camera.cpp
endef

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default rule to build and run the executable
all: $(TARGET)

# Rule to link object files into the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to run the executable
run: $(TARGET)
	$(TARGET)

# Clean rule to remove generated files
clean:
	del $(TARGET_DEL) $(OBJS)