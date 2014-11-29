
#ifndef __CAMERA_H__
#define __CAMERA_H__


#include "MPUtils/Vector.h"
#include <openvdb/Grid.h>
namespace MeshPotato {
namespace MPUtils {
class Camera
{

  public:

    Camera(const MeshPotato::MPUtils::MPVec3& eye, const MeshPotato::MPUtils::MPVec3& view, const MeshPotato::MPUtils::MPVec3& up );
    Camera();
   ~Camera();
    static boost::shared_ptr<Camera> Ptr();
    void setEyeViewUp( const MeshPotato::MPUtils::MPVec3& eye, const MeshPotato::MPUtils::MPVec3& view, const MeshPotato::MPUtils::MPVec3& up );
    const MeshPotato::MPUtils::MPVec3& eye() const;
    const MeshPotato::MPUtils::MPVec3& view() const;
    const MeshPotato::MPUtils::MPVec3& up() const;
    const MeshPotato::MPUtils::MPRay getRay(const double i, const double j)  const;
    // view direction of a pixel at the fractional position x,y.
    // Nominally 0 <= x <= 1 and 0 <= y <= 1 for the primary fov,
    // but the values can extend beyond that
    const MeshPotato::MPUtils::MPVec3 view( const double x, const double y ) const;

    void setFov( const double fov );
    const double& fov() const;

    void setAspectRatio( const double ar );
    const double& aspectRatio() const;

    void setNearPlane( const double n );
    const double& nearPlane() const;

    void setFarPlane( const double n );
    const double& farPlane() const;
    openvdb::math::Transform::Ptr createFrustumTransform(openvdb::BBoxd bbox);


  private:
    class Impl;
    boost::shared_ptr<Impl> mImpl;
   


};
}}
#endif


