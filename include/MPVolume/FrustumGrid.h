/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
///  Class for creating Frustum Grids
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

#ifndef __FRUSTUM_GRID_H__
#define __FRUSTUM_GRID_H__
#include "MPUtils/Vector.h"
#include "MPVolume/VDBVolumeGrid.h"
#include <openvdb/Grid.h>
#include <openvdb/tools/Interpolation.h>
#include <openvdb/tools/RayIntersector.h>
#include "MPUtils/Camera.h"

namespace MeshPotato {
namespace MPVolume {
	/// Creates a frustum-shaped grid
class FrustumGrid : public Volume<float> {
public:

	static boost::shared_ptr<FrustumGrid > Ptr(boost::shared_ptr<MeshPotato::MPUtils::Camera> _cam, openvdb::BBoxd _bbox, openvdb::Coord::ValueType x_count=100, openvdb::Coord::ValueType z_count=100);
	
	FrustumGrid(boost::shared_ptr<MeshPotato::MPUtils::Camera> _cam, openvdb::BBoxd _bbox, openvdb::Coord::ValueType x_count=100, openvdb::Coord::ValueType z_count=100);
	
	const float eval(const MeshPotato::MPUtils::MPVec3 &P) const;
	
	const void set(const openvdb::Coord &ijk, float value);
	
	MeshPotato::MPUtils::MPVec3 indexToWorld(const openvdb::Coord &ijk);
	
	virtual const MeshPotato::MPUtils::MPVec3 grad(const MeshPotato::MPUtils::MPVec3 &P) const;

	const void stamp(const VolumeFloatPtr density_grid);

	const void dsm(const VolumeFloatPtr density_grid, double K);
private:
	class Impl;
	boost::shared_ptr<Impl> mImpl;
};

}
}
#endif // __FRUSTUM_GRID_H__
