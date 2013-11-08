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

#include "Camera.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define ZOOMIN 0.1
#define ZOOMOUT -0.1

using namespace std;

double DeltaAzim;
double DeltaElev;
double LocalDeltaAzim;
double LocalDeltaElev;

int MouseStartX;
int MouseStartY;
int MousePrevX;
int MousePrevY;

const float epsilon = 0.0001;

GLdouble MvMatrix[16];
GLdouble ProjMatrix[16];
GLint ViewPort[4];

int CameraMode = INACTIVE;

Vector3d PrevMousePos;

//
// Rotation routines - regular functions, not methods
//

void RotateX(Vector3d &v, double degree){
  double c = cos(DegToRad(degree));
  double s = sin(DegToRad(degree));
  double v1 = v[1] * c - v[2] * s;
  double v2 = v[1] * s + v[2] * c;
  v[1] = v1; v[2] = v2;
}

void RotateY(Vector3d &v, double degree){
  double c = cos(DegToRad(degree));
  double s = sin(DegToRad(degree));
  double v0 = v[0] * c + v[2] * s;
  double v2 = -v[0] * s + v[2] * c;
  v[0] = v0; v[2] = v2;
}

/* 
 * ArbitraryRotate() - rotate around an arbitrary coordinate system specified by
 *                     U, V, & W
 */
void ArbitraryRotate(Vector3d U, Vector3d V, Vector3d W, double degreeX, double degreeY, Vector3d& point, Vector3d aim) {
  double cx = cos(DegToRad(degreeX));
  double sx = sin(DegToRad(degreeX));
  double cy = cos(DegToRad(degreeY));
  double sy = sin(DegToRad(degreeY)); 
  
  Matrix4x4 trans(1, 0, 0, -aim[0],
		  0, 1, 0, -aim[1],
		  0, 0, 1, -aim[2],
		  0, 0, 0, 1);
  
  Matrix4x4 mat(U[0], U[1], U[2], 0,
		V[0], V[1], V[2], 0,
		W[0], W[1], W[2], 0,
		0, 0, 0, 1);
  
  Matrix4x4 rot;
  Vector4d pos(point[0], point[1], point[2], 1);
  
  pos = trans*pos;
  
  pos = mat*pos;
  
  rot.set(1,   0,  0, 0,
	  0,  cx, sx, 0,
	  0, -sx, cx, 0,
	  0,   0,  0, 1);
  
  pos = rot*pos;
  
  rot.set( cy, 0, sy, 0,
	  0, 1,  0, 0,
	  -sy, 0, cy, 0,
	  0, 0,  0, 1);
  
  pos = rot*pos;
  
  pos = mat.inv()*pos;
  
  pos = trans.inv()*pos;
  
  point.set(pos[0], pos[1], pos[2]);
}

//
// Private Helper Methods
//

// set camera position, aim point, and up vector. verify ok, and make up the true up direction
void Camera::computeCoords(const Vector3d &P, const Vector3d &A, const Vector3d &U){
  Vector3d zaxis = P - A;
  // if camera positoin and aim position coincident, no way to aim the camera
  if (zaxis.norm() < epsilon) {
    fprintf (stderr, "Camera position and aim position the same. Can't aim camera!\n");
    exit(1);
  }
  Vector3d dir = -zaxis.normalize();
  
  Vector3d up = U.normalize();
  Vector3d xaxis = dir % up;

  // if up vector and aim vector parallel, no way to tell which way is up
  if (xaxis.norm() < epsilon) {
    fprintf (stderr, "Up parallel to aim. Can't tell which way is up!\n");
    exit(1);
  }

  Pos = P;
  Aim = A;
  Up = xaxis.normalize() % dir; // correct up vector to be perpendicular to dir
}

// Initialize routine setting up defaults
void Camera::initialize() {
  Vector3d tmp, tmp1, tmp2;
  Vector3d axisOrigin, updatePos;
  double dist;
  
  DefaultPos = Pos;
  DefaultAim = Aim;
  DefaultUp = Up;
  
  // find the angle around the x axis 
  updatePos = Pos - Aim;
  axisOrigin.set(updatePos.x, 0, 0);
  dist = (axisOrigin-updatePos).norm();
  tmp1.set(updatePos.x, 0, dist);
  
  tmp = updatePos.normalize();
  tmp1 = tmp1.normalize();
  
  CurrentElev = RadToDeg(acos(tmp * tmp1));
  
  // find the angle around the y axis
  axisOrigin.set(0, updatePos.y, 0);
  dist = (axisOrigin-updatePos).norm();
  
  tmp2.set(0, updatePos.y, dist);
  tmp2 = tmp2.normalize();
  
  CurrentAzim = 360.0 - RadToDeg(acos(tmp2*tmp));
  
  DefaultElev = CurrentElev;
  DefaultAzim = CurrentAzim;
}

/* constructors */

// default constructor... sets position to 0, 0, 5, aimed at the origin
// with the up vector set to the y axis
Camera::Camera() {
  Pos.set(0, 0, 5);
  Aim.set(0, 0, 0);
  Up.set(0, 1, 0);
  
  // set default view volume
  nearPlane = 0.1;
  farPlane = 1000.0;
  fov = 60.0;
  
  initialize();
}

/* 
 * constructor to set a camera to a desired orientation
 * P is position in 3D
 * A is the aim coordinate
 * U is the up vector 
 */
Camera::Camera(Vector3d P, Vector3d A, Vector3d U) {
  computeCoords(P, A, U);
  
  // set default view volume
  nearPlane = 0.1;
  farPlane = 1000.0;
  fov = 60.0;
  
  initialize();
}

/*
 * Constructor setting up all values
 */
Camera::Camera(Vector3d P, Vector3d A, Vector3d U, 
	       float Near, float Far, float ViewAngle) {
  computeCoords(P, A, U);
  
  nearPlane = Near;
  farPlane = Far;
  fov = ViewAngle;

  initialize();
}

// set functions for the Pos, Aim, and Up vectors....
// just remember that |Aim - Pos| != 0, and (Aim - Pos) % Up != 0, and or you'll see problems
void Camera::setPos(Vector3d P) {
  computeCoords(P, Aim, Up);
}

void Camera::setAim(Vector3d A) {
  computeCoords(Pos, A, Up);
}

void Camera::setUp(Vector3d U) {
  computeCoords(Pos, Aim, U);
}

/*
 * sets the near and far clipping planes for the camera view
 */
void Camera::setClippingPlanes(float Near, float Far) {
  nearPlane = Near;
  farPlane = Far;
}

/*
 * sets the field of view of the camera, ViewAngle is in degrees
 */
void Camera::setFOV(float ViewAngle) {
  fov = ViewAngle;
}

/*
 * resets the camera to its original orientation
 */
void Camera::reset() {
  Pos = DefaultPos;
  Aim = DefaultAim;
  Up = DefaultUp;
  
  CurrentElev = DefaultElev;
  CurrentAzim = DefaultAzim;
}

/*
 * sets the camera's aim to be the given vector v
 */
void Camera::setCenterOfFocus(Vector3d NewAim) {
  Vector3d dif = NewAim - Aim;
  computeCoords(Pos + dif, NewAim, Up);
}

/*
 * draws an opengl window with the camera orientation
 * W and H are the width and height of the window respectively
 */
void Camera::perspectiveDisplay(int W, int H, float aspect) {
  
  // set up the projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  gluPerspective(fov, aspect, nearPlane, farPlane);
  gluLookAt(Pos.x, Pos.y, Pos.z, 
	    Aim.x, Aim.y, Aim.z,
	    Up.x, Up.y, Up.z);
}

/*
 * mouse event handler function... should be called in the 
 * mouse event handler function of your own code
 */
void Camera::handleMouseEvent(int button, int state, int x, int y) {
  double realy, wx, wy, wz;
  Vector3d dir;

  if (button == 3 || button == 4) {
    switch(button) {
      case 3:               // 3 indicates Mouse Wheel up, zoom in
        dir = Aim - Pos;  
        Pos = Pos + ZOOMIN * dir;
        break;
      case 4:               // 4 indicates Mouse Wheel down, zoom out
        dir = Aim - Pos;
        Pos = Pos + ZOOMOUT * dir;
        break;
    }
  }
  
  if (state == GLUT_UP && CameraMode != INACTIVE) {
    // update the elevation and roll of the camera
    CurrentElev += DeltaElev;
    CurrentAzim += DeltaAzim;
    
    //printf("%f %f\n", CurrentElev, CurrentAzim);
    
    // reset the change in elevation and roll of the camera
    DeltaElev = DeltaAzim = 0.0;
    
    CameraMode = INACTIVE;
  } else if (state == GLUT_DOWN){ // && mode == GLUT_ACTIVE_ALT) {
    
    // set the new mouse state
    MouseStartX = MousePrevX = x;
    MouseStartY = MousePrevY = y;
    
    // alt key and mouse button have been pressed, camera will move
    
    switch (button) {
      case GLUT_LEFT_BUTTON:
	      // rotating camera
	      CameraMode = ROTATE;
	      break;

      case GLUT_RIGHT_BUTTON:
        CameraMode = ZOOM;
        break;

      case GLUT_MIDDLE_BUTTON:
	      // translating camera:
	      CameraMode = TRANSLATE;
	
	      // get the modelview and projection matrices for projection
	      // of the mouse's cursor into 3D
	      glGetIntegerv(GL_VIEWPORT, ViewPort);
	      glGetDoublev(GL_MODELVIEW_MATRIX, MvMatrix);
	      glGetDoublev(GL_PROJECTION_MATRIX, ProjMatrix);
	
	      // viewport[3] is height of window in pixels
	      realy = ViewPort[3] - y - 1;
	
	      // project the aim of the camera into window coordinates
	      // only concerned about getting the depth (wz) from here
	      gluProject(Aim.x, Aim.y, Aim.z, 
		         MvMatrix, ProjMatrix, ViewPort,
		         &wx, &wy, &wz);
	
	      // from the depth found from the previous call, project the
	      // mouse coordinates into 3D coordinates
	      gluUnProject((GLdouble) x, (GLdouble) realy, wz,
		           MvMatrix, ProjMatrix, ViewPort, 
		           &PrevMousePos.x, &PrevMousePos.y, &PrevMousePos.z);
	
	      break;
    }
  }
}

/*
 * Mouse Motion handler function... should be called in the 
 * mouse motion function of your own code
 */
void Camera::handleMouseMotion(int x, int y) {
  int mouse_dx, mouse_dy, d;
  Vector3d MousePos, dir;
  Vector3d WindowX, WindowY, WindowZ;
  float realy;
  double wx, wy, wz, z;
  
  if (CameraMode != INACTIVE) {
    
    // find the greates change in mouse position 
    mouse_dx = x - MousePrevX;
    mouse_dy = y - MousePrevY;
    
    if (abs(mouse_dx) > abs(mouse_dy)) 
      d = mouse_dx;
    else
      d = mouse_dy;
   
    switch (CameraMode) {
      case ZOOM:
	    // camera is zooming in
	    z = (double) d / 100.0;
	
	    dir = Aim - Pos;
	
	    if (dir.norm() < 0.1 && z > 0) {
	      // move the aim position too when you get in really close
	      z *= 10.0;
	      Aim = Aim + z*dir;
	    }
	
	    // update the new position
	    Pos = Pos + z * dir;
	    break;
      case ROTATE:
	      // camera is rotating
	
	      // get rate of change in screen coordinates from when the 
	      // mouse was first pressed
	      DeltaAzim = ((double) (x - MouseStartX)) / 5.0;
	      DeltaElev = ((double) (y - MouseStartY)) / 5.0;
	
	      // get rate of change in screen coordinate from prev mouse pos
	      LocalDeltaAzim = ((double) mouse_dx)  / 5.0;
	      LocalDeltaElev = ((double) mouse_dy) / 5.0;
	
	      // rotate the window coordinate system by the rate of change
	      // from the onset of the mouse event
	
	      // got this small section of code from Dr. House
	      WindowX.set(1, 0, 0);
	      WindowY.set(0, 1, 0);
	
	      RotateX(WindowX, CurrentElev + DeltaElev);
	      RotateY(WindowX, CurrentAzim + DeltaAzim);
	      WindowX.z = -WindowX.z;
	
	      RotateX(WindowY, CurrentElev+DeltaElev);
	      RotateY(WindowY, CurrentAzim+DeltaAzim);
	      WindowY.z = -WindowY.z;
	
	      WindowZ = (WindowX % WindowY).normalize();
	
	      ArbitraryRotate(WindowX, WindowY, WindowZ, 
			      LocalDeltaElev, 0, Pos, Aim);
	
	      ArbitraryRotate(Vector3d(1, 0, 0), Vector3d(0, 1, 0), 
			      Vector3d(0, 0, 1), 0, -LocalDeltaAzim, Pos, Aim);
	
	      Up = WindowY.normalize();
	
	      break;
      case TRANSLATE:
	      // camera is translating
	
	      realy = ViewPort[3] - y - 1;
	
	      gluProject(Aim.x, Aim.y, Aim.z, 
		         MvMatrix, ProjMatrix, ViewPort, 
		         &wx, &wy, &wz);
	
	      gluUnProject((GLdouble) x, (GLdouble) realy, wz, 
		           MvMatrix, ProjMatrix, ViewPort,
		           &MousePos.x, &MousePos.y, &MousePos.z);
	
	      // move both the camera position and its aim coordinate
	      dir = MousePos - PrevMousePos;
	      Pos = Pos - dir;
	      Aim = Aim - dir;
	
	      PrevMousePos = MousePos;
	      break;
    }    
    MousePrevX = x;
    MousePrevY = y;
  }
}

// assignment operator
const Camera& Camera::operator=(const Camera& Cam) {
  Aim = Cam.Aim;
  Pos = Cam.Pos;
  Up = Cam.Up;
  
  return *this;
}
