#ifndef __IMPLICITSHAPE_H__
#define __IMPLICITSHAPE_H__
#include "MPVolume/MPVolume.h"
#include "MPUtils/Vector.h"
#include "MPUtils/MPNoise.h"
namespace MeshPotato {
	namespace MPVolume {
		class ImplicitSphere : public MeshPotato::MPVolume::Volume<float> {
			public:
				static boost::shared_ptr<ImplicitSphere> Ptr(const float &_R,  const MeshPotato::MPUtils::MPVec3 &_C) {
                                        return boost::shared_ptr<ImplicitSphere>(new ImplicitSphere(_R, _C));
                                }

				ImplicitSphere(const float &_R, const MeshPotato::MPUtils::MPVec3 &_C) : R(_R), C(_C) {}
				virtual const volumeDataType eval(const MeshPotato::MPUtils::MPVec3 &P) const { 
					return R - (P - C).length(); 
				}
				virtual const volumeGradType grad(const MeshPotato::MPUtils::MPVec3& P) const {}
			private:
				float R;
				MeshPotato::MPUtils::MPVec3 C;
		};
		class PyroclasticSphere : public MeshPotato::MPVolume::Volume<float> {
			public:
				static boost::shared_ptr<PyroclasticSphere> Ptr(const float &_R,  const MeshPotato::MPUtils::MPVec3 &_C, const MeshPotato::MPNoise::Noise_t &_noiseparms) {
                                        return boost::shared_ptr<PyroclasticSphere>(new PyroclasticSphere(_R, _C, _noiseparms));
                                }

				PyroclasticSphere(const float &_R, const MeshPotato::MPUtils::MPVec3 &_C, MeshPotato::MPUtils::Noise_t &_noiseparms) : R(_R), C(_C), noiseparms(_noiseparms), perlin(), noise() {
					perlin.setParameters(parms);
					noise = &perlin;
				}
				virtual const volumeDataType eval(const MeshPotato::MPUtils::MPVec3 &P) const {
					return _R - (P - C).length() + pow(fabs(noise->eval((P - C)/(P - C).length())), parms.gamma)*1.0;
				}
				virtual const volumeGradType grad(const MeshPotato::MPUtils::MPVec3& P) const {
					return (C-P).unit();
				}
			private:
				float R;
				MeshPotato::MPUtils::MPVec3 C;
				MeshPotato::MPNoise::Noise_t noiseparms;
				MeshPotato::MPNoise::FractalSum<PerlinNoiseGustavson> perlin;
				boost::shared_ptr<MeshPotato::MPNoise::Noise> noise;
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
		template<typename T>
		class ConstantVolume: public MeshPotato::MPVolume::Volume<T> {
			public:
			typedef typename GradType<T>::GType volumeGradType;
			static boost::shared_ptr<Volume<T> > Ptr(const T _value) { 
				return boost::shared_ptr<ConstantVolume>(new ConstantVolume<T>(_value));
			}
			ConstantVolume(const T _value) : value(_value) {}
			virtual const T eval(const MeshPotato::MPUtils::MPVec3 &P) const {
				return value;
			}
			virtual const volumeGradType grad(const MeshPotato::MPUtils::MPVec3 &P) const {}
			private:
				T value;
		};
	}
}
#endif // __IMPLICITSHAPE_H__
