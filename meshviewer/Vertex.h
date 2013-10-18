/*
*  Tim Curtis
*  CpSc 405
*  Dr. House
*  10/19/09
*  Project 5 - Object Viewer
*   Vertex.h
*
*  Header file for the Vertex class.
*/

#include "Matrix.h"

class Vertex {
public:
  Vertex() : x(0), y(0), z(0) {}
  ~Vertex() {}

  float x;
  float y;
  float z;
};
