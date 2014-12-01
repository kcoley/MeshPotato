#include "MeshPotato/meshpotato.h"
#include <boost/shared_ptr.hpp>

//! Global namespace for project
namespace MeshPotato {
	//! Handles mesh generation and conversion
	namespace MPMesh
	{

		//! Class for Generation of Meshes (OBJ, Alembic, VDB) so far	
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
