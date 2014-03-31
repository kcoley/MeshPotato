#include "MPMesh/MeshObject.h"
#include <boost/shared_ptr.hpp>
#include <MPPlugins/coreapi.h>
#include "MPPlugins/pluginmanager.h"
#include <boost/filesystem.hpp>
namespace MeshPotato {
namespace MPMesh {
	class MeshObject::Impl {
	public:
		MeshPotato::MPMesh::OutputMesh* oMesh;
		MeshPotato::MPMesh::InputMesh* iMesh;
	};
	MeshObject::~MeshObject() {
		delete mImpl->oMesh;
		delete mImpl->iMesh;
	}
	MeshObject::MeshObject() : mImpl(new Impl) {
		MeshPotato::MPPlugins::PluginManager::GetInstance().LoadAll();

	}

	bool MeshObject::loadMesh(const std::string &name, const MPUtils::AttributeTable table) {
	// clear out the vertices, normals and faces previously stored
	// load the mesh into the inputMesh
		std::string inputExtension = boost::filesystem::extension(name);
		inputExtension = inputExtension.erase(0,1);
		mImpl->iMesh = MeshPotato::MPPlugins::InputMeshFactory::CreateInputMesh(inputExtension);
		      if (! mImpl->iMesh) {
			              std::cout << "Failed to register inputmesh!" << std::endl;
				              return false;
					       }
		        mImpl->iMesh->loadMesh(name.c_str());
			return true;
	}
	bool MeshObject::writeMesh(const std::string &name, const MPUtils::AttributeTable table) {
	std::string outputExtension = boost::filesystem::extension(name);
	outputExtension = outputExtension.erase(0,1);
	mImpl->oMesh = MeshPotato::MPPlugins::OutputMeshFactory::CreateOutputMesh(outputExtension);
	mImpl->oMesh->loadMesh(mImpl->iMesh->getVertices(), mImpl->iMesh->getNormals(), mImpl->iMesh->getFaces(), table);
	mImpl->oMesh->writeMesh(name.c_str());
	return true;
	}
	const std::list<std::vector<std::string> > MeshObject::getVertices() const {
		return mImpl->iMesh->getVertices();
	}
	const std::list<std::vector<std::string> > MeshObject::getNormals() const {
		return mImpl->iMesh->getNormals();
	}
	const std::list<std::vector<std::string> > MeshObject::getFaces() const {
		return mImpl->iMesh->getFaces();
	}
}

}
