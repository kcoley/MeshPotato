#include "MPVolume.h"

namespace MeshPotato {
    namespace MPVolume {
        template <typename T>
        /// Translates a volume
	    class TranslateVolume : public Volume<T> {
	    public:
	        typedef typename GradType<T>::GType volumeGradType;
		static boost::shared_ptr<Volume<T> > Ptr(const boost::shared_ptr<Volume<T> > &_f,
							 const MeshPotato::MPUtils::MPVec3 &_D) {
		    return boost::shared_ptr<TranslateVolume<T> >(new TranslateVolume<T>(_f, _D)); }

		TranslateVolume(const boost::shared_ptr<Volume<T> > &_f,
				const MeshPotato::MPUtils::MPVec3 &_D) :
		    f(_f), D(_D) {}

		virtual const T eval(const MeshPotato::MPUtils::MPVec3 &P) const {
		    return f->eval(P - D);
					}

		virtual const volumeGradType grad(const MeshPotato::MPUtils::MPVec3& P) const {}
		private:
			boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > f;
			MeshPotato::MPUtils::MPVec3 D;
	   };
    }
}
