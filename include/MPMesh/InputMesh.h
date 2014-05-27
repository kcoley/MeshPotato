///////////////////////////////////////////////////////////////////////////////
// InputMesh		                                                             //
//                                                                           //
// Kacey Coley								                                               //
// Abstract Base Class for reading mesh types into MeshPotato                //
///////////////////////////////////////////////////////////////////////////////
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
   * \brief InputMesh is the Abstract Base Class for reading Mesh Formats.
   *
   * InputMesh stores the vertices, normals and faces or a mesh.  The faces
   * can be an arbitrary n-gon planar, though it is recommended to use triangles
   * or quadrilaterals.
   **/
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
