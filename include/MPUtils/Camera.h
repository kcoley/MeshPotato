/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
///  Simple Camera Class
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
    /// view direction of a pixel at the fractional position x,y.
    /// Nominally 0 <= x <= 1 and 0 <= y <= 1 for the primary fov,
    /// but the values can extend beyond that
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


