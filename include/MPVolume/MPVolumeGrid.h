#include "MPVolume.h"

namespace MeshPotato {
	namespace MPVolumeGrid {
	template<typename T>
	class MPVolumeGrid: public MPVolume<T> {
		public:
			MPVolumeGrid() {}
			virtual ~MPVolumeGrid() {}
			virtual const MeshPotato::MPUtils::MPVec3 evalP(float i, float j, float k) const {}
	};

	}
}
