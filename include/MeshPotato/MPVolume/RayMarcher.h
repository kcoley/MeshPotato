/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
///  Class for raymarching a VDB volume
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

#ifndef __RAYMARCHER_H__
#define __RAYMARCHER_H__
#include "ImplicitShape.h"
#include <iostream>
#include <openvdb/Grid.h>
#include <openvdb/tools/Interpolation.h>
#include <openvdb/tools/RayIntersector.h>
#include "MeshPotato/MPVolume/FrustumGrid.h"
#include "MeshPotato/MPUtils/DeepImage.h"
#include "MeshPotato/MPUtils/Image.h"
#include "tbb/tbb.h"
#include "MeshPotato/MPUtils/OIIOFiles.h"
#include "MeshPotato/MPUtils/AttributeTable.h"
using namespace MeshPotato::MPUtils;
namespace MeshPotato {
namespace MPVolume {
/// Raymarcher for vdb volumes
class VDBRayMarcher {
public:
typedef openvdb::tools::VolumeRayIntersector<openvdb::FloatGrid> IntersectorT;
typedef openvdb::FloatGrid::ConstAccessor  AccessorType;
typedef openvdb::tools::BoxSampler SamplerT;
typedef openvdb::tools::GridSampler<AccessorType, SamplerT> SamplerType;
VDBRayMarcher(openvdb::FloatGrid::Ptr _grid,
	     VolumeColorPtr _dsm,
	     boost::shared_ptr<MeshPotato::MPUtils::Camera> _camera,
	     const AttributeTable &_table = AttributeTable()
	     );

const MeshPotato::MPUtils::Color L(
	MPRay &ray,
	IntersectorT &intersector2,
	SamplerType &interpolator2) const;

const MeshPotato::MPUtils::DeepPixelBuffer deepL(
	MPRay &ray,
	IntersectorT &intersector2,
	SamplerType &interpolator2) const;

void operator() (const tbb::blocked_range<size_t>& r) const;

void writeImage(const std::string outputImage);

void render(bool threaded) const;

private:
class Impl;
boost::shared_ptr<Impl> mImpl;
};

/// Generic Raymarcher for Resolution Independent Volumes
class RayMarcher {
public:
RayMarcher(
	VolumeFloatPtr _grid,
	VolumeColorPtr _dsm,
	boost::shared_ptr<Camera> _camera,
	const AttributeTable &_table /*= AttributeTable()*/
	);

MeshPotato::MPUtils::Color L(MPRay &ray) const;

void operator() (const tbb::blocked_range<size_t>& r) const;

void render(bool threaded) const;

void writeImage(const std::string outputImage);

private:
class Impl;
boost::shared_ptr<Impl> mImpl;
};


}
}
#endif // __RAYMARCHER_H__
