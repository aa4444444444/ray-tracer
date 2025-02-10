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
source/triangle.cpp source/intersection.cpp source/radiance.cpp
source/light_source.cpp source/phong.cpp source/util.cpp
endef

# Header only files that we depend on
define DEPS
header/object.h header/constants.h header/illumination_model.h
endef

# Object files
OBJS = $(SRCS:.cpp=.o)

# This determines how to handle make clean
# Differentiates between windows vs linux systems
ifdef OS
   RM = del /Q
   FixPath = $(subst /,\,$1)
else
   ifeq ($(shell uname), Linux)
      RM = rm -f
      FixPath = $1
   endif
endif

# Default rule to build and run the executable
all: $(TARGET)

# Rule to link object files into the target executable
$(TARGET): $(OBJS) $(call FixPath, $(DEPS))
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to run the executable
run: $(TARGET)
	$(TARGET)

# Clean rule to remove generated files
clean:
	$(RM) $(call FixPath, $(TARGET_DEL)) $(call FixPath, $(OBJS))