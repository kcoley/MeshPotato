#include "MeshPotato/meshpotato.h"

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
				MPMesh(std::string);
				//! Copy Constructor
				MPMesh(const MPMesh&);				
				//! Destructor
				~MPMesh();

				//! Name
				const std::string GetName() const;

				void SetName(std::string); 

				//! Read in a mesh
				const int Read(const std::string);

				//! Write out a mesh
				const int Write(const std::string);

			private:
				std::string name;
		};
		std::ostream& operator<<(std::ostream& out, const MPMesh& mesh);

	}
}
