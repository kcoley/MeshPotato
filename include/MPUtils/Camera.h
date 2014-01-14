
#ifndef __CAMERA_H__
#define __CAMERA_H__


#include "MPUtils/Vector.h"
namespace MeshPotato {
namespace MPUtils {
class Camera
{

  public:

    Camera();
   ~Camera(){}

    void setEyeViewUp( const MeshPotato::MPUtils::MPVec3& eye, const MeshPotato::MPUtils::MPVec3& view, const MeshPotato::MPUtils::MPVec3& up );
    const MeshPotato::MPUtils::MPVec3& eye() const  { return position; }
    const MeshPotato::MPUtils::MPVec3& view() const { return axis_view; }
    const MeshPotato::MPUtils::MPVec3& up() const   { return axis_up; }

    // view direction of a pixel at the fractional position x,y.
    // Nominally 0 <= x <= 1 and 0 <= y <= 1 for the primary fov,
    // but the values can extend beyond that
    const MeshPotato::MPUtils::MPVec3 view( const double x, const double y );

    void setFov( const double fov );
    const double& fov() const { return FOV; }

    void setAspectRatio( const double ar );
    const double& aspectRatio() const { return aspect_ratio; }

    void setNearPlane( const double n ){ near = n; }
    const double& nearPlane() const { return near; }

    void setFarPlane( const double n ){ far = n; }
    const double& farPlane() const { return far; }

  private:


    
    double FOV, aspect_ratio;
    double htanfov, vtanfov;
    double near, far;

    MeshPotato::MPUtils::MPVec3 position;
    MeshPotato::MPUtils::MPVec3 axis_right, axis_up, axis_view;



};
}}
#endif


