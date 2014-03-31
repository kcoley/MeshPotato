///////////////////////////////////////////////////////////////////////////////
// OutputMesh.h                                                              //
//                                                                           //
// Kacey Coley								     //
// Abstract Base Class for Writing Mesh data                                 //
///////////////////////////////////////////////////////////////////////////////
#ifndef __OUTPUTMESH_H__
#define __OUTPUTMESH_H__

#include <string>
#include <list>
#include <vector>
#include "MPPlugins/defines.h"
#include"MPPlugins/pluginapi.h"
#include "MeshPotato/meshpotato.h"
#include "MPUtils/AttributeTable.h"

namespace MeshPotato{
 namespace MPMesh {
    class OutputMesh {
    public:
        virtual ~OutputMesh() {}
        virtual bool loadMesh(
	    const std::list<std::vector<std::string> > &vertices, 
	    const std::list<std::vector<std::string> > &normals, 
	    const std::list<std::vector<std::string> > &faces, 
	    const MPUtils::AttributeTable &table = MPUtils::AttributeTable()
        ) = 0;
	virtual const char * getName() const = 0;
	virtual bool writeMesh(const char *name) = 0;
	virtual const unsigned int getNumberVertices() const = 0;
	virtual const unsigned int getNumberNormals() const = 0;
	virtual const unsigned int getNumberFaces() const = 0;
    	};
 } // namespace MPMesh
} // namespace MeshPotato

#endif// __OUTPUTMESH_H__
