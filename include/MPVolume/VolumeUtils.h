#include "MPVolume.h"
#include "FrustumGrid.h"
namespace MeshPotato {
	namespace MPVolume {
		openvdb::FloatGrid::Ptr makeVDBGrid(boost::shared_ptr<Volume<float> > &mpgrid, const openvdb::CoordBBox& indexBB, double h) {
			openvdb::FloatGrid::Ptr vdbgrid = openvdb::FloatGrid::create();
			openvdb::FloatGrid::Accessor accessor = vdbgrid->getAccessor();
			for (openvdb::Int32 i = indexBB.min().x(); i <= indexBB.max().x(); ++i) {
				for (openvdb::Int32 j = indexBB.min().y(); j <= indexBB.max().y(); ++j) {
					for (openvdb::Int32 k = indexBB.min().z(); k <= indexBB.max().z(); ++k) {
						// transform point (i, j, k) of index space into world space
						MeshPotato::MPUtils::MPVec3 p(i * h, j * h, k * h);
						// compute level set function value
						MeshPotato::MPUtils::MPVec3 P(p.x(), p.y(),p.z());
						float value = -mpgrid->eval(P);

						accessor.setValue(openvdb::Coord(i, j, k), value);
					}
				}
			}
			vdbgrid->setTransform(openvdb::math::Transform::createLinearTransform(h));
			return vdbgrid;
		}
/*
		//! Start from the near plane of the frustum grid.  Calculate a ray to the far plane for each cell of the frustum
		void dsm(openvdb::tools::PerspectiveCamera &vdbcam, FrustumGrid *grid, VolumeFloatPtr f) {
			// Get dimensions of the frustum grid by getting it's axis-aligned bounding box
			openvdb::CoordBBox bbox = grid->getBoundingBox();

			int xSize = bbox.max().x() - bbox.min.x();
			int ySize = bbox.max().y() - bbox.min.y();
			int zSize = bbox.max().z() - bbox.min.z();
			for (int j = 0; j < ySize; ++j) {
				for (int i = 0; i < xSize; ++i) {
					float x = (float)i/(xSize - 1);
					float y = (float)j/(ySize - 1);
					MeshPotato::MPUtils::Ray ray = vdbcam->getRay(i,j);
					float deltaS; // (farPlane - nearPlane)/(cam->view*d))/(zSize - 1)	
					
					for (int k = 0; k < zSize; ++k) {
						float val = f->eval(X);
						if (val > 0) {
							value += val * deltaS;
						}
						grid->set(ijk, value);
					}
				}

			}	

		}
*/
	}
} 
