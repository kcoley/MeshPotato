/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
///  Factory Class for generating input and output mesh types
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
#ifndef __COREAPI_H__
#define __COREAPI_H__

#include "defines.h"
#include "MeshPotato/MPMesh/OutputMesh.h"
#include "MeshPotato/MPMesh/InputMesh.h"
#include "MeshPotato/MPVolume/MPVolume.h"
#include <string>
#include <map>

///
/// A factory object in the Core API
///
using namespace MeshPotato::MPMesh;
namespace MeshPotato {
namespace MPPlugins {
class CORE_API OutputMeshFactory {
public:
	typedef MeshPotato::MPMesh::OutputMesh *(*CreateCallback)();

	static void RegisterOutputMesh(const std::string & type, CreateCallback cb);
	static void UnregisterOutputMesh(const std::string &type);
	static MeshPotato::MPMesh::OutputMesh *CreateOutputMesh(const std::string &type);
private:
	typedef std::map<std::string, CreateCallback> CallbackMap;
	static CallbackMap mOutputMeshes;

};

class CORE_API InputMeshFactory {
public:
	typedef MeshPotato::MPMesh::InputMesh *(*CreateCallback)();

	static void RegisterInputMesh(const std::string & type, CreateCallback cb);
	static void UnregisterInputMesh(const std::string &type);
	static MeshPotato::MPMesh::InputMesh *CreateInputMesh(const std::string &type);
private:
	typedef std::map<std::string, CreateCallback> CallbackMap;
	static CallbackMap mInputMeshes;

};
class CORE_API VolumeFloatFactory {
public:
	typedef MPVolume::VolumeFloatPtr *(*CreateCallback)();

	static void RegisterVolumeFloat(const std::string & type, CreateCallback cb);
	static void UnregisterVolumeFloat(const std::string &type);
	static MPVolume::VolumeFloatPtr *CreateVolumeFloat(const std::string &type);
private:
	typedef std::map<std::string, CreateCallback> CallbackMap;
	static CallbackMap mVolumeFloats;

};

}
}
#endif // __COREAPI_H__
