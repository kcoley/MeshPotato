#include "MPPlugins/coreapi.h"
#include <iostream>

namespace MeshPotato {
namespace MPPlugins {
OutputMeshFactory::CallbackMap OutputMeshFactory::mOutputMeshes;
InputMeshFactory::CallbackMap InputMeshFactory::mInputMeshes;

void OutputMeshFactory::RegisterOutputMesh(const std::string &type, CreateCallback cb) {
	mOutputMeshes[type] = cb;
}
void InputMeshFactory::RegisterInputMesh(const std::string &type, CreateCallback cb) {
	mInputMeshes[type] = cb;
}
void OutputMeshFactory::UnregisterOutputMesh(const std::string &type) {
	mOutputMeshes.erase(type);
}
void InputMeshFactory::UnregisterInputMesh(const std::string &type) {
	mInputMeshes.erase(type);
}

OutputMeshAPI *OutputMeshFactory::CreateOutputMesh(const std::string &type) {
	CallbackMap::iterator it = mOutputMeshes.find(type);
	if (it != mOutputMeshes.end()) {
		// call the creation callback to construct this derived type
		return (it->second)();
	}
	std::cout <<"type " << type << " not found" << std::endl;
	return NULL;

}
InputMeshAPI *InputMeshFactory::CreateInputMesh(const std::string &type) {
	CallbackMap::iterator it = mInputMeshes.find(type);
	if (it != mInputMeshes.end()) {
		// call the creation callback to construct this derived type
		return (it->second)();
	}
	std::cout <<"type " << type << " not found" << std::endl;
	return NULL;

}
} // namespace MPPlugins
} // namespace MeshPotato
