#ifndef __VDBVOLUMEGRID_H__
#define __VDBVOLUMEGRID_H__
#include "MPUtils/Vector.h"
#include "MPVolume/MPVolumeGrid.h"
#include <openvdb/Grid.h>
#include <openvdb/tools/Interpolation.h>
#include <openvdb/openvdb.h>
using MeshPotato::MPUtils::MPVec3;

namespace MeshPotato {
	namespace MPVolume {
		template<typename T>
                struct GridType {
                	typedef openvdb::FloatGrid VDBGridT;
                };
		template<>
                struct GridType<float> {
                	typedef openvdb::FloatGrid VDBGridT;
                };
		template<>
                struct GridType<MPUtils::Color> {
                	typedef openvdb::VectorGrid VDBGridT;
                };
		template<>
                struct GridType<MPUtils::MPVec3> {
                	typedef openvdb::VectorGrid VDBGridT;
                };

		template <typename T>
		class VDBVolumeGrid: public MeshPotato::MPVolume::Volume<T> {
			public:
				typedef typename GridType<T>::VDBGridT GridT;  
				typedef typename GridT::ConstAccessor AccessorT;  
				typedef typename GridT::Ptr GridPtr;
				typedef typename openvdb::tools::GridSampler<GridT, openvdb::tools::BoxSampler> SamplerT;
				static boost::shared_ptr<Volume<T> > Ptr(
					const GridPtr _grid) {
					return boost::shared_ptr<Volume<T> >(new VDBVolumeGrid(_grid)); 
				}
				VDBVolumeGrid(const GridPtr _grid) : 
					grid(_grid), 
					interpolator(grid->constTree(), grid->transform()) {}

				openvdb::GridBase::Ptr getVDBGrid() const { return grid; }
				~VDBVolumeGrid() {}
				const typename Volume<T>::volumeDataType  eval(const MPVec3 &P) const {
					return interpolator.wsSample(P);

				}
				const typename Volume<T>::volumeGradType  grad(const MPVec3 &P) const {
				}
			private:
				GridPtr grid;
				SamplerT interpolator;			

		};
	}

}
#endif // __VDBVOLUMEGRID_H__
