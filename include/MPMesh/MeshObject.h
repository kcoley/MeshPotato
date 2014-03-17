#include "MPPlugins/pluginmanager.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <MeshPotato/meshpotato.h>
#include <list>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace MeshPotato {
namespace MPMesh {
class MeshObject {
public:
	MeshObject();
	~MeshObject();
        // Load Mesh
        bool loadMesh(const std::string &name);
        bool writeMesh(const std::string &name);
	const std::list<std::vector<std::string> > getVertices() const;
	const std::list<std::vector<std::string> > getNormals() const;
	const std::list<std::vector<std::string> > getFaces() const;
	
	class Impl;
	boost::shared_ptr<Impl> mImpl;
};
}
}
