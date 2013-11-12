/*
*  Tim Curtis
*  MeshViewer.h
*
*  Header file for the MeshViewer class.
*/

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <vector>
#include <fstream>
#include <istream>
#include <sstream>
#include "Face.h"
#include "Camera.h"

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GLUT/glut.h>
#else
#  include <GL/glew.h>
#  include <GL/glut.h>
#endif

// Initial Window Dimesions
const int WIDTH = 1280;
const int HEIGHT = 720;
const float ASPECT = 16.0/9.0;

// Container for pixel values
typedef unsigned char RGBA[4];

using namespace std;

class MeshViewer {
public:
  // Initializing constant values relative to the class
  MeshViewer() { 
    wireframe = TRUE;  minX = maxX = minY = maxY = minZ = maxZ = 0;
    Width = WIDTH;  Height = HEIGHT; Aspect = ASPECT;
  }
  ~MeshViewer() {
    if(verts != NULL) delete [] verts;
    if(norms != NULL) delete [] norms;
    if(tex   != NULL) delete [] tex;  
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
  void writeFile();

  // Callback functions used in the event loops
  void doDisplay();
  void handleKey(unsigned char, int, int);
  void doReshape(int, int);
  void initialize(char*);

private:
  // Kernel for Mesh Potato
//  MyEngine::Kernel mpkernel;
  // Values to calculate center of of the object
  float minX;
  float maxX;
  float minY;
  float maxY;
  float minZ;
  float maxZ;

  // Viewing parameters
  float Depth;

  // Booleans for toggle keys
  float axisLength;
  bool Axes;
  bool wireframe;

  // Variable for switching between wireframe and shaded
  int mode;

  // Viewport dimensions
  int Width;
  int Height;
  double Aspect;

  // VBO variables
  /*
  unsigned int vertsVBO;
  unsigned int normsVBO;
  unsigned int texVBO;
  unsigned int texID;
  unsigned int meshVAO;
  */
  unsigned int vertCount;
  unsigned int polySize;
  float *verts;
  float *norms;
  float *tex;

  // vectors to hold the vertices and faces of the model
  vector<Vertex> vertices;
  vector<Vertex> vertNorms;
  vector<Vertex> vertTex;
  list<Face> faces;
  vector<Material> mats;

  // Used as a buffer to store pixels read from glReadPixels
  RGBA *pixmap;
};
