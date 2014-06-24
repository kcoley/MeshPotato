#ifndef __IMPLICITSHAPE_H__
#define __IMPLICITSHAPE_H__
#include "MPVolume/MPVolume.h"
#include "MPUtils/Vector.h"
#include "MPNoise/Noise.h"
#include "MPNoise/PerlinNoise.h"
#include <boost/shared_ptr.hpp>
namespace MeshPotato {
	namespace MPVolume {
		/// Abstract Base class for implicit shapes
		class ImplicitShape: public MeshPotato::MPVolume::Volume<float> {
			public:
			virtual const volumeDataType eval(const MeshPotato::MPUtils::MPVec3 &P) const = 0;
			virtual const volumeGradType grad(const MeshPotato::MPUtils::MPVec3 &P) const = 0;
		};
		class ImplicitSphere : public ImplicitShape {
			public:
				static boost::shared_ptr<Volume<float> > Ptr(const float &_R,  const MeshPotato::MPUtils::MPVec3 &_C);
				ImplicitSphere(const float &_R, const MeshPotato::MPUtils::MPVec3 &_C);
				~ImplicitSphere();
				const volumeDataType eval(const MeshPotato::MPUtils::MPVec3 &P) const;
				const volumeGradType grad(const MeshPotato::MPUtils::MPVec3& P) const;
			private:
				class Impl;
				boost::shared_ptr<Impl> mImpl;
		};
		class PyroclasticSphere : public ImplicitShape {
			public:
				static VolumeFloatPtr Ptr(const float &_R,  const MeshPotato::MPUtils::MPVec3 &_C, const MeshPotato::MPNoise::Noise_t &_noiseparms);
				PyroclasticSphere(const float &_R, const MeshPotato::MPUtils::MPVec3 &_C, const MeshPotato::MPNoise::Noise_t &_noiseparms);
				~PyroclasticSphere();
				const volumeDataType eval(const MeshPotato::MPUtils::MPVec3 &P) const;
				const volumeGradType grad(const MeshPotato::MPUtils::MPVec3& P) const;
			private:
				class Impl;
				boost::shared_ptr<Impl> mImpl;
		};
		class Cumulo : public ImplicitShape {
			public:
				static boost::shared_ptr<Cumulo> Ptr(VolumeFloatPtr  &_f,  MPNoise::Noise_t &_noiseparms, VolumeVectorPtr &Y);
				Cumulo(VolumeFloatPtr  &_f,  MPNoise::Noise_t &_noiseparms, VolumeVectorPtr &Y);
				~Cumulo();
				const volumeDataType eval(const MeshPotato::MPUtils::MPVec3 &P) const;
				const volumeGradType grad(const MeshPotato::MPUtils::MPVec3& P) const;
			private:
				class Impl;
				boost::shared_ptr<Impl> mImpl;
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
