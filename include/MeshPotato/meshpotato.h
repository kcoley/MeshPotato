/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
///  Class for representing Mesh Specifications
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

#ifndef __MESHPOTATO_H__
#define __MESHPOTATO_H__
#include <iostream>
#include <string.h>
namespace MeshPotato {
	class MeshSpec {
	public:
		MeshSpec(): voxelSize(0.05), exBandWidth(3), inBandWidth(3) {}
		MeshSpec(const float &vs, const float &eb, const float &ib) : voxelSize(vs), exBandWidth(eb), inBandWidth(ib) {}
		float voxelSize;
		float exBandWidth;
		float inBandWidth;
	}; //!< Define the specification for mesh data

	struct DeepData {}; //!< Strut for deep data

	class MeshInput {
		public:
			MeshInput();
			MeshInput(const MeshInput &mesh);
			~MeshInput();
			//! Attempt to open the mesh file
			/*!
			 \param const std::string & - Mesh Name
			 \return an input Mesh handle
			*/
			MeshInput *create(const std::string &);
			virtual bool create(const std::string &, const MeshSpec &newspec) = 0;

	}; //! Input Mesh

	class MeshOutput {}; //! Output Mesh




}
#endif // __MESHPOTATO_H__
