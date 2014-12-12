/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
/// Class for retrieving mesh information
///
///   Copyright 2014 Kacey Coley
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

#include "MPPlugins/pluginmanager.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <MeshPotato/meshpotato.h>
#include <list>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "MPUtils/AttributeTable.h"

namespace MeshPotato {
namespace MPMesh {
	/// Class for retrieving mesh information
	class MeshObject {
	public:
		MeshObject();
		~MeshObject();
	        // Load Mesh
	        bool loadMesh(const std::string &name, const MPUtils::AttributeTable table=MPUtils::AttributeTable());
	        bool writeMesh(const std::string &name, const MPUtils::AttributeTable table=MPUtils::AttributeTable());
		const std::list<std::vector<std::string> > getVertices() const;
		const std::list<std::vector<std::string> > getNormals() const;
		const std::list<std::vector<std::string> > getFaces() const;
		
		class Impl;
		boost::shared_ptr<Impl> mImpl;
	};
}
}
