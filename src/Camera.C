#include "MPUtils/Camera.h"
#include "MPUtils/Vector.h"
namespace MeshPotato {
namespace MPUtils {
   class Camera::Impl {
   public:
      Impl() {
         
      }
      double FOV, aspect_ratio;
    double htanfov, vtanfov;
    double near, far;

    MeshPotato::MPUtils::MPVec3 position;
    MeshPotato::MPUtils::MPVec3 axis_right, axis_up, axis_view;
   };

MeshPotato::MPUtils::Camera::Camera() : mImpl(new Impl()) {
   setEyeViewUp( MeshPotato::MPUtils::MPVec3(0,0,1), MeshPotato::MPUtils::MPVec3(0,0,-1), MeshPotato::MPUtils::MPVec3(0,1,0) );
         setFov( 60.0 );
         setAspectRatio( 16.0/9.0 );
         setNearPlane(0.0);
         setFarPlane(1.0e6);

}
Camera::~Camera() {}

boost::shared_ptr<Camera> Camera::Ptr() { return boost::shared_ptr<Camera>(new Camera());}

const MeshPotato::MPUtils::MPVec3& Camera::eye() const  { return mImpl->position; }
const MeshPotato::MPUtils::MPVec3& Camera::view() const { return mImpl->axis_view; }
const MeshPotato::MPUtils::MPVec3& Camera::up() const   { return mImpl->axis_up; }
const MeshPotato::MPUtils::MPRay Camera::getRay(const double i, const double j)  const {
   MPVec3 temp = view(i,j).unit();

   return MeshPotato::MPUtils::MPRay(mImpl->position,temp);  
}


const double& Camera::fov() const { return mImpl->FOV; }


const double& Camera::aspectRatio() const { return mImpl->aspect_ratio; }

void Camera::setNearPlane( const double n ){ mImpl->near = n; }

const double& Camera::nearPlane() const { return mImpl->near; }

void Camera::setFarPlane( const double n ){ mImpl->far = n; }

const double& Camera::farPlane() const { return mImpl->far; }

void MeshPotato::MPUtils::Camera::setEyeViewUp( const MeshPotato::MPUtils::MPVec3& eye, const MeshPotato::MPUtils::MPVec3& view, const MeshPotato::MPUtils::MPVec3& up )
{
   mImpl->position = eye;
   mImpl->axis_view = view.unit();
   mImpl->axis_up = ( up - (up.dot(mImpl->axis_view)) * mImpl->axis_view ).unit();
   mImpl->axis_right = (mImpl->axis_view.cross(mImpl->axis_up)).unit();
}

// view direction of a pixel at the fractional position x,y.
// Nominally 0 <= x <= 1 and 0 <= y <= 1 for the primary fov,
// but the values can extend beyond that
const MeshPotato::MPUtils::MPVec3 Camera::view( const double x, const double y ) const
{
   double xx = (2.0*x-1.0)*mImpl->htanfov;
   double yy = (2.0*y-1.0)*mImpl->vtanfov;
   return (mImpl->axis_up * yy + mImpl->axis_right * xx + mImpl->axis_view).unit();
}

void MeshPotato::MPUtils::Camera::setFov( const double fov )
{
   mImpl->FOV = fov;
   mImpl->htanfov = tan( mImpl->FOV*0.5*M_PI/180.0 );
   mImpl->vtanfov = mImpl->htanfov/mImpl->aspect_ratio;
}

void MeshPotato::MPUtils::Camera::setAspectRatio( const double ar )
{
   mImpl->aspect_ratio = ar;
   mImpl->vtanfov = mImpl->htanfov/mImpl->aspect_ratio;
}

openvdb::math::Transform::Ptr MeshPotato::MPUtils::Camera::createFrustumTransform(openvdb::BBoxd bbox) {
   // Calculate taper from far and near plane distances
   // Calculate xWidth by calculating the difference between the right and left values of the near plane in world space
   float farPlaneWidth = 2*mImpl->far*mImpl->htanfov;
   float farPlaneHeight = 2*mImpl->far*mImpl->vtanfov;
   float nearPlaneWidth = 2*mImpl->near*mImpl->htanfov;
   float nearPlaneHeight = 2*mImpl->near*mImpl->vtanfov;
   double taper = nearPlaneWidth/farPlaneWidth;
   double depth = (mImpl->far - mImpl->near)/(nearPlaneWidth);
   MPVec3 topRight = mImpl->position + view(1,1)*sqrt(mImpl->far*mImpl->far + (farPlaneWidth/2.0)*(farPlaneWidth/2.0));
   topRight = MPVec3(farPlaneWidth/2.0, farPlaneHeight/2.0, -mImpl->far);
   MPVec3 lowerLeft = mImpl->position + view(0,0)*sqrt(mImpl->near*mImpl->near + (nearPlaneWidth/2.0)*(nearPlaneWidth/2.0));
   lowerLeft = MPVec3(-nearPlaneWidth/2.0, -nearPlaneHeight/2.0, -mImpl->near);
  
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
   std::cout << "axis_up = " << mImpl->axis_up << std::endl;
   std::cout << "axis_right = " << mImpl->axis_right << std::endl;
   std::cout << "axis_view = " << mImpl->axis_view << std::endl;

   if(mImpl->axis_view.dot(MPVec3(0,0,1)) > 0) {
      depth *= -1;
   }
   
   openvdb::math::Transform::Ptr frustumTransform = openvdb::math::Transform::createFrustumTransform(bbox, taper, depth, nearPlaneWidth);
      frustumTransform->postTranslate( mImpl->position + (mImpl->axis_view * mImpl->near));


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
