#ifndef __MPVOLUMEGRID_H__
#define __MPVOLUMEGRID_H__
#include "MPVolume.h"
using MeshPotato::MPUtils::MPVec3;
namespace MeshPotato {
	namespace MPVolume {
		/// Base class for volume grids
	template<typename T>
	class VolumeGrid: public Volume<T> {
	public:
		virtual ~VolumeGrid() {}

		virtual const typename Volume<T>::volumeDataType eval(const MPUtils::MPVec3 &P) const = 0;
		virtual const typename Volume<T>::volumeGradType grad(const MeshPotato::MPUtils::MPVec3& P) const = 0;
		virtual void set(const MPUtils::Coord ijk, const T value) = 0;
		virtual const T get(const MPUtils::Coord ijk) const= 0;
		virtual void add(const MPUtils::Coord ijk, const T value) = 0;
		virtual void stamp(boost::shared_ptr<Volume<T> > volume) = 0;
		virtual boost::shared_ptr<VolumeGrid<T> > deepCopy() = 0;

		virtual const int nx() const = 0;
		virtual const int ny() const = 0;
		virtual const int nz() const = 0;
	virtual	const MPUtils::MPVec3 indexToWorld(const MPUtils::Coord ijk) const =0;
virtual const MPUtils::MPVec3 worldToIndex(const MPUtils::MPVec3 P) const =0;

	};
	}
}
#endif // __MPVOLUMEGRID_H__
