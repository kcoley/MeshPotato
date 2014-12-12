/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
/// Abstract Base Class for reading mesh types into MeshPotato
///
/// Copyright 2014 Kacey Coley
///   Licensed under the Apache License, Version 2.0 (the "License");
///   you may not use this file except in compliance with the License.
///   You may obtain a copy of the License at
///
///   http://www.apache.org/licenses/LICENSE-2.0
///
///   Unless required by applicable law or agreed to in writing, software
///   distributed under the License is distributed on an "AS IS" BASIS,
///   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
///   See the License for the specific language governing permissions and
///   limitations under the License.
///


#ifndef __INPUTMESH_H__
#define __INPUTMESH_H__


#include <string>
#include <list>
#include <vector>
#include "MPPlugins/defines.h"
#include"MPPlugins/pluginapi.h"
#include "MeshPotato/meshpotato.h"
#include "MPUtils/AttributeTable.h"
namespace MeshPotato{
 namespace MPMesh {
   /**
   * \class InputMesh
   *
   * InputMesh stores the vertices, normals and faces or a mesh.  The faces
   * can be an arbitrary n-gon planar, though it is recommended to use triangles
   * or quadrilaterals.
   **/
   /// Class for storing Mesh Data
    class InputMesh {
    public:
        virtual ~InputMesh() {}
        virtual bool loadMesh(
			       const char *filename,
			       const MPUtils::AttributeTable &table = MPUtils::AttributeTable()
	      )  =0;
        /*! \fn loadMesh
            \brief Load a mesh file, with an optional attribute table
            \param filename The filepath to the file
            \param table An optional AttributeTable
        */
	virtual const char * getName() const =0;
      /*! \fn getName
          \brief Return the name of the mesh type
      */
	virtual const std::list<std::vector<std::string> > getVertices() const =0;
    /*! \fn getVertices
        \brief Return a list of vertices
    */
	virtual const std::list<std::vector<std::string> > getNormals() const =0;
  /*! \fn getNormals
      \brief Return a list of normals
  */
	virtual const std::list<std::vector<std::string> > getFaces() const =0;
  /*! \fn getFaces
      \brief Return a list of faces
  */
	virtual const unsigned int getNumberVertices() const =0;
  /*! \fn getNumberVertices
      \brief Return the number of vertices
  */
	virtual const unsigned int getNumberNormals() const =0;
  /*! \fn getNumberNormals
      \brief Return the number of normals
  */
	virtual const unsigned int getNumberFaces() const =0;
    };
    /*! \fn getNumberFaces
        \brief Return the number of faces
    */
 } // namespace MPMesh
} // namespace MeshPotato

#endif // __INPUTMESH_H__
