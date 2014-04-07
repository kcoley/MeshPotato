#ifndef __VDBVOLUMEUTILS_H__
#define __VDBVOLUMEUTILS_H__

#include "MPVolume.h"

namespace MeshPotato {
	namespace MPVolume {
		openvdb::FloatGrid::Ptr makeVDBGrid(boost::shared_ptr<Volume<float> > &mpgrid, const openvdb::CoordBBox& indexBB, double h) {
			openvdb::FloatGrid::Ptr vdbgrid = openvdb::FloatGrid::create();
			openvdb::FloatGrid::Accessor accessor = vdbgrid->getAccessor();
			for (openvdb::Int32 i = indexBB.min().x(); i <= indexBB.max().x(); ++i) {
				for (openvdb::Int32 j = indexBB.min().y(); j <= indexBB.max().y(); ++j) {
					for (openvdb::Int32 k = indexBB.min().z(); k <= indexBB.max().z(); ++k) {
						// transform point (i, j, k) of index space into world space
						MeshPotato::MPUtils::MPVec3 p(i * h, j * h, k * h);
						// compute level set function value
						MeshPotato::MPUtils::MPVec3 P(p.x(), p.y(),p.z());
						float value = -mpgrid->eval(P);

						accessor.setValue(openvdb::Coord(i, j, k), value);
					}
				}
			}
			vdbgrid->setTransform(openvdb::math::Transform::createLinearTransform(h));
			vdbgrid->setGridClass(openvdb::GRID_LEVEL_SET);
			return vdbgrid;
		}
	}
} 
#endif // __VDBVOLUMEUTILS_H__
