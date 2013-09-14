#include <MeshPotato/meshpotato.h>
#include <MPMesh/MPMesh.h>
#include <MPUtils/Vector.h>
int main() {
	//! Create two mesh objects
	MeshPotato::MPMesh::MPMesh mesh("test.obj");

	MeshPotato::MPMesh::MPMesh mesh2;
	// OpenVDB Vector
	MeshPotato::MPUtils::Vector3d vect;

	// Ilm Vector
	MeshPotato::MPUtils::Vector2 vect2;
	
//	MeshPotato::MeshInput simpleMesh;
//	MeshPotato::MeshInput *example = simpleMesh.open("sample.obj");	
	vect.x() = 10.0f;
	vect.y() = 20.0f;
	vect.z() = 30.0f;

	vect2.x = 10.0f;
	vect2.y = 20.0f;
	vect2.z = 30.0f;

	//! Set the name of the second mesh
	mesh.SetName("new name");
//	int k = (int)mesh2;
//	std::cout<<k<<std::endl;

	//! Print out the name of the mesh
	std::cout << "mesh name =  " << mesh << std::endl;
	std::cout << "mesh2 name = " << mesh2 << std::endl;
	std::cout << "Vect = " << vect + vect << std::endl;
	std::cout << "Vect2 = " << vect2 + vect2<< std::endl;
	//! Generate an OBJ file from the mesh
//	mesh->Write("example.obj");

	return 0;
}
