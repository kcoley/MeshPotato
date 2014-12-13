/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
/// Set of typedef Math routines
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

#include <vector>
#include <openvdb/math/Vec3.h>
#include <openvdb/math/Mat3.h>
#include <openvdb/math/Vec4.h>
#include <openvdb/util/Util.h>
#include <openvdb/openvdb.h>
#include <openvdb/tools/MeshToVolume.h>
#include <openvdb/math/Ray.h>
#include <OpenEXR/ImathVec.h>

namespace MeshPotato {
	//! Utilities 
	namespace MPUtils {
		//! MPUtilities
		typedef openvdb::math::Vec3d  MPVec3;
		typedef openvdb::math::Mat3<float> MPMat3;
		typedef openvdb::math::Mat4d MPMat4;
		typedef openvdb::math::Ray<double> MPRay;	
		typedef openvdb::BBoxd BBox;		
		typedef openvdb::Coord Coord;		
		typedef std::vector<openvdb::math::Vec4<uint32_t> > Polys;
		typedef std::vector<openvdb::math::Vec3<float> >Verts;
		

	}
}
