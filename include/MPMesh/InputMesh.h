///////////////////////////////////////////////////////////////////////////////
// InputMesh		                                                     //
//                                                                           //
// Kacey Coley								     //
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
    class InputMesh {
    public:
        virtual ~InputMesh() {}
        virtual bool loadMesh(
			const char *filename, 
			const MPUtils::AttributeTable &table = MPUtils::AttributeTable()
	)  =0;
	virtual const char * getName() const =0;
	virtual const std::list<std::vector<std::string> > getVertices() const =0;
	virtual const std::list<std::vector<std::string> > getNormals() const =0;
	virtual const std::list<std::vector<std::string> > getFaces() const =0;
	virtual const unsigned int getNumberVertices() const =0;
	virtual const unsigned int getNumberNormals() const =0;
	virtual const unsigned int getNumberFaces() const =0;
    };
 } // namespace MPMesh
} // namespace MeshPotato

#endif // __INPUTMESH_H__
