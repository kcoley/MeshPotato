/*
*  Tim Curtis
*  Material.h
*
*  Header file for the Face class.
*/

#include "Vertex.h"
#include <cstring>
#include <string>

class Material {
public:
  Material() {}
  ~Material() {}

  string name;

  float Kd[3];
  float Ka[3];
  float Ks[3];
  float Tf[3];
  float Ni;
  float Ns;
  float illum;
};
