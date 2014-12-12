/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
///  Class for wrapping an OpenVDB volume
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

		/// OpenVDB Grid Wrapper

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
