#include <boost/python.hpp>
#include <MPVolume/MPVolume.h>
using namespace boost::python;
using namespace MeshPotato::MPVolume;
using namespace MeshPotato::MPUtils;
struct MPVolumeWrap : Volume<float>, wrapper<Volume<float> > {
	const float eval(const MPVec3& P) const {
		return this->get_override("eval")();
	}
};
BOOST_PYTHON_MODULE(mpvolume) {
	class_<MPVolumeWrap, boost::noncopyable>("Volume")
//		.def("eval", pure_virtual(&Volume<float>::eval))	
	;
}
