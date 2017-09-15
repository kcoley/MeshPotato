#include "MeshPotato/MPVolume/Transformation.h" 

namespace MeshPotato {
	namespace MPVolume {
		template <typename T>
		class TranslateVolume<T>::Impl {
		public:
			Impl(const boost::shared_ptr<Volume<T> > &_f,
				 const MeshPotato::MPUtils::MPVec3 &_D) : f(_f), D(_D) {}

			boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > f;
			MeshPotato::MPUtils::MPVec3 D;
		};

		template <typename T>
		boost::shared_ptr<Volume<T> > TranslateVolume<T>::Ptr(const boost::shared_ptr<Volume<T> > &_f,
							 const MeshPotato::MPUtils::MPVec3 &_D) {
		    return boost::shared_ptr<TranslateVolume<T> >(new TranslateVolume<T>(_f, _D)); }

		template <typename T>
		TranslateVolume<T>::TranslateVolume(const boost::shared_ptr<Volume<T> > &_f,
											const MeshPotato::MPUtils::MPVec3 &_D) : 
												mImpl(new Impl(_f, _D)) {}

		template <typename T>
		const T TranslateVolume<T>::eval(const MeshPotato::MPUtils::MPVec3 &P) const {
		    return mImpl->f->eval(P - mImpl->D);
		}

		template <typename T>
		const typename TranslateVolume<T>::volumeGradType TranslateVolume<T>::grad(const MeshPotato::MPUtils::MPVec3& P) const {}

		template class TranslateVolume<float>;
		template class TranslateVolume<MPUtils::MPVec3>;
	

		template <typename T>
		class RotateVolume<T>::Impl {
		public:
			Impl(const boost::shared_ptr<Volume<T> > &_f,
				 const MeshPotato::MPUtils::MPVec3 &axis,
				 const MeshPotato::MPUtils::MPVec3 &angle) : f(_f), matrix()  {
				matrix.setToRotation(axis, angle);
			}

			boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > f;
			MeshPotato::MPUtils::MPMat4 matrix;
		};

		template <typename T>
		boost::shared_ptr<Volume<T> > RotateVolume<T>::Ptr(const boost::shared_ptr<Volume<T> > &_f,
							 const MeshPotato::MPUtils::MPVec3 &_axis,
							 const MeshPotato::MPUtils::MPVec3 &_angle) {
		    return boost::shared_ptr<RotateVolume<T> >(new RotateVolume<T>(_f, _axis, _angle)); }

		template <typename T>
		RotateVolume<T>::RotateVolume(const boost::shared_ptr<Volume<T> > &_f,
									  const MeshPotato::MPUtils::MPVec3 &_axis,
									  const MeshPotato::MPUtils::MPVec3 &_angle) : 
												mImpl(new Impl(_f, _axis, _angle)) {}

		template <typename T>
		const T RotateVolume<T>::eval(const MeshPotato::MPUtils::MPVec3 &P) const {
		    return mImpl->f->eval(mImpl->matrix.inverse().transform(P));
		}

		template <typename T>
		const typename RotateVolume<T>::volumeGradType RotateVolume<T>::grad(const MeshPotato::MPUtils::MPVec3& P) const {}

		template class RotateVolume<float>;


		template <typename T>
		class ScaleVolume<T>::Impl {
		public:
			Impl(const boost::shared_ptr<Volume<T> > &_f,
				 const float scale) : f(_f), invScale(1/scale)  {
				
			}

			boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > f;
			float invScale;
		};

		template <typename T>
		boost::shared_ptr<Volume<T> > ScaleVolume<T>::Ptr(
				const boost::shared_ptr<Volume<T> > &_f,
				const float scale) {
		    return boost::shared_ptr<ScaleVolume<T> >(new ScaleVolume<T>(_f, scale)); }

		template <typename T>
		ScaleVolume<T>::ScaleVolume(const boost::shared_ptr<Volume<T> > &_f,
									  const float scale) : 
												mImpl(new Impl(_f, scale)) {}

		template <typename T>
		const T ScaleVolume<T>::eval(const MeshPotato::MPUtils::MPVec3 &P) const {
		    return mImpl->f->eval(P * mImpl->invScale);
		}

		template <typename T>
		const typename ScaleVolume<T>::volumeGradType ScaleVolume<T>::grad(const MeshPotato::MPUtils::MPVec3& P) const {}

		template class ScaleVolume<float>;
	}
}