SRC_DIR = src/
SRC_FILES = main.cpp renderer.cpp shader.cpp mesh.cpp model.cpp mouse.cpp callbacks.cpp camera.cpp glm/operators.cpp
CFLAGS=-Wall -Wextra -o a.out -g -std=c++14 #-Werror 
INCFLAGS=-I/usr/include/glm
LIBFLAGS=-lGL -lglfw -lGLEW

SOURCES = $(addprefix $(SRC_DIR), $(SRC_FILES))

all:
	g++ $(CFLAGS) $(INCFLAGS) $(SOURCES) $(LIBFLAGS)

clean:
	rm a.out -f

run:
	@make clean
	@make all
	@echo "+---------+"
	@echo "| Running |"
	@echo "+---------+"
	@./a.out