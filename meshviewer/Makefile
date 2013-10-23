ifeq ("$(shell whoami)", "kcoley")
  CXX = clang++
else
  CXX = g++
endif

# Warnings frequently signal eventual errors:
CXXFLAGS=`sdl-config --cflags` -g -W -Wall -Weffc++ -Wextra -pedantic -O0 -I ../include

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS = -framework Foundation -framework GLUT -framework OpenGL -lm
else
  ifeq ("$(shell uname)", "Linux")
  LDFLAGS = `sdl-config --libs` -lm -lSDL_ttf -lSDL_image -lexpat -lX11 -lGL -lGLU -lglut -lGLEW -lmeshpotato -lopenvdb -lboost_system -lboost_filesystem
  endif
endif

OBJS = \
	GLViewer.o
EXEC = meshviewer

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

GLViewer.o: GLViewer.cpp

clean:
	rm -rf $(OBJS)
	rm -rf $(EXEC)
