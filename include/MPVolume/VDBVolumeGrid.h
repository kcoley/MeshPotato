#ifndef __VDBVOLUMEGRID_H__
#define __VDBVOLUMEGRID_H__
#include "MPUtils/Vector.h"
#include "MPVolume/MPVolumeGrid.h"
#include <openvdb/Grid.h>
#include <openvdb/tools/Interpolation.h>
using MeshPotato::MPUtils::MPVec3;
typedef openvdb::FloatGrid::Ptr VDBFloatGridPtr;
namespace MeshPotato {
	namespace MPVolume {
		class VDBVolumeGrid: public MeshPotato::MPVolume::Volume<float> {
			public:
				static boost::shared_ptr<Volume<float> > Ptr(const openvdb::FloatGrid::Ptr _grid) {
					return boost::shared_ptr<Volume<float> >(new VDBVolumeGrid(_grid)); 
				}
				VDBVolumeGrid(const openvdb::FloatGrid::Ptr _grid) : grid(_grid), interpolator(grid->constTree(), grid->transform()) {}

				openvdb::GridBase::Ptr getVDBGrid() const { return grid; }
				~VDBVolumeGrid() {}
				virtual const float eval(const MPUtils::MPVec3 &P) const {
					return interpolator.wsSample(P);

				}
				virtual const MPVec3 grad(const MPVec3 &P) const {
				}
				virtual const float defaultValue() const {}
			private:
				openvdb::FloatGrid::Ptr grid;
				openvdb::tools::GridSampler<openvdb::FloatTree, openvdb::tools::BoxSampler> interpolator;			

		};
	}

}
#endif // __VDBVOLUMEGRID_H__
