/*
*  Camera.cpp
*  OpenGL Camera Class
* 
*  Christopher Root, 2006
*  Modifications by Donald House, 2009
*    corrected calculation of camera coordinates
*  Modifications by Tim Curtis, 2012
*    added alternate functionality for zoom using mouse wheel (Linux support)
*/

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Matrix.h"
#include <cstdlib>
#include <cstdio>
#include <cmath>

#define INACTIVE  0
#define TRANSLATE 1
#define ROTATE    2
#define ZOOM      3

using namespace std;

class Camera {
private:
  // all variables starting with 'Default' hold the initial camera values
  // these values are used if the camera is reset to its initial state
  Vector3d DefaultPos;
  Vector3d DefaultAim;
  Vector3d DefaultUp;
  
  double DefaultAzim;
  double DefaultElev;
  
  double CurrentAzim;
  double CurrentElev;
  
  void initialize();
  void computeCoords(const Vector3d &P, const Vector3d &A, const Vector3d &U);
  
public:
  Vector3d Pos;
  Vector3d Aim; 
  Vector3d Up;
  
  float nearPlane;
  float farPlane;
  float fov;
  
  // constructors
  
  // default constructor
  Camera();
  
  // constructor setting up camera orientation
  // P is position in 3D, A is the aim coordinate in 3D, U is the up vector
  Camera(Vector3d P, Vector3d A, Vector3d U);
  
  // constructor setting up camera orientation and view volume
  // P is position in 3D, A is aim coordinate in 3D, U is up vector
  // Near is near clipping plane, Far is far clipping plane, 
  // ViewAngle is field of view angle in degrees
  Camera(Vector3d P, Vector3d A, Vector3d U, 
	 float Near, float Far, float ViewAngle);
  
  // sets the clipping planes of the view volume
  void setClippingPlanes(float Near, float Far);
  
  // sets the FOV, ViewAngle should be in degrees
  void setFOV(float ViewAngle);	
  
  // set routines for Pos, Aim, and Up vector
  void setPos(Vector3d P);
  void setAim(Vector3d A);
  void setUp(Vector3d U);
  
  // reset the camera to its initial position
  void reset();
  
  // change camera aim position, and also move camera to same
  // relative position with respect to new aim position
  void setCenterOfFocus(Vector3d NewAim);
  
  // function to use the camera as the opengl camera
  // W and H are the width and height of the window
  void perspectiveDisplay(int W, int H, float aspect);
  
  // function that handles mouse events
  void handleMouseEvent(int button, int state, int x, int y);
  
  // function that handles mouse movements
  void handleMouseMotion(int x, int y);
  
  const Camera& operator=(const Camera& cam);
};

#endif

