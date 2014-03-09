///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used                                        //
///////////////////////////////////////////////////////////////////////////////
#ifndef __INPUTMESHAPI_H__
#define __INPUTMESHAPI_H__


#include "defines.h"
#include"pluginapi.h"
#include <string>
#include <list>
#include <vector>
#include "MeshPotato/meshpotato.h"
namespace MeshPotato{
	namespace MPPlugins {

		class InputMeshAPI {
			public:
				virtual ~InputMeshAPI() {}
				virtual bool loadMesh(const char *filename) = 0;
				virtual const char * getName() const = 0;
				virtual const std::list<std::vector<std::string> > getVertices() const = 0;
				virtual const std::list<std::vector<std::string> > getNormals() const = 0;
				virtual const std::list<std::vector<std::string> > getFaces() const = 0;
				virtual const unsigned int getNumberVertices() const = 0;
				virtual const unsigned int getNumberNormals() const = 0;
				virtual const unsigned int getNumberFaces() const = 0;
		};
	} // namespace MPPlugins
} // namespace MeshPotato

#endif // __INPUTMESHAPI_H__
