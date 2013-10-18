/*
*  Tim Curtis
*  CpSc 405
*  Dr. House
*  10/19/09
*  Project 5 - Object Viewer
*   GLViewer.h
*
*  Header file for the GLViewer class.
*/

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <vector>
#include <fstream>
#include <istream>
#include <sstream>
#include "Face.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

// Initial Window Dimesions
const int WIDTH = 1280;
const int HEIGHT = 720;
const float ASPECT = 1.778;

// Container for pixel values
typedef unsigned char RGBA[4];

using namespace std;

class GLViewer {
public:
  // Initializing constant values relative to the class
  GLViewer() : ORTHO(0), PERSPECTIVE(1), NONE(-1), NEAR(0.1), FAR(5000), 
     YANGLE(40), ROTFACTOR(0.2), XLATEFACTOR(0.1) 
     { wireframe = TRUE;  minX = maxX = minY = maxY = minZ = maxZ = 0;
       Width = WIDTH;  Height = HEIGHT; }
  ~GLViewer() {}

  void setInitialState();
  void updateProjection();
  void drawAxes(float);
  
  // Routines for the drawing the model
  void drawModel();
  void readFile(char*);
  void readMaterialFile(string);
  int  findMatIndex(string);
  void writeFile();

  // The 5 Callback functions used in the event loops
  void doDisplay();
  void handleKey(unsigned char, int, int);
  void handleButtons(int, int, int, int);
  void handleMotion(int, int);
  void doReshape(int, int);
  void initialize(char*);

  const int ORTHO;	             // projection system codes
  const int PERSPECTIVE;
  const int NONE;               // used to indicate no mouse button pressed

  const float NEAR;             // distance of near clipping plane
  const float FAR;	             // distance of far clipping plane

  const int YANGLE;

  const float ROTFACTOR;     // degrees rotation per pixel of mouse movement
  const float XLATEFACTOR;   // units of translation per pixel of mouse movement

private:
  // Values to calculate center of of the object
  float minX;
  float maxX;
  float minY;
  float maxY;
  float minZ;
  float maxZ;

  // Viewing parameters
  int Projection;
  int Depth;

  // Booleans for toggle keys
  float axisLength;
  bool Axes;
  bool wireframe;

  // Variable for switching between wireframe and shaded
  int mode;

  // Camera position and orientation
  double Pan;
  double Tilt;
  double Approach;

  // model orientation
  double ThetaX;
  double ThetaY;

  // Viewport dimensions
  int Width;
  int Height;

  // variables to track mouse and shift key
  int MouseX;
  int MouseY;
  int Button;

  // vectors to hold the vertices and faces of the model
  vector<Vertex> vertices;
  vector<Vertex> vertNorms;
  vector<Vertex> vertTex;
  list<Face> faces;
  vector<Material> mats;

  // Used as a buffer to store pixels read from glReadPixels
  RGBA *pixmap;
};
