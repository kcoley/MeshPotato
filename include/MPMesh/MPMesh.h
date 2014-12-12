/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
/// Class for generating various mesh types
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

#include "MeshPotato/meshpotato.h"
#include <boost/shared_ptr.hpp>

namespace MeshPotato {
	/// Handles Mesh Types
	namespace MPMesh
	{
		/// Class for Generation of Meshes (OBJ, Alembic, VDB) so far	
		class MPMesh {
			public:
				//Constructors

				//! Default Constructor
				MPMesh();

				//! Conversion Constructor
				MPMesh(const std::string &);
				//! Copy Constructor
				MPMesh(const MPMesh &);				
				//! Destructor
				~MPMesh();

				//! Name
				const std::string GetName() const;

				void SetName(const std::string &); 

				//! Read in a mesh
				const int Read(const std::string &);

				//! Write out a mesh
				const int Write(const std::string &);

			private:
				class Impl;
				boost::shared_ptr<Impl> mImpl;
		};
		std::ostream& operator<<(std::ostream& out, const MPMesh& mesh);

	}
}
