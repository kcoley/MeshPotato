#include "MPVolume.h"

namespace MeshPotato {
namespace MPVolume {
template <typename T, typename FieldType>
class AddVolume: public MeshPotato::MPVolume::Volume<T> {
public:
AddVolume(const boost::shared_ptr<FieldType> _f1, const boost::shared_ptr<FieldType> _f2) : f1(_f1), f2(_f2) {}
	static boost::shared_ptr<Volume<float> > create(const boost::shared_ptr<FieldType> _f1, const boost::shared_ptr<FieldType> _f2) {
                                        return boost::shared_ptr<Volume<float> >(new AddVolume<T, FieldType>(_f1, _f2));
        }

	virtual const T eval(const MeshPotato::MPUtils::MPVec3 &P) const { return f1->eval(P) + f2->eval(P);}
	virtual const MeshPotato::MPUtils::MPVec3 grad(const MeshPotato::MPUtils::MPVec3 &P) const {}
private:
	boost::shared_ptr<FieldType > f1, f2;	
};

}
}
