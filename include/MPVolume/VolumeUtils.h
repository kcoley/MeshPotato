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
