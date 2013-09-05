#include "MPMesh/MPMesh.h"

namespace MeshPotato {
namespace MPMesh {

		//! Default Constructor
		MPMesh::MPMesh()   {
			//! Initialize the vertices, indices, etc to default values
			name = "unnamed";
		}
		MPMesh::MPMesh(std::string  n) : name(n) {
			
		}
		//! Copy Constructor
		MPMesh::MPMesh(const MPMesh&) {
			//! Deep copy the mesh to another mesh
			
		}
		//! Destructor
		MPMesh::~MPMesh() {
		}
		const std::string MPMesh::GetName() const {
			return name;
		}

		//! Set Name
		void MPMesh::SetName(std::string n) {
			name = n;
		}
		//! Output operator
		std::ostream& operator<<(std::ostream& out, const MPMesh& mesh) {
                        return out << mesh.GetName();
                }
	
}
}
