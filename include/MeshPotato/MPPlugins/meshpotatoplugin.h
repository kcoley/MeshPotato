/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
///  Abstract Base Class for writing a MeshPotato Mesh Plugin
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

#ifndef MESHPOTATOPLUGIN_H
#define MESHPOTATOPLUGIN_H
#include <MeshPotato/meshpotato.h>
#include <dlfcn.h>
namespace MeshPotato
{
	namespace MPPlugins {
		template<typename Interface>
		class Factory {
			virtual Interface *create() = 0;
		};
		class InputMesh {
			virtual void open() = 0;
			virtual void close() = 0;

		};
		typedef Factory<InputMesh> InputMeshFactory;
	}
}
#endif //MESHPOTATOPLUGIN_H
