/*
*  Tim Curtis
*  MeshViewer.h
*
*  Header file for the MeshViewer class.
*/

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <time.h>
#include <OpenImageIO/imageio.h>
#include "Face.h"
#include "Camera.h"

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GLUT/glut.h>
#else
#  include <GL/glew.h>
#  include <GL/glut.h>
#endif

#define POINTS    0
#define WIREFRAME 1
#define SHADED    2

using namespace std;
OIIO_NAMESPACE_USING

// Initial Window Dimesions
const int WIDTH = 1280;
const int HEIGHT = 720;
const float ASPECT = 16.0/9.0;

// Container for pixel values
typedef float RGBA[4];

class MeshViewer {
public:
  // Initializing constant values relative to the class
  MeshViewer() { 
    minX = maxX = minY = maxY = minZ = maxZ = 0;
    Width = WIDTH;  Height = HEIGHT; Aspect = ASPECT;
  }
  ~MeshViewer() {
    destructArrays();
  }

  void destructArrays() {
    if(vertTris  != NULL) delete [] vertTris;
    if(vertQuads != NULL) delete [] vertQuads;
    if(normTris  != NULL) delete [] normTris;
    if(normQuads != NULL) delete [] normQuads;
    if(texTris   != NULL) delete [] texTris;
    if(texQuads  != NULL) delete [] texQuads;
  }

  void initMeshPotato();
  void setInitialState();
  void updateProjection();
  void drawAxes(float);
  
  // Routines for the drawing the model
  void drawModel();
  void readFile(char*);
  void readMaterialFile(string);
  int  findMatIndex(string);
  void buildVBOs();
  void turntable();
  void writeFile(string basename, string ext, int frame=1);

  // Callback functions used in the event loops
  void doDisplay();
  void handleKey(unsigned char, int, int);
  void doReshape(int, int);
  void initialize(char*);

private:
  // Kernel for Mesh Potato
  // MyEngine::Kernel mpkernel;

  // Values to calculate center of of the object
  float minX;
  float maxX;
  float minY;
  float maxY;
  float minZ;
  float maxZ;

  // Viewing parameters
  float Depth;
  float angle;

  // Booleans for toggle keys
  float axisLength;
  bool Axes;

  // Variable for switching between points, wireframe and shaded
  unsigned int shadingMode;
  int mode;

  // Viewport dimensions
  int Width;
  int Height;
  double Aspect;

  // VBO variables
  unsigned int vertQuadsVBO;
  unsigned int vertTrisVBO;
  unsigned int normQuadsVBO;
  unsigned int normTrisVBO;
  unsigned int texQuadsVBO;
  unsigned int texTrisVBO;

  unsigned int triCount;
  unsigned int quadCount;
  float *vertQuads;
  float *vertTris;
  float *normQuads;
  float *normTris;
  float *texQuads;
  float *texTris;

  // vectors to hold the vertices and faces of the model
  vector<Vertex> vertices;
  vector<Vertex> vertNorms;
  vector<Vertex> vertTex;
  list<Face> faces;
  vector<Material> mats;

  // Used as a buffer to store pixels read from glReadPixels
  RGBA *pixmap;
};
