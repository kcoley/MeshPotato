# Tim Curtis
# CpSc 405
# Dr. House
# 10/19/09
# Project 5 - Object Viewer
#
#  Programmer: Donald House
#  Date: March 8, 1999
#
#  Copyright (C) - Donald H. House. 2005

CC = g++
C	= cpp
H	= h

CFLAGS 	= -g -Wall

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS     = -framework Foundation -framework GLUT -framework OpenGL -lm
else
  ifeq ("$(shell uname)", "Linux")
    LDFLAGS     = -lglut -lGL -lGLU -lm
  endif
endif

HFILES 	= GLViewer.${H} Face.${H} Material.${H} Vertex.${H} Matrix.${H} Vector.${H} Utility.${H}
OFILES 	= Matrix.o Vector.o Utility.o

PROJECT = objview

${PROJECT}:	GLViewer.o $(OFILES)
	${CC} $(CFLAGS) -o ${PROJECT} GLViewer.o $(OFILES) $(LDFLAGS)

GLViewer.o: GLViewer.${C} $(HFILES)
	${CC} $(CFLAGS) -c GLViewer.${C}

Matrix.o: Matrix.${C} Matrix.${H} Vector.${H} Utility.${H} 
	${CC} $(CFLAGS) -c Matrix.${C}

Vector.o: Vector.${C} Vector.${H} Utility.${H} 
	${CC} $(CFLAGS) -c Vector.${C}

Utility.o: Utility.${C} Utility.${H}
	${CC} $(CFLAGS) -c Utility.${C}

debug:
	make 'DFLAGS = /usr/lib/debug/malloc.o'

clean:
	rm *.o *~ ${PROJECT}

