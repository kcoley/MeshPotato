/*
*  Tim Curtis
*  CpSc 405
*  Dr. House
*  10/19/09
*  Project 5 - Object Viewer
*    GLViewer.cpp
*
* A good deal of the source code here is credited to:
*   BIHE Computer Graphics    Donald H. House     12/12/06
*   Demonstration of OpenGL Matrices
*
* Additionally, some code from:
* http://www.gamedev.net/community/forums/topic.asp?topic_id=490770 was
* consulted for help in parsing .obj files.  Credit given to member
* force_of_will.
*
* This program will initialize an ojbect viewer utilzing the GLUT libraries and
* OpenGL procedure calls.  It will read in the specified file from the command
* line, and use the data in the file to construct and display the various faces
* of that desired object.  The object materials aren't supported in this
* parser, so three materials are hard-coded in (being white, grey, and dark
* grey), which will be cycled through as each 'usemtl' is spotted in the .obj
* file.  Though, smooth shading across the polygons is supported when vertex
* normals are specified in the .obj file.
*
* Keyboard keypresses have the following effects:
*   a		- toggle drawing coordinate axes
*   i   - reinitialize (reset program to initial default state)
*   p		- toggle between orthographic and perspective view
*   z   - toggle between wireframe and shaded viewing
*   w   - write the file out to 'image.ppm'
*   q or Esc	- quit
*
* Camera and model controls following the mouse:
*   model yaw   - left-button, horizontal motion, rotation of the model around the y axis
*   model tilt  - left-button, vertical motion, rotation of the model about the x axis
*   camera yaw  - middle-button, horizontal motion, rotation of the camera about the y axis
*   camera tilt	- middle-button, vertical motion, rotation of the camera about the x axis
*   approach    - right-button, vertical or horizontal motion, translation of camera along z axis
*
*/

#include "GLViewer.h"
// Global GLViewer to be used in the class and in main()
GLViewer *viewer;

void GLViewer::initMeshPotato() {
//	mpkernel.loadPlugin("OBJInputPlugin");


}

// Routine to initialize the state of the program to start-up defaults
void GLViewer::setInitialState(){

  // initial camera viewing controls
  Projection = ORTHO;
  glDisable(GL_LIGHTING);

  Axes = FALSE;
  Button = NONE;

  // initial camera orientation and position
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  Pan = 0;
  Tilt = 0;
  Approach = Depth;

  // initial model orientation
  ThetaX = 0;
  ThetaY = 0;
}

/*
*  Routine to build the viewport and projection matrices and to
*  save the projection matrix
*/
void GLViewer::updateProjection() {
  // initialize the projection matrixp
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  float h, w;

  // determine the projection system and drawing coordinates
  if(Projection == ORTHO){
    // make screen size match perspective at the nominal depth of the object
    h = Depth * sin(PI * (YANGLE / 2.0) / 180);
    w = ASPECT * h;
    glOrtho(-w, w, -h, h, NEAR, FAR);
  }
  else {
    gluPerspective(YANGLE, ASPECT, NEAR, FAR);
  }

  // restore modelview matrix as the one being updated
  glMatrixMode(GL_MODELVIEW);
}

// Routine to draw a set of coordinate axes centered at the origin
void GLViewer::drawAxes(float size){
  glBegin(GL_LINES);
    glColor3f(1, 0, 0);		  // x axis drawn in red
    glVertex3f(0, 0, 0);
    glVertex3f(size, 0, 0);

    glColor3f(0, 1, 0);		  // y axis drawn in green
    glVertex3f(0, 0, 0);
    glVertex3f(0, size, 0);

    glColor3f(0, 0, 1);		  // z axis drawn in blue
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, size);
  glEnd();
}

// Routine to draw the model
void GLViewer::drawModel(){

  if(wireframe)
    mode = GL_LINE_LOOP;
  else
    mode = GL_POLYGON;

  list<Face>::const_iterator face_iter;
  list<int>::const_iterator v_iter;
  list<int>::const_iterator n_iter;
  const float lightgrey[3] = {0.1, 0.1, 0.1};
  const float grey[3] = {0.5, 0.5, 0.5};
  const float white[3] = {1, 1, 1};

  for(face_iter = faces.begin(); face_iter != faces.end(); face_iter++) {  
    glBegin(mode);
      if(!wireframe) {
        if(face_iter->mat != -1) {
          glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mats.at(face_iter->mat).Ka);
          glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mats.at(face_iter->mat).Kd);
          glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mats.at(face_iter->mat).Ks);
        } else {
          glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, lightgrey);
          glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, grey);
          glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
        }
      } else {
        glColor3f(1, 1, 1);
      }
      n_iter = face_iter->nIndexes.begin();
      for(v_iter = face_iter->vIndexes.begin(); v_iter != face_iter->vIndexes.end(); v_iter++) {
        Vertex v;
        if(face_iter->nIndexes.size() > 0) {
          v = vertNorms.at(*(n_iter));
          n_iter++;
          glNormal3f(v.x, v.y, v.z);   
        } else {
          glNormal3f(0, 1, 0);
        }
        v = vertices.at(*(v_iter));
        glVertex3f(v.x, v.y, v.z);
      }
    glEnd();
  }
}

/* 
*  Routine to read in data from the specified .mtl file
*  Will populate the respective vectors with the data for later displaying
*/
void GLViewer::readMaterialFile(string inName) {
  ifstream inFile(inName.c_str(), ifstream::in);

  if(!inFile) {
    cout << "Error opening the specified file, exiting." << endl;
    exit(0);
  }

  char skip[256];
  string key;
  float i, x, y, z;
  string matName;

  inFile >> key;
  while (inFile.good()) {
    if(key == "newmtl") {

      Material m;
      inFile >> matName;

      while(inFile.good() && inFile >> key) {
        if(key == "newmtl") {
          break;
        }

        if(key == "illum") {
            inFile >> i;
            m.illum = i;
        } else if(key == "Kd") {
            inFile >> x >> y >> z;
            m.Kd[0] = x; m.Kd[1] = y; m.Kd[2] = z;
        } else if(key == "Ka") {
            inFile >> x >> y >> z;
            m.Ka[0] = x; m.Ka[1] = y; m.Ka[2] = z;
        } else if(key == "Ks") {
            inFile >> x >> y >> z;
            m.Ks[0] = x; m.Ks[1] = y; m.Ks[2] = z;
        } else if(key == "Tf") {
            inFile >> x >> y >> z;
            m.Tf[0] = x; m.Tf[1] = y; m.Tf[2] = z;
        } else if(key == "Ni") {
            inFile >> i;
            m.Ni = i;
        }
      }
      m.name = matName;
      mats.push_back(m);
    // Skip the line if the cases aren't satisfied above
    } else {
      inFile.getline(skip, 256);
    }
  }
  inFile.close();
}

int GLViewer::findMatIndex(string matName) {
  unsigned int i;
  for(i = 0; i < mats.size(); i++) {
    if(mats.at(i).name == matName)
      return i;
  }
  
  return -1;
}

/* 
*  Routine to read in data from the specified .obj file
*  Will populate the respective vectors with the data for later displaying
*/
void GLViewer::readFile(char* inName) {
  cout << "Opening " << inName << " ... " << endl;
  ifstream inFile(inName, ifstream::in);

  if(!inFile) {
    cout << "Error opening the specified file, exiting." << endl;
    exit(0);
  }

  char skip[256];
  string buffer, key;
  Vertex v;
  char ch;
  string matName;
  int index, nIndex, tIndex;
  int material = -1;

  while (inFile.good() && (inFile >> key)) {

    Face face;

    // If material libray is provided, load the materials
    if(key == "mtllib") {
      cout << "Found material library ..." << endl;
      inFile >> matName;
      string path(inName);
      string fullpath = path.substr(0, path.find_last_of("/")+1);
      readMaterialFile(fullpath + matName);
    }

    // The following five if statements will check for vertices ('v'), vertex
    // normals ('vn'), texture coordinates ('vt'), material change commands 
    // ('usemtl') and faces ('f') at the start of each line
    else if(key == "v") {
      inFile >> v.x >> v.y >> v.z;
      if(v.x > maxX)  maxX = v.x;
      if(v.x < minX)  minX = v.x;
      if(v.y > maxY)  maxY = v.y;
      if(v.y < minY)  minY = v.y;
      if(v.z > maxZ)  maxZ = v.z;
      if(v.z < minZ)  minZ = v.z;
      vertices.push_back(v); 
    }
    else if(key == "vn") {
      inFile >> v.x >> v.y >> v.z;
      vertNorms.push_back(v);
    }
    else if(key == "vt") {
      inFile >> v.x >> v.y;
      vertTex.push_back(v);
    }
    else if(key == "usemtl") {
      inFile >> matName;
      material = findMatIndex(matName);
    }
    else if(key == "f") {          // If a face is found, pull the whole line,
      getline(inFile, buffer);      // and then parse through that line as a 
      stringstream line(buffer);    // separate input stream

      while(line.good()) {
        // If this check passes, there are no text coords or normals
        if(buffer.find("/") == string::npos) {
          line >> index;
          face.vIndexes.push_back(index - 1);
        }
        // If this check passes, it is only vertices followed by vertex normals
        else if(buffer.find("//") != string::npos) {;
          line >> index >> ch >> ch >> nIndex;
          face.vIndexes.push_back(index - 1);
          face.nIndexes.push_back(nIndex - 1);
        }
        // Otherwise, the input will follow the format v/t/n
        else {
          line >> index >> ch >> tIndex >> ch >> nIndex;
          face.vIndexes.push_back(index - 1);
          face.tIndexes.push_back(tIndex - 1);
          face.nIndexes.push_back(nIndex - 1);
        }
      }
      face.mat = material;
      faces.push_back(face);
    }
    // Skip the line if one of the five cases aren't satisfied above
    else {
      inFile.getline(skip, 256);
    }
  }

  inFile.close();
}

/*
*  This function will write the data read from the OpenGL frame buffer to
*  image.ppm (can be changed in the code)
*/
void GLViewer::writeFile() {
  FILE *out;
  out = fopen("image.ppm", "w");

  if(out == NULL) {
    cout << "Unable to open file for writing, exiting." << endl;
    exit(0);
  }

  // Write header for new file.
  fprintf(out, "P6\n");     
  fprintf(out, "%d %d %d\n", WIDTH, HEIGHT, 255);

  int row, col;

  // Write pixels out, starting from bottom row.  Do this since they were stored
  // as so for the GL rendering.
  for(row = HEIGHT - 1; row >= 0; row--) {
    for(col = 0; col < WIDTH; col++) {
      fputc(pixmap[row * WIDTH + col][0], out);
      fputc(pixmap[row * WIDTH + col][1], out);
      fputc(pixmap[row * WIDTH + col][2], out);
    }
  }

  fclose(out);  

}

// Display callback
void GLViewer::doDisplay(){
  // create a white light which will come from the upper left
  const float light_direction[4] = {-1, 1, 1, 0};
  const float light_color[4] = {1, 1, 1, 1};
  const float ambient_color[4] = {0.1, 0.1, 0.1, 1};

  // clear window to background color and clear depth buffer if shading
  if(wireframe) {
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    glShadeModel(GL_FLAT);
  }
  else {
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);
  }
  
   // light is positioned in camera space so it does not move with object
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glLightfv(GL_LIGHT0, GL_POSITION, light_direction);
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_color);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);

  // establish camera coordinates
  glRotatef(Tilt, 1, 0, 0);	    // tilt - rotate camera about x axis
  glRotatef(Pan, 0, 1, 0);	    // pan - rotate camera about y axis
  glTranslatef(0, 0, -Approach);    // approach - translate camera along z axis

  // rotate the model
  glRotatef(ThetaY, 0, 1, 0);       // rotate model about x axis
  glRotatef(ThetaX, 1, 0, 0);       // rotate model about y axis

  // draw axes if required
  glDisable(GL_LIGHTING);
  if(Axes)
    drawAxes(axisLength);
  
  // if shaded drawing is required, turn on lighting
  if(wireframe){
    glDisable(GL_LIGHTING);
  }
  else {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);    
  }

  drawModel();
  glutSwapBuffers();
}

/*
*  Keyboard callback routine. 
*  Set various modes or take actions based on key presses
*/
void GLViewer::handleKey(unsigned char key, int x, int y){

  switch(key){

  case 'p':			// P -- toggle between ortho and perspective
  case 'P':
    Projection = !Projection;
    updateProjection();
    glutPostRedisplay();
    break;

  case 'i':			// I -- reinitialize 
  case 'I':
    setInitialState();
    updateProjection();
    glutPostRedisplay();
    break;

  case 'a':			// A -- toggle between drawing axes or not
  case 'A':
    Axes = !Axes;
    glutPostRedisplay();
    break;

  case 'w':     // R -- write current display to file
  case 'W':
    pixmap = new RGBA[Width * Height];
    glReadPixels(0, 0, Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, pixmap);
    writeFile();
    delete [] pixmap;
    break;

  case 'z':     // W -- toggle between shaded and wireframe views
  case 'Z':
    wireframe = !wireframe;
    glutPostRedisplay();
    break;
    
  case 'q':			// Q or Esc -- exit program
  case 'Q':
    exit(0);
  }
}

/*
*  Mouse Button Callback
*  on button press, record mouse position and which button is pressed
*/
void GLViewer::handleButtons(int button, int state, int x, int y){

  if(state == GLUT_UP)
    Button = NONE;		  // no button pressed
  else{
    MouseY = -y;		    // invert y window coordinate to correspond with OpenGL
    MouseX = x;
    Button = button;		// store which button pressed
  }
}

/*
*  Mouse Motion Callback
*  when mouse moves with a button down, update appropriate camera parameter
*/
void GLViewer::handleMotion(int x, int y){
  int delta;
  
  y = -y;
  int dy = y - MouseY;
  int dx = x - MouseX;

  switch(Button){
  case GLUT_LEFT_BUTTON:
    ThetaX -= ROTFACTOR * dy;
    ThetaY += ROTFACTOR * dx;
    glutPostRedisplay();
    break;
  case GLUT_MIDDLE_BUTTON:
    Pan -= ROTFACTOR * dx;
    Tilt += ROTFACTOR * dy;
    glutPostRedisplay();
    break;
  case GLUT_RIGHT_BUTTON:
    delta = (Abs(dx) > Abs(dy)? dx: dy);
    Approach += XLATEFACTOR * delta;
    glutPostRedisplay();
    break;
  }

  MouseX = x;
  MouseY = y;
}

/*
*  Reshape Callback
*  Keep viewport, modelview and projection matrices the same as they were,
*  even though window resizes
*/
void GLViewer::doReshape(int width, int height) {
  glViewport(0, 0, width, height);
  Width = width;
  Height = height;

  updateProjection();
}

/*
*  Initialize OpenGL to establish lighting and colors
*  and initialize viewing and model parameters
*/
void GLViewer::initialize(char* inName){

  readFile(inName);
  axisLength = ((abs(maxX - minX) + abs(maxY - minY) + abs(maxZ - minZ))/3) * 1.2;
  Depth = ((fabs(maxX - minX) + fabs(maxY - minY) + fabs(maxZ - minZ))/3) * 2;

  cout << "Model Coordinates -" << endl;
  cout << "Center x: " << (maxX + minX)/2 << " Center y: " <<
    (maxY + minY)/2 << " Center z: " << (maxZ + minZ)/2 << endl;
  
  // initialize modelview matrix to identity
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // specify window clear (background) color to be black
  glClearColor(0, 0, 0, 1);

  // initialize viewing and model parameters
  setInitialState();
  updateProjection();
}

/*
*  The following 5 functions are simply dummy functions that are used
*  to invoke the callback routines found in the GLViewer class
*/
void myDisplay() {
  viewer->doDisplay();
}

void myReshape(int width, int height) {
  viewer->doReshape(width, height);
}

void myHandleKey(unsigned char key, int x, int y) {
  viewer->handleKey(key, x, y);
}

void myHandleButtons(int button, int state, int x, int y) {
  viewer->handleButtons(button, state, x, y);
}

void myHandleMotion(int x, int y) {
  viewer->handleMotion(x, y);
}

// Main program to create window, setup callbacks, and initiate GLUT
int main(int argc, char* argv[]) {

  if(argc < 2) {
    cout << "Usage: ./objview [filename.obj]"  << endl;
    exit(0);
  }

  string arg(argv[1]);

  if(arg.find(".obj") == string::npos) {
    cout << "Please provide a .obj file to read in." << endl;
    exit(0);
  }

  viewer = new GLViewer();

  // start up the glut utilities
  glutInit(&argc, argv);

  // create the graphics window, giving width, height, and title text
  // and establish double buffering, RGBA color
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Object Viewer");

  // register callback to draw graphics when window needs updating
  glutDisplayFunc(myDisplay);
  glutReshapeFunc(myReshape);
  glutKeyboardFunc(myHandleKey);
  glutMouseFunc(myHandleButtons);
  glutMotionFunc(myHandleMotion);

  viewer->initialize(argv[1]);

  glutMainLoop();

  delete viewer;
  return 0;
}
