

#include "MPUtils/Camera.h"
#include "MPUtils/Vector.h"
namespace MeshPotato {
namespace MPUtils {

MeshPotato::MPUtils::Camera::Camera()
{
   setEyeViewUp( MeshPotato::MPUtils::MPVec3(0,0,1), MeshPotato::MPUtils::MPVec3(0,0,-1), MeshPotato::MPUtils::MPVec3(0,1,0) );
   setFov( 60.0 );
   setAspectRatio( 16.0/9.0 );
   setNearPlane(0.0);
   setFarPlane(1.0e6);
}

void MeshPotato::MPUtils::Camera::setEyeViewUp( const MeshPotato::MPUtils::MPVec3& eye, const MeshPotato::MPUtils::MPVec3& view, const MeshPotato::MPUtils::MPVec3& up )
{
   position = eye;
   axis_view = view.unit();
   axis_up = ( up - (up.dot(axis_view)) * axis_view ).unit();
   axis_right = (axis_view.cross(axis_up)).unit();
}

// view direction of a pixel at the fractional position x,y.
// Nominally 0 <= x <= 1 and 0 <= y <= 1 for the primary fov,
// but the values can extend beyond that
const MeshPotato::MPUtils::MPVec3 Camera::view( const double x, const double y )
{
   double xx = (2.0*x-1.0)*htanfov;
   double yy = (2.0*y-1.0)*vtanfov;
   return (axis_up * yy + axis_right * xx + axis_view).unit();
}

void MeshPotato::MPUtils::Camera::setFov( const double fov )
{
   FOV = fov;
   htanfov = tan( FOV*0.5*M_PI/180.0 );
   vtanfov = htanfov/aspect_ratio;
}

void MeshPotato::MPUtils::Camera::setAspectRatio( const double ar )
{
   aspect_ratio = ar;
   vtanfov = htanfov/aspect_ratio;
}

openvdb::math::Transform::Ptr MeshPotato::MPUtils::Camera::createFrustumTransform(openvdb::BBoxd bbox) {
   // Calculate taper from far and near plane distances
   // Calculate xWidth by calculating the difference between the right and left values of the near plane in world space
   float farPlaneWidth = 2*far*htanfov;
   float farPlaneHeight = 2*far*vtanfov;
   float nearPlaneWidth = 2*near*htanfov;
   float nearPlaneHeight = 2*near*vtanfov;
   double taper = nearPlaneWidth/farPlaneWidth;
   double depth = (far - near)/(nearPlaneWidth);
   MPVec3 topRight = position + view(1,1)*sqrt(far*far + (farPlaneWidth/2.0)*(farPlaneWidth/2.0));
   topRight = MPVec3(farPlaneWidth/2.0, farPlaneHeight/2.0, -far);
   MPVec3 lowerLeft = position + view(0,0)*sqrt(near*near + (nearPlaneWidth/2.0)*(nearPlaneWidth/2.0));
   lowerLeft = MPVec3(-nearPlaneWidth/2.0, -nearPlaneHeight/2.0, -near);
  
//   float bbox_offset = taper/2.0;
//   topRight.x() -= bbox_offset;

   
//   openvdb::BBoxd bbox(lowerLeft, topRight);

   std::cout << "taper = " << taper << std::endl;
   std::cout << "depth = " << depth << std::endl;
   std::cout << "bbox = " << bbox << std::endl;
   std::cout << "nearPlaneWidth/2 = " << nearPlaneWidth/2.0 << std::endl;
   std::cout << "farPlaneWidth = " << farPlaneWidth << std::endl;
   std::cout << "nearPlaneWidth = " << nearPlaneWidth << std::endl;
   std::cout << "topRight = " << topRight << std::endl;
   std::cout << "lowerLeft = " << lowerLeft << std::endl;
   std::cout << "view(1,1) = " << view(1,1) << std::endl;
   std::cout << "view(0,0) = " << view(0,0) << std::endl;
   std::cout << "view(0.5,0.5) = " << view(0.5,0.5) << std::endl;
   std::cout << "view(1,0.5) = " << view(1,0.5) << std::endl;
   std::cout << "view(1,0) = " << view(1,0) << std::endl;
   std::cout << "view(0,1) = " << view(0,1) << std::endl;
   std::cout << "axis_up = " << axis_up << std::endl;
   std::cout << "axis_right = " << axis_right << std::endl;
   std::cout << "axis_view = " << axis_view << std::endl;

   if(axis_view.dot(MPVec3(0,0,1)) > 0) {
      depth *= -1;
   }
   
   openvdb::math::Transform::Ptr frustumTransform = openvdb::math::Transform::createFrustumTransform(bbox, taper, depth, nearPlaneWidth);
      frustumTransform->postTranslate( position + (axis_view * near));


//if (axis_view.dot(MPVec3(0,0,1)) < 0) {

//   frustumTransform->postRotate(axis_view.z()*M_PI, openvdb::math::X_AXIS); // X
  // frustumTransform->postRotate(M_PI, openvdb::math::X_AXIS); // Y
//      frustumTransform->postTranslate( position + (axis_view * near));
//   frustumTransform->postRotate(axis_view.x()*M_PI, openvdb::math::Y_AXIS); // Z
//}



   //frustumTransform->postRotate();
   return frustumTransform;



}
}}
