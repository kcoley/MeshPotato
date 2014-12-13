/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
///  Set of routines for working with volumes
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

#ifndef __VOLUMEUTILS_H__
#define __VOLUMEUTILS_H__

#include "MPVolume.h"
#include "FrustumGrid.h"
namespace MeshPotato {
	namespace MPVolume {
		/// Helper function for reading VDB grids
		openvdb::GridBase::Ptr readVDBGrid(const std::string &vdbFile);
/// Helper function for writing VDB grids
		openvdb::FloatGrid::Ptr makeVDBGrid(boost::shared_ptr<Volume<float> > &mpgrid, const MeshPotato::MPUtils::BBox& worldBB, double voxelSize);

		/// Helper function for building frustum cameras
		boost::shared_ptr<MeshPotato::MPUtils::Camera> buildFrustumCamera(MeshPotato::MPUtils::MPVec3 &eye, openvdb::FloatGrid::Ptr grid);
		
		double computeFOVDegrees(MeshPotato::MPUtils::MPVec3 &point, MeshPotato::MPUtils::MPVec3 &eye, MeshPotato::MPUtils::MPVec3 &view);
		double computeDistance(MeshPotato::MPUtils::MPVec3 &point, MeshPotato::MPUtils::MPVec3 &eye, MeshPotato::MPUtils::MPVec3 &view);

	}
}
#endif // __VOLUMEUTILS_H__
