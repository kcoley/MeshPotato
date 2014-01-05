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
				static boost::shared_ptr<Volume<float> > Ptr(const openvdb::GridBase::Ptr& _grid) {
					return boost::shared_ptr<Volume<float> >(new VDBVolumeGrid(_grid)); 
				}
				VDBVolumeGrid(const openvdb::GridBase::Ptr& _grid) : grid(_grid) {
				}
				VDBVolumeGrid() : grid() {
					grid = openvdb::FloatGrid::create(2.0);
				}
				~VDBVolumeGrid() {}
				virtual const float eval(const MPUtils::MPVec3 &P) const {
					openvdb::FloatGrid::Ptr inputGrid = openvdb::gridPtrCast<openvdb::FloatGrid>(grid);
					openvdb::tools::GridSampler<openvdb::FloatTree, openvdb::tools::QuadraticSampler> interpolator(inputGrid->constTree(), inputGrid->transform());			
					return -interpolator.wsSample(P);

				}
				virtual const MPVec3 grad(const MPVec3 &P) const {
				}
				virtual const float defaultValue() const {}
			private:
				openvdb::GridBase::Ptr grid;

		};
	}

}
