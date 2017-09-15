/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
/// Class for translating a volume
///
///   Copyright 2014 Kacey Coley
///   Licensed under the Apache License, Version 2.0 (the "License");
///   you may not use this file except in compliance with the License.
///   You may obtain a copy of the License at
///
///   http://www.apache.org/licenses/LICENSE-2.0
///
///   Unless required by applicable law or agreed to in writing, software
///   distributed under the License is distributed on an "AS IS" BASIS,
///   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
///   See the License for the specific language governing permissions and
///   limitations under the License.
///

#include "MPVolume.h"

namespace MeshPotato {
    namespace MPVolume {
    	/// Translates a volume
        template <typename T>
	    class TranslateVolume : public Volume<T> {
	    public:
	        typedef typename GradType<T>::GType volumeGradType;
			
			static boost::shared_ptr<Volume<T> > Ptr(const boost::shared_ptr<Volume<T> > &_f,
							 const MeshPotato::MPUtils::MPVec3 &_D);

			TranslateVolume(const boost::shared_ptr<Volume<T> > &_f,
							const MeshPotato::MPUtils::MPVec3 &_D);

			virtual const T eval(const MeshPotato::MPUtils::MPVec3 &P) const;

			virtual const volumeGradType grad(const MeshPotato::MPUtils::MPVec3& P) const;
		private:
			class Impl;
			boost::shared_ptr<Impl> mImpl;
	   };

	   typedef TranslateVolume<float> TranslateVolumeFloat;
	   typedef TranslateVolume<MPUtils::MPVec3> TranslateVolumeVector;

	   /// Rotates a volume
	   template <typename T>
	   class RotateVolume: public Volume<T> {
	   public:
	   	typedef typename GradType<T>::GType volumeGradType;

	   	static boost::shared_ptr<Volume<T> > Ptr(const boost::shared_ptr<Volume<T> > &_f,
	   						const MeshPotato::MPUtils::MPVec3 &axis,
	   						const MeshPotato::MPUtils::MPVec3 &angle);

	   	RotateVolume(const boost::shared_ptr<Volume<T> > &f,
	   				 const MeshPotato::MPUtils::MPVec3 &axis,
	   				 const MeshPotato::MPUtils::MPVec3 &angle);

	   	virtual const T eval (const MeshPotato::MPUtils::MPVec3 &P) const;

	   	virtual const volumeGradType grad(const MeshPotato::MPUtils::MPVec3 &P) const;
	   private:
	   	class Impl;
	   	boost::shared_ptr<Impl> mImpl;
	   };

	   typedef RotateVolume<float> RotateVolumeFloat;

	   	/// Scales a volume
	    template <typename T>
	    class ScaleVolume : public Volume<T> {
	    public:
	        typedef typename GradType<T>::GType volumeGradType;
			
			static boost::shared_ptr<Volume<T> > Ptr(
						const boost::shared_ptr<Volume<T> > &f,
						const float scale);

			ScaleVolume(const boost::shared_ptr<Volume<T> > &f,
						const float scale);

			virtual const T eval(const MeshPotato::MPUtils::MPVec3 &P) const;

			virtual const volumeGradType grad(
						const MeshPotato::MPUtils::MPVec3& P) const;
		private:
			class Impl;
			boost::shared_ptr<Impl> mImpl;
	   };

	   typedef ScaleVolume<float> ScaleVolumeFloat;

    }
}
