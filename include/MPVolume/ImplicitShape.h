#ifndef __IMPLICITSHAPE_H__
#define __IMPLICITSHAPE_H__
#include "MPVolume/MPVolume.h"
#include "MPUtils/Vector.h"
namespace MeshPotato {
	namespace MPVolume {
		class ImplicitSphere : public MeshPotato::MPVolume::Volume<float> {
			public:
				static boost::shared_ptr<ImplicitSphere> Ptr(const float &_R,  const MeshPotato::MPUtils::MPVec3 &_C) {
                                        return boost::shared_ptr<ImplicitSphere>(new ImplicitSphere(_R, _C));
                                }

				typedef typename GradType<float>::GType volumeGradType;
				ImplicitSphere(const float &_R, const MeshPotato::MPUtils::MPVec3 &_C) : R(_R), C(_C) {}
				virtual const float eval(const MeshPotato::MPUtils::MPVec3 &P) const { 
					return R - (P - C).length(); 
				}
				virtual const volumeGradType grad(const MeshPotato::MPUtils::MPVec3& P) const {}
			private:
				float R;
				MeshPotato::MPUtils::MPVec3 C;
		};
		template<typename T>
		class Clamp: public MeshPotato::MPVolume::Volume<T> {
			public:
			typedef typename GradType<T>::GType volumeGradType;
			static boost::shared_ptr<Volume<T> > Ptr(const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> >&_f,  const float _e0, const float _e1) { 
				return boost::shared_ptr<Clamp>(new Clamp<T>(_f, _e0, _e1));
			}
			Clamp(const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > &_f,  const float _e0, const float _e1) : f(_f), e0(_e0), e1(_e1) {}
			virtual const T eval(const MeshPotato::MPUtils::MPVec3 &P) const {
                                        if (e0 <= f->eval(P) && f->eval(P) <= e1) return f->eval(P);
					else if (f->eval(P) < e0) return e0;
					else if (f->eval(P) > e1) return e1;
			}
			virtual const volumeGradType grad(const MeshPotato::MPUtils::MPVec3 &P) const {}
			private:
				boost::shared_ptr<MeshPotato::MPVolume::Volume<T> >f;
				float e0, e1;
		};
	}
}
#endif // __IMPLICITSHAPE_H__
