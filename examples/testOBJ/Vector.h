//*******************************************************************
//
//   Vector.h
//
// 3D vector class in the namespace lux
//
//
//
//*******************************************************************

#ifndef __LUX_VECTOR_H__
#define __LUX_VECTOR_H__

#include <cmath>

namespace lux
{

//! Vector is a 3D vector class
class Vector
{
  public:

   Vector(){ xyz[0] = xyz[1] = xyz[2] = 0; }

   Vector(const Vector& v)
   { 
      xyz[0] = v.xyz[0];
      xyz[1] = v.xyz[1];
      xyz[2] = v.xyz[2]; 
   }
   
   Vector(const double a, const double b, const double c)
   {
      xyz[0] = a;
      xyz[1] = b;
      xyz[2] = c; 
   }

   ~Vector(){}

   //!  Set all three components
   void set( const float vx, const float vy, const float vz )
   {
      xyz[0] = vx;
      xyz[1] = vy;
      xyz[2] = vz;
   }

   //! Add two vectors together
   const Vector operator+        (const Vector& v) const 
   { 
      return Vector(xyz[0]+v.xyz[0], xyz[1]+v.xyz[1], xyz[2]+v.xyz[2]); 
   }
  
   //! Subtract one vector from another
   const Vector operator-        (const Vector& v) const
   { 
      return Vector(xyz[0]-v.xyz[0], xyz[1]-v.xyz[1], xyz[2]-v.xyz[2]); 
   }

   //! Unary minus
   friend const Vector operator- (const Vector& v)
   { return Vector(-v.xyz[0],-v.xyz[1],-v.xyz[2]); }

   //! Multiplication of a constant with a vector
   friend const Vector operator* (const double w, const Vector& v)
   { return v*w; }
	  
   //! Multiplication of a vector with a constant
   const Vector operator*        (const double v) const
   { return Vector(xyz[0]*v, xyz[1]*v, xyz[2]*v); }

   const Vector operator/        (const double v) const
   { return Vector(xyz[0]/v, xyz[1]/v, xyz[2]/v); }

   //! Inner product
   const double operator*        (const Vector& v) const  
   { return (xyz[0]*v.xyz[0] + xyz[1]*v.xyz[1] + xyz[2]*v.xyz[2]); }
  
   //! cross product
   const Vector operator^        (const Vector& v) const 
   { return Vector(xyz[1]*v.xyz[2] - xyz[2]*v.xyz[1], 
		   xyz[2]*v.xyz[0] - xyz[0]*v.xyz[2], 
		   xyz[0]*v.xyz[1] - xyz[1]*v.xyz[0]); }

   Vector& operator=       (const Vector& v)
   { xyz[0] = v.xyz[0]; xyz[1] = v.xyz[1]; xyz[2] = v.xyz[2]; return *this; }
  
   Vector& operator+=      (const Vector& v)
   { xyz[0] += v.xyz[0]; xyz[1] += v.xyz[1]; xyz[2] += v.xyz[2]; return *this; }
  
   Vector& operator-=      (const Vector& v)
   { xyz[0] -= v.xyz[0]; xyz[1] -= v.xyz[1]; xyz[2] -= v.xyz[2]; return *this; }
  
   Vector& operator*=      (const double v)
   { xyz[0] *= v; xyz[1] *= v; xyz[2] *= v; return *this; }
  
   Vector& operator/=      (const double v)
   { xyz[0] /= v; xyz[1] /= v; xyz[2] /= v; return *this; }
  

   const double& operator[] (const int v) const { return xyz[v]; }
         double& operator[] (const int v)       { return xyz[v]; }
   const double& operator() (const int v) const { return xyz[v]; }

   const double X() const { return xyz[0]; }
   const double Y() const { return xyz[1]; }
   const double Z() const { return xyz[2]; }

   const double magnitude() const 
   { return sqrt( xyz[0]*xyz[0] + xyz[1]*xyz[1] + xyz[2]*xyz[2] ); }
   
   const Vector unitvector() const { return *this/magnitude(); }

   void normalize() 
   { double mag = magnitude(); xyz[0] /= mag; xyz[1] /= mag; xyz[2] /= mag; }

//  Comparisons

   const bool operator==         (const Vector& v) const
       { return ( xyz[0]==v.xyz[0] && xyz[1]==v.xyz[1] && xyz[2]==v.xyz[2] ); }
  
   const bool operator!=         (const Vector& v) const
       { return ( xyz[0]!=v.xyz[0] || xyz[1]!=v.xyz[1] || xyz[2]!=v.xyz[2] ); }
  
   const bool operator<          (const Vector& v) const
       { return ( magnitude() < v.magnitude() ); }
  
   const bool operator<=         (const Vector& v) const
       { return ( magnitude() <= v.magnitude() ); }
  
   const bool operator>          (const Vector& v) const
       { return ( magnitude() > v.magnitude() ); }
  
   const bool operator>=         (const Vector& v) const
       { return ( magnitude() >= v.magnitude() ); }

   // Test for parallel
   const bool operator||         (const Vector& v) const
       { return (  fabs((*this)*v) == v.magnitude()*((*this).magnitude()) ); }
   
  private:
  double xyz[3];
};

}



#endif
