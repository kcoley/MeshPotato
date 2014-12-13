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
