#include "MPVolume/DenseGrid.h"


namespace MeshPotato {
	namespace MPVolume {

		template <typename T>
		class DenseGrid<T>::Impl {
		public:
			Impl(const MPUtils::BBox &_bbox, const float _resolution) : 
						bbox(_bbox), 
						voxelSize(_resolution),  grid(MPGridT::create()),
      					interpolator(grid->constTree(),grid->transform()),
      					accessor(grid->getAccessor()) {}

			  MPUtils::BBox bbox;
		      float voxelSize;
		      MPGridPtr grid;
		      SamplerT interpolator;
		      AccessorT accessor;
		      MPUtils::Coord dim;
		};


		template <typename T>
		boost::shared_ptr<VolumeGrid<T> > DenseGrid<T>::Ptr(
      	const MPUtils::BBox & _box,
      	const float _resolution) {
        return boost::shared_ptr<VolumeGrid<T> >(new DenseGrid<T>(_box, _resolution));
      }

      template <typename T>
      DenseGrid<T>::DenseGrid(const MPUtils::BBox bbox,
      const float resolution) : mImpl(new Impl(bbox, resolution)) 
      {

        std::cout << "test" << std::endl;
        mImpl->grid->transform().preScale(mImpl->voxelSize);
        mImpl->grid->transform().postTranslate(mImpl->bbox.min());
        std::cout << "voxelSize = " << mImpl->voxelSize << std::endl;
        std::cout << "voxelSize2 = " << mImpl->grid->transform().voxelSize() << std::endl;
        MPUtils::MPVec3 temp  = ((mImpl->bbox.max() - mImpl->bbox.min())/mImpl->voxelSize);
        mImpl->dim.x() = (1 + ceil(temp.x()));
        mImpl->dim.y() = (1 + ceil(temp.y()));
        mImpl->dim.z() = (1 + ceil(temp.z()));

      }

      template <typename T>
      const typename Volume<T>::volumeDataType  DenseGrid<T>::eval(const MPVec3 &P) const {
        return mImpl->interpolator.wsSample(P);
      }

      template <typename T>
      const typename Volume<T>::volumeGradType  DenseGrid<T>::grad(const MPVec3 &P) const {
      }

      template <typename T>
      void DenseGrid<T>::set(const MPUtils::Coord ijk, const T value) {
        mImpl->accessor.setValue(ijk, value);
      }

      template <typename T>
      const T DenseGrid<T>::get(const MPUtils::Coord ijk) const{
        return mImpl->accessor.getValue(ijk);
      }

      template <typename T>
      void DenseGrid<T>::add(const MPUtils::Coord ijk, const T value) {
        mImpl->accessor.setValue(ijk, mImpl->accessor.getValue(ijk) + value);
      }

      template <typename T>
      void DenseGrid<T>::stamp(boost::shared_ptr<Volume<T> > volume) {
        MPUtils::Coord ijk;
        int &i = ijk[0], &j = ijk[1], &k = ijk[2];
        for (i = 0; i < mImpl->dim.x(); ++i) {
          for (j = 0; j < mImpl->dim.y(); ++j) {
            for (k = 0; k < mImpl->dim.z(); ++k) {
              MPUtils::MPVec3 P = mImpl->grid->transform().indexToWorld(ijk);
              mImpl->accessor.setValue(ijk, volume->eval(P));
            }
          }
        }
      }

      template <typename T>
      const MPUtils::MPVec3 DenseGrid<T>::indexToWorld(const MPUtils::Coord ijk) const {
        return mImpl->grid->transform().indexToWorld(ijk);
      }

      template <typename T>
      const MPUtils::MPVec3 DenseGrid<T>::worldToIndex(const MPUtils::MPVec3 P) const {
        return mImpl->grid->transform().worldToIndex(P);
      }

      template <typename T>
      const int DenseGrid<T>::nx() const { return mImpl->dim.x(); }

      template <typename T>
      const int DenseGrid<T>::ny() const { return mImpl->dim.y(); }

      template <typename T>
      const int DenseGrid<T>::nz() const { return mImpl->dim.z(); }

      template <typename T>
      boost::shared_ptr<VolumeGrid<T> > DenseGrid<T>::deepCopy() {
        boost::shared_ptr<VolumeGrid<T> > newGrid = DenseGrid::Ptr(mImpl->bbox, mImpl->voxelSize);
        MPUtils::Coord ijk;
        int &i = ijk[0], &j = ijk[1], &k = ijk[2];
        for (i = mImpl->bbox.min().x(); i < mImpl->bbox.max().x(); ++i) {
          for (j = mImpl->bbox.min().y(); j < mImpl->bbox.max().y(); ++j) {
            for (k = mImpl->bbox.min().z(); k < mImpl->bbox.max().z(); ++k) {

              newGrid->set(ijk, mImpl->accessor.getValue(ijk));
            }
          }
        }
        return newGrid;


      }

      template class DenseGrid<float>;
      template class DenseGrid<MPUtils::MPVec3>;
  }
}
