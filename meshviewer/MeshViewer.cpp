/*
*  Tim Curtis
*  MeshViewer.cpp
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
*   t   - do a turntable of the model at resting position
*   z   - toggle between points, wireframe and shaded viewing
*   w   - write the file out to 'image.ppm'
*   q or Esc	- quit
*
* Camera and model controls following the mouse:
*   Left   button - Rotate camera about aim (usually the model)
*   Right  button - Zoom in/out
*   Middle button - (Scroll) Zoom/in out
*                    (Press)  Pan camera
*/

#include "MeshViewer.h"

// Global Objects
MeshViewer *viewer;
Camera     *cam;

void MeshViewer::initMeshPotato() {
//	mpkernel.loadPlugin("OBJInputPlugin");
}

// Routine to initialize the state of the program to start-up defaults
void MeshViewer::setInitialState(){

  // initial camera viewing controls
  glDisable(GL_LIGHTING);

  Axes = FALSE;
  shadingMode = POINTS;
  angle = 0;

  // initial camera orientation and position
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

/*
*  Routine to build the viewport and projection matrices and to
*  save the projection matrix
*/
void MeshViewer::updateProjection() {
  // initialize the projection matrixp
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  cam->perspectiveDisplay(Width, Height, Aspect);

  // restore modelview matrix as the one being updated
  glMatrixMode(GL_MODELVIEW);
}

// Routine to draw a set of coordinate axes centered at the origin
void MeshViewer::drawAxes(float size){
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
void MeshViewer::drawModel(){

  if(shadingMode != SHADED)
    mode = GL_LINE_LOOP;
  else
    mode = GL_POLYGON;

  list<Face>::const_iterator face_iter;
  list<int>::const_iterator v_iter;
  list<int>::const_iterator n_iter;
  const float lightgrey[3] = {0.8, 0.8, 0.8};
  const float grey[3] = {0.5, 0.5, 0.5};
  const float white[3] = {1, 1, 1};

  for(face_iter = faces.begin(); face_iter != faces.end(); face_iter++) {  
    glBegin(mode);
      if(shadingMode == SHADED) {
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
*  Routine to read in data from the specified .obj file
*  Will populate the respective vectors with the data for later displaying
*/
void MeshViewer::readFile(char* inName) {
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

  cout << "Number of Vertices: " << vertices.size() << endl;
  cout << "Number of Normals:  " << vertNorms.size() << endl;
  cout << "Number of Faces:    " << faces.size() << endl;

  inFile.close();
  buildVBOs();
}

/* 
*  Routine to read in data from the specified .mtl file
*  Will populate the respective vectors with the data for later displaying
*/
void MeshViewer::readMaterialFile(string inName) {
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

int MeshViewer::findMatIndex(string matName) {
  unsigned int i;
  for(i = 0; i < mats.size(); i++) {
    if(mats.at(i).name == matName)
      return i;
  }
  
  return -1;
}

// Routine to create Vertex Buffer Objects to hold mesh data
void MeshViewer::buildVBOs() {
  vertQuads = vertTris = normQuads = normTris = texQuads = texTris = NULL;
  unsigned int i = 0, t, q;
  quadCount = triCount = 0;
  bool warning = FALSE;
  string answer;

  list<Face>::const_iterator f;
  list<int>::const_iterator v;
  for(f = faces.begin(); f != faces.end(); f++) {
    if(f->vIndexes.size() == 3)
      triCount += 3;
    else if(f->vIndexes.size() == 4)
      quadCount += 4;
    else if(!warning) {
      cout << endl << "********" << endl;
      cout << "WARNING! Non-triangle or non-quad polygons encountered!" << endl;
      cout << f->vIndexes.size() << " vertices on this polygon!" << endl;
      cout << "You are likely to see rendering problems with shading!" << endl;
      cout << "********" << endl << endl;
      cout << "Would you like to suppress future warnings? (y/n) >> ";
      cin >> answer;
      if(answer == "y") {
        cout << endl << "Future warnings will be suppressed..." << endl;
        warning = !warning;
      } else
        cout << endl << "Next warning will not be suppressed..." << endl;
    }
  }

  if(triCount > 0) {
    vertTris  = new float[3 * triCount];
    normTris  = new float[3 * triCount];
    texTris  = new float[4 * triCount];
  }
  if(quadCount > 0) {
    vertQuads = new float[3 * quadCount];
    normQuads = new float[3 * quadCount];
    texQuads = new float[4 * quadCount];
  }

  t = 0;
  q = 0;
  for(f = faces.begin(); f != faces.end(); f++) {
    if(f->vIndexes.size() == 3) {
      i = 0;
      for(v = f->vIndexes.begin(); v != f->vIndexes.end(); v++) {
        vertTris[t*9+i]   = vertices[(*v)].x;
        vertTris[t*9+i+1] = vertices[(*v)].y;
        vertTris[t*9+i+2] = vertices[(*v)].z;
        i+=3;
      }
      t++;
    } else if(f->vIndexes.size() == 4) {
      i = 0;
      for(v = f->vIndexes.begin(); v != f->vIndexes.end(); v++) {
        vertQuads[q*12+i]   = vertices[(*v)].x;
        vertQuads[q*12+i+1] = vertices[(*v)].y;
        vertQuads[q*12+i+2] = vertices[(*v)].z;
        i+=3;
      }
      q++;
    }
  }
  cout << t*3 << " vertices have been copied into triangle array ..." << endl;
  cout << q*4 << " vertices have been copied into quad array ..." << endl;

  if(vertNorms.size() > 0) {
    t = 0;
    q = 0;
    for(f = faces.begin(); f != faces.end(); f++) {
      if(f->nIndexes.size() == 3) {
        i = 0;
        for(v = f->nIndexes.begin(); v != f->nIndexes.end(); v++) {
          normTris[t*9+i]   = vertNorms[(*v)].x;
          normTris[t*9+i+1] = vertNorms[(*v)].y;
          normTris[t*9+i+2] = vertNorms[(*v)].z;
          i+=3;
        }
        t++;
      } else if(f->nIndexes.size() == 4) {
        i = 0;
        for(v = f->nIndexes.begin(); v != f->nIndexes.end(); v++) {
          normQuads[q*12+i]   = vertNorms[(*v)].x;
          normQuads[q*12+i+1] = vertNorms[(*v)].y;
          normQuads[q*12+i+2] = vertNorms[(*v)].z;
          i+=3;
        }
        q++;
      }
    }
    cout << t*3 << " normals have been copied into triangle array ..." << endl;
    cout << q*4 << " normals have been copied into quad array ..." << endl;
  } else {
    // If there are no normals, create them all as (0,1,0)
    for(i = 0; i < triCount*3; i+=3) {
      normTris[i]   = 0;
      normTris[i+1] = 1;
      normTris[i+2] = 0;
    }
    for(i = 0; i < quadCount*3; i+=3) {
      normQuads[i]   = 0;
      normQuads[i+1] = 1;
      normQuads[i+2] = 0;
    }
    cout << "Default normals (0,1,0) have been loaded into arrays ..." << endl;

    /*
    glGenBuffers(1, &texVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texVBO);
    glBufferData(GL_ARRAY_BUFFER, vertCount*4*sizeof(float), tex, GL_STATIC_DRAW);
    cout << "Default colors bound to array buffer ..." << endl;
    */
  }

  /* TODO: Needs extra work to grab and map correct texture values to a buffer
  if(vertTex.size() > 0 && vertTex.size() == vertCount) {
    tex = new float[2 * vertCount];
    for(i = 0; i < vertCount; i++) {
      tex[i]   = vertTex[i].x;
      tex[i+1] = vertTex[i].y;
    }
    cout << "Texture Coords have been copied into VBO..." << endl;
  }
  */

  // If there is no texture data, map RGBA colors to light grey
  for(i = 0; i < triCount*4; i+=4) {
    texTris[i]   = 0.8;
    texTris[i+1] = 0.8;
    texTris[i+2] = 0.8;
    texTris[i+3] = 1.0;
  }
  for(i = 0; i < quadCount*4; i+=4) {
    texQuads[i]   = 0.8;
    texQuads[i+1] = 0.8;
    texQuads[i+2] = 0.8;
    texQuads[i+3] = 1.0;
  }
  cout << "Default color (light grey) has been loaded into an arrays ..." << endl;

  glGenBuffers(1, &vertTrisVBO);
  glBindBuffer(GL_ARRAY_BUFFER, vertTrisVBO);
  glBufferData(GL_ARRAY_BUFFER, triCount*3*sizeof(float), vertTris, GL_STATIC_DRAW);
  cout << "Triangle Vertices bound to array buffer ..." << endl;

  glGenBuffers(1, &normTrisVBO);
  glBindBuffer(GL_ARRAY_BUFFER, normTrisVBO);
  glBufferData(GL_ARRAY_BUFFER, triCount*3*sizeof(float), normTris, GL_STATIC_DRAW);
  cout << "Triangle Normals bound to array buffer ..." << endl;

  glGenBuffers(1, &texTrisVBO);
  glBindBuffer(GL_ARRAY_BUFFER, texTrisVBO);
  glBufferData(GL_ARRAY_BUFFER, triCount*4*sizeof(float), texTris, GL_STATIC_DRAW);
  cout << "Triangle Colors bound to array buffer ..." << endl;

  glGenBuffers(1, &vertQuadsVBO);
  glBindBuffer(GL_ARRAY_BUFFER, vertQuadsVBO);
  glBufferData(GL_ARRAY_BUFFER, quadCount*3*sizeof(float), vertQuads, GL_STATIC_DRAW);
  cout << "Quad Vertices bound to array buffer ..." << endl;

  glGenBuffers(1, &normQuadsVBO);
  glBindBuffer(GL_ARRAY_BUFFER, normQuadsVBO);
  glBufferData(GL_ARRAY_BUFFER, quadCount*3*sizeof(float), normQuads, GL_STATIC_DRAW);
  cout << "Quad Normals bound to array buffer ..." << endl;

  glGenBuffers(1, &texQuadsVBO);
  glBindBuffer(GL_ARRAY_BUFFER, texQuadsVBO);
  glBufferData(GL_ARRAY_BUFFER, quadCount*4*sizeof(float), texQuads, GL_STATIC_DRAW);
  cout << "Quad Colors bound to array buffer ..." << endl;

  destructArrays();
}

void MeshViewer::turntable() {
  struct timespec frameTime, frameRem;
  frameTime.tv_sec  = 0;
  frameTime.tv_nsec = 40000000L;

  for(int f = 0; f < 120; f++) {
    angle = f*3;
    doDisplay();

    nanosleep(&frameTime, &frameRem);
  }

  angle = 0;
  glutPostRedisplay();
}

/*
*  This function will write the data read from the OpenGL frame buffer to
*  image.ppm (can be changed in the code)
*/
void MeshViewer::writeFile() {
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
void MeshViewer::doDisplay(){
  // create a white light which will come from the upper left
  const float light_direction[4] = {-1, 1, 1, 0};
  const float light_color[4] = {1, 1, 1, 1};
  const float ambient_color[4] = {0.1, 0.1, 0.1, 1};

  // clear window to background color and clear depth buffer if shading
  if(shadingMode != SHADED) {
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

  glRotatef(angle, 0, 1, 0);

  glLightfv(GL_LIGHT0, GL_POSITION, light_direction);
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_color);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);

  // draw axes if required
  glDisable(GL_LIGHTING);
  if(Axes)
    drawAxes(axisLength);
  
  // if shaded drawing is required, turn on lighting
  if(shadingMode != SHADED) {
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
  }
  else {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);    
  }
///*

  if(shadingMode == POINTS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
  else if(shadingMode == WIREFRAME)
    glPolygonMode(GL_FRONT, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  //glColor3f(0.8f, 0.8f, 0.8f);
  //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  //glEnable(GL_COLOR_MATERIAL);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  if(triCount > 0) {
    glBindBuffer(GL_ARRAY_BUFFER, normTrisVBO);
    glNormalPointer(GL_FLOAT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, vertTrisVBO);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, texTrisVBO);
    glColorPointer(4, GL_FLOAT, 0, 0);
    if(shadingMode == POINTS)
      glDrawArrays(GL_POINTS, 0, triCount);
    else
      glDrawArrays(GL_TRIANGLES, 0, triCount);
  }

  if(quadCount > 0) {
    glBindBuffer(GL_ARRAY_BUFFER, normQuadsVBO);
    glNormalPointer(GL_FLOAT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, vertQuadsVBO);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, texQuadsVBO);
    glColorPointer(4, GL_FLOAT, 0, 0);
    if(shadingMode == POINTS)
      glDrawArrays(GL_POINTS, 0, quadCount);
    else
      glDrawArrays(GL_QUADS, 0, quadCount);
  }

  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
//*/
  //drawModel();
  glutSwapBuffers();
}

/*
*  Reshape Callback
*  Keep viewport, modelview and projection matrices the same as they were,
*  even though window resizes
*/
void MeshViewer::doReshape(int width, int height) {
  glViewport(0, 0, width, height);
  Width = width;
  Height = height;

  updateProjection();
}

/*
*  Keyboard callback routine. 
*  Set various modes or take actions based on key presses
*/
void MeshViewer::handleKey(unsigned char key, int x, int y){

  switch(key){

  case 'i':			// I -- reinitialize 
  case 'I':
    setInitialState();
    cam->reset();
    updateProjection();
    glutPostRedisplay();
    break;

  case 'a':			// A -- toggle between drawing axes or not
  case 'A':
    Axes = !Axes;
    glutPostRedisplay();
    break;

  case 't':			// T -- do a turntable
  case 'T':
    turntable();
    break;
/*
  case 'w':     // W -- write current display to file
  case 'W':
    pixmap = new RGBA[Width * Height];
    glReadPixels(0, 0, Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, pixmap);
    writeFile();
    delete [] pixmap;
    break;
*/
  case 'z':     // Z -- toggle between points, wireframe and shaded views
  case 'Z':
    shadingMode = (shadingMode+1)%3;
    if(shadingMode == POINTS)
      cout << "Shading mode: POINTS" << endl;
    else if(shadingMode == WIREFRAME)
      cout << "Shading mode: WIREFRAME" << endl;
    else
      cout << "Shading mode: SHADED" << endl;
    glutPostRedisplay();
    break;
    
  case 'q':			// Q or Esc -- exit program
  case 'Q':
    exit(0);
  }
}

/*
*  Initialize OpenGL to establish lighting and colors
*  and initialize viewing and model parameters
*/
void MeshViewer::initialize(char* inName){

  readFile(inName);
  axisLength = ((abs(maxX - minX) + abs(maxY - minY) + abs(maxZ - minZ))/3) * 1.2;
  Depth = ((fabs(maxX - minX) + fabs(maxY - minY) + fabs(maxZ - minZ))/3) * 2;

  Vector3d center((maxX + minX)/2.0, (maxY + minY)/2.0, (maxZ + minZ)/2.0);
  cout << "Model Center - " << center << endl;

  Vector3d pos = center + Vector3d(0, 0, Depth);
  Vector3d up(0, 1, 0);
  cam = new Camera(pos, center, up);
  
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
*  to invoke the callback routines found in the MeshViewer class
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

/*
*  Mouse Button Callback
*  Pass mouse button information to camera
*/
void myHandleButtons(int button, int state, int x, int y) {
  cam->handleMouseEvent(button, state, x, y);
  viewer->updateProjection();
  glutPostRedisplay();
}

/*
*  Mouse Motion Callback
*  Pass mouse motion to camera
*/
void myHandleMotion(int x, int y) {
  cam->handleMouseMotion(x, y);
  viewer->updateProjection();
  glutPostRedisplay();
}

// Main program to create window, setup callbacks, and initiate GLUT
int main(int argc, char* argv[]) {

  if(argc < 2) {
    cout << "Usage: ./meshviewer [filename.obj]"  << endl;
    exit(0);
  }

  string arg(argv[1]);

  if(arg.find(".obj") == string::npos) {
    cout << "Please provide a .obj file to read in." << endl;
    exit(0);
  }

  viewer = new MeshViewer();

  // start up the glut utilities
  glutInit(&argc, argv);

  // create the graphics window, giving width, height, and title text
  // and establish double buffering, RGBA color
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Mesh Viewer");
  glewInit();

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
