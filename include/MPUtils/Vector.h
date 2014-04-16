#include <vector>
#include <openvdb/math/Vec3.h>
#include <openvdb/math/Mat3.h>
#include <openvdb/math/Vec4.h>
#include <openvdb/util/Util.h>
#include <openvdb/openvdb.h>
#include <openvdb/tools/MeshToVolume.h>
#include <openvdb/math/Ray.h>
#include <OpenEXR/ImathVec.h>

namespace MeshPotato {
	//! Utilities 
	namespace MPUtils {
		//! MPUtilities
		typedef openvdb::math::Vec3d  MPVec3;
		typedef openvdb::math::Mat3<float> MPMat3;
		typedef openvdb::math::Mat4d MPMat4;
		typedef openvdb::math::Ray<double> MPRay;	
		typedef openvdb::BBoxd BBox;		
		typedef openvdb::Coord Coord;		
		typedef std::vector<openvdb::math::Vec4<uint32_t> > Polys;
		typedef std::vector<openvdb::math::Vec3<float> >Verts;
		

	}
}
