EXECUTABLE_PATH=..
INCLUDE_PATH=../headers/include
PROJECT_DEPENDENCY_PATH=../headers/dependencies
OUT_PATH=../bin

CXX = g++

CFLAGS = -O3 -I$(INCLUDE_PATH) -isystem$(PROJECT_DEPENDENCY_PATH) -std=c++11 -Wall
LFLAGS = 

TARGETS = main

all: $(TARGETS)

main: main.o occupancy_grid_map.o robot.o robot_utils.o project_utils.o cardinal_direction.o sign.o

$(TARGETS):
	mkdir -p $(OUT_PATH)
	$(CXX) -o $(EXECUTABLE_PATH)/$@ $(^:%.o=$(OUT_PATH)/%.o) $(CFLAGS) $(LFLAGS)


%.o: %.cpp
	$(CXX) -c $(CFLAGS) $< -o $(OUT_PATH)/$@


