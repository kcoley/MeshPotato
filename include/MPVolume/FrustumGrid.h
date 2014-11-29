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
