#include "MPMesh/MPMesh.h"
#include <iostream>

namespace MeshPotato {
	namespace MPMesh {
	class MPMesh::Impl {
	public:
		Impl() : name() {}
		Impl(const std::string &n) : name(n) {}
		std::string name;
	};

		//! Default Constructor
		MPMesh::MPMesh() : mImpl(new Impl())  {
			//! Initialize the vertices, indices, etc to default values
			mImpl->name = "unnamed";
		}
		MPMesh::MPMesh(const std::string &n) : mImpl(new Impl(n)) {
			
		}
		//! Copy Constructor
		MPMesh::MPMesh(const MPMesh &) {
			//! Deep copy the mesh to another mesh
			
		}
		//! Destructor
		MPMesh::~MPMesh() {
		}

		//! Set Name
		void MPMesh::SetName(const std::string &n) {
			mImpl->name = n;
		}
		//! Set Name
		const std::string MPMesh::GetName() const {
			return mImpl->name;
		}
		//! Output operator
		std::ostream& operator<<(std::ostream& out, const MPMesh& mesh) {
                        return out << mesh.GetName();
                }
	
	}
}
