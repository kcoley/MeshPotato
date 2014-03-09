///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used                                        //
///////////////////////////////////////////////////////////////////////////////
#ifndef __OUTPUTMESHAPI_H__
#define __OUTPUTMESHAPI_H__


#include "defines.h"
#include"pluginapi.h"
#include <string>
#include <list>
#include <vector>
#include "MeshPotato/meshpotato.h"
namespace MeshPotato{
	namespace MPPlugins {

		class OutputMeshAPI {
			public:
				virtual ~OutputMeshAPI() {}
				virtual bool loadMesh(const std::list<std::vector<std::string> > &vertices, const std::list<std::vector<std::string> > &normals, const std::list<std::vector<std::string> > &faces, const MeshPotato::MeshSpec &spec) = 0;
				virtual const char * getName() const = 0;
				virtual bool writeMesh(const char *name) = 0;
				virtual const unsigned int getNumberVertices() const = 0;
				virtual const unsigned int getNumberNormals() const = 0;
				virtual const unsigned int getNumberFaces() const = 0;
		};
	} // namespace MPPlugins
} // namespace MeshPotato

#endif // __OUTPUTMESHAPI_H__
