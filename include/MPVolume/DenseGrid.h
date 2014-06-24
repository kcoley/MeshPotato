///
/// \file DenseGrid.h
///
/// \brief Public API for the Dense Grid data structure
///
/// \author Kacey Coley
/// \date June 19, 2014
/// \since 0.4
///
/// DenseGrid is a data structure which allocates a set grid resolution.
/// As opposed to a sparse grid, the dimension of a dense grid is rigidly defined


#ifndef __DENSEGRID_H__
#define __DENSEGRID_H__
#include "MPUtils/Vector.h"
#include "MPVolume/MPVolumeGrid.h"
#include <openvdb/Grid.h>
#include <openvdb/tools/Interpolation.h>
#include <openvdb/openvdb.h>
#include <openvdb/math/Transform.h>
#include <openvdb/math/BBox.h>
#include <cmath>


using MeshPotato::MPUtils::MPVec3;
/// Partial Template Instantiation for Grid Types

namespace MeshPotato {
  namespace MPVolume {

    template<typename T>
    struct MPGridType {
      typedef openvdb::FloatGrid VDBGridT;
    };
    template<>
    struct MPGridType<float> {
      typedef openvdb::FloatGrid VDBGridT;
    };
    template<>
    struct MPGridType<MPUtils::Color> {
      typedef openvdb::VectorGrid VDBGridT;
    };
    template<>
    struct MPGridType<MPUtils::MPVec3> {
      typedef openvdb::VectorGrid VDBGridT;
    };

    template <typename T>
    class DenseGrid: public MeshPotato::MPVolume::VolumeGrid<T> {
    public:
      typedef typename MPGridType<T>::VDBGridT MPGridT;
      typedef typename MPGridT::Accessor AccessorT;
      typedef typename MPGridT::Ptr MPGridPtr;
      typedef typename openvdb::tools::GridSampler<MPGridT, openvdb::tools::BoxSampler> SamplerT;

      static boost::shared_ptr<VolumeGrid<T> > Ptr(
      const MPUtils::BBox _box,
      const float _resolution) {
        return boost::shared_ptr<VolumeGrid<T> >(new DenseGrid<T>(_box, _resolution));
      }

      DenseGrid(const MPUtils::BBox _bbox,
      const float _resolution) :
      bbox(_bbox),
      voxelSize(_resolution),
      grid(MPGridT::create()),
      interpolator(
      grid->constTree(),
      grid->transform()
      ),
      accessor(grid->getAccessor())
      {

        std::cout << "test" << std::endl;
        grid->transform().preScale(voxelSize);
        grid->transform().postTranslate(bbox.min());
        std::cout << "voxelSize = " << voxelSize << std::endl;
        std::cout << "voxelSize2 = " << grid->transform().voxelSize() << std::endl;
        MPUtils::MPVec3 temp  = ((bbox.max() - bbox.min())/voxelSize);
        dim.x() = (1 + ceil(temp.x()));
        dim.y() = (1 + ceil(temp.y()));
        dim.z() = (1 + ceil(temp.z()));

      }

      /*! \brief evaluates the grid at a particular world space point using trilinear
      *           interpolation
      *
      * \param MPVec3 &P is a const vector representing a world space position
      * \return const typename Volume<T>::volumeDataType
      */
      const typename Volume<T>::volumeDataType  eval(const MPVec3 &P) const {
        return interpolator.wsSample(P);
      }
      const typename Volume<T>::volumeGradType  grad(const MPVec3 &P) const {
      }
      void set(const MPUtils::Coord ijk, const T value) {
        accessor.setValue(ijk, value);
      }
      const T get(const MPUtils::Coord ijk) const{
        return accessor.getValue(ijk);
      }
      void add(const MPUtils::Coord ijk, const T value) {
        accessor.setValue(ijk, accessor.getValue(ijk) + value);
      }
      virtual void stamp(boost::shared_ptr<Volume<T> > volume) {
        MPUtils::Coord ijk;
        int &i = ijk[0], &j = ijk[1], &k = ijk[2];
        for (i = 0; i < dim.x(); ++i) {
          for (j = 0; j < dim.y(); ++j) {
            for (k = 0; k < dim.z(); ++k) {
              MPUtils::MPVec3 P = grid->transform().indexToWorld(ijk);
              accessor.setValue(ijk, volume->eval(P));
            }
          }
        }
      }
      const MPUtils::MPVec3 indexToWorld(const MPUtils::Coord ijk) const {
        return grid->transform().indexToWorld(ijk);
      }
      const MPUtils::MPVec3 worldToIndex(const MPUtils::MPVec3 P) const {
        return grid->transform().worldToIndex(P);
      }
      const int nx() const { return dim.x(); }
      const int ny() const { return dim.y(); }
      const int nz() const { return dim.z(); }

      boost::shared_ptr<VolumeGrid<T> > deepCopy() {
        boost::shared_ptr<VolumeGrid<T> > newGrid = DenseGrid::Ptr(bbox, voxelSize);
        MPUtils::Coord ijk;
        int &i = ijk[0], &j = ijk[1], &k = ijk[2];
        for (i = bbox.min().x(); i < bbox.max().x(); ++i) {
          for (j = bbox.min().y(); j < bbox.max().y(); ++j) {
            for (k = bbox.min().z(); k < bbox.max().z(); ++k) {

              newGrid->set(ijk, accessor.getValue(ijk));
            }
          }
        }
        return newGrid;


      }
    private:
      MPUtils::BBox bbox;
      float voxelSize;
      MPGridPtr grid;
      SamplerT interpolator;
      AccessorT accessor;
      MPUtils::Coord dim;
    };
    typedef boost::shared_ptr<MeshPotato::MPVolume::VolumeGrid<float> > VolumeGridFloatPtr;
    typedef boost::shared_ptr<MeshPotato::MPVolume::VolumeGrid<MPUtils::MPVec3> > VolumeGridVectorPtr;
    typedef VolumeGrid<float> VolumeGridFloat;
    typedef VolumeGrid<MPUtils::MPVec3> VolumeGridVector;
  }

}
#endif // __DENSEGRID_H__
