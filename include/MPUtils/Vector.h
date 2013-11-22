#include <vector>
#include <openvdb/math/Vec3.h>
#include <openvdb/math/Mat3.h>
#include <openvdb/math/Vec4.h>
#include <openvdb/util/Util.h>
#include <openvdb/openvdb.h>
#include <openvdb/tools/MeshToVolume.h>

#include <OpenEXR/ImathVec.h>

namespace MeshPotato {
	//! Utilities 
	namespace MPUtils {
		//! MPUtilities
		typedef openvdb::math::Vec3<float> MPVec3;
		typedef openvdb::math::Mat3<float> MPMat3;
		typedef Imath::Vec3<float> Vector2;
	
		
		typedef std::vector<openvdb::math::Vec4<uint32_t> > Polys;
		typedef std::vector<openvdb::math::Vec3<float> >Verts;
		

	}
}
