#ifndef __FIELD_ALGEBRA_H__
#define __FIELD_ALGEBRA_H__
#include "MPVolume.h"

namespace MeshPotato {
namespace MPVolume {
template <typename T>
class AddVolume: public MeshPotato::MPVolume::Volume<T> {
public:
AddVolume(const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f1, const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f2) : f1(_f1), f2(_f2) {}
	static boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > Ptr(const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f1, const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f2) {
                                        return boost::shared_ptr<Volume<T> >(new AddVolume<T>(_f1, _f2));
        }

	virtual const T eval(const MeshPotato::MPUtils::MPVec3 &P) const { return f1->eval(P) + f2->eval(P);}
	virtual const MeshPotato::MPUtils::MPVec3 grad(const MeshPotato::MPUtils::MPVec3 &P) const {}
private:
	boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > f1, f2;	
};

}
}
#endif // __FIELD_ALGEBRA_H__
