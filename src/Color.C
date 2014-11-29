#include "MPUtils/Color.h"
namespace MeshPotato {
namespace MPUtils {
	class Color::Impl {
	public:
		double xyzw[4];
	};

 std::ostream& operator<<(std::ostream& out, const Color& col) {
        out << "[" << col.X() << ", " << col.Y() << ", " << col.Z() << ", " << col.W() << "]";
        return out;
  }


	Color::Color() : mImpl(new Impl()) { mImpl->xyzw[0] = mImpl->xyzw[1] = mImpl->xyzw[2] = mImpl->xyzw[3] = 0; }

	Color::Color(const Color& v) : mImpl(new Impl())
	{ 
		mImpl->xyzw[0] = v.mImpl->xyzw[0];
		mImpl->xyzw[1] = v.mImpl->xyzw[1];
		mImpl->xyzw[2] = v.mImpl->xyzw[2]; 
		mImpl->xyzw[3] = v.mImpl->xyzw[3]; 
	}

	Color::Color(const double a, const double b, const double c, const double d) : mImpl(new Impl())
	{
		mImpl->xyzw[0] = a;
		mImpl->xyzw[1] = b;
		mImpl->xyzw[2] = c; 
		mImpl->xyzw[3] = d; 
	}

	Color::~Color(){}

	//!  Set all four components
	void Color::set( const float vx, const float vy, const float vz, const float vw )
	{
		mImpl->xyzw[0] = vx;
		mImpl->xyzw[1] = vy;
		mImpl->xyzw[2] = vz;
		mImpl->xyzw[3] = vw;
	}

	//! Add two colors together
	const Color Color::operator+(const Color& v) const 
	{ 
		return Color(mImpl->xyzw[0]+v.mImpl->xyzw[0], mImpl->xyzw[1]+v.mImpl->xyzw[1], mImpl->xyzw[2]+v.mImpl->xyzw[2], mImpl->xyzw[3]+v.mImpl->xyzw[3]); 
	}

	//! Subtract one color from another
	const Color Color::operator-(const Color& v) const
	{ 
		return Color(mImpl->xyzw[0]-v.mImpl->xyzw[0], mImpl->xyzw[1]-v.mImpl->xyzw[1], mImpl->xyzw[2]-v.mImpl->xyzw[2], mImpl->xyzw[3]-v.mImpl->xyzw[3]); 
	}

	//! Unary minus
	const Color operator-(const Color& v)
	{ return Color(-v.mImpl->xyzw[0],-v.mImpl->xyzw[1],-v.mImpl->xyzw[2], -v.mImpl->xyzw[3]); }

	//! Multiplication of a constant with a Color 
	const Color operator*(const double w, const Color& v)
	{ return v*w; }

	//! Multiplication of a Color with a constant
	const Color Color::operator*(const double v) const
	{ return Color(mImpl->xyzw[0]*v, mImpl->xyzw[1]*v, mImpl->xyzw[2]*v, mImpl->xyzw[3]*v); }

	const Color Color::operator/(const double v) const
	{ return Color(mImpl->xyzw[0]/v, mImpl->xyzw[1]/v, mImpl->xyzw[2]/v, mImpl->xyzw[3]/v); }

	//! component product
	const Color Color::operator*(const Color& v) const  
	{ return Color(mImpl->xyzw[0]*v.mImpl->xyzw[0] , mImpl->xyzw[1]*v.mImpl->xyzw[1] , mImpl->xyzw[2]*v.mImpl->xyzw[2] , mImpl->xyzw[3]*v.mImpl->xyzw[3]); }

	Color& Color::operator=(const Color& v)
	{ mImpl->xyzw[0] = v.mImpl->xyzw[0]; mImpl->xyzw[1] = v.mImpl->xyzw[1]; mImpl->xyzw[2] = v.mImpl->xyzw[2]; mImpl->xyzw[3]=v.mImpl->xyzw[3]; return *this; }

	Color& Color::operator+=(const Color& v)
	{ mImpl->xyzw[0] += v.mImpl->xyzw[0]; mImpl->xyzw[1] += v.mImpl->xyzw[1]; mImpl->xyzw[2] += v.mImpl->xyzw[2]; mImpl->xyzw[3] += v.mImpl->xyzw[3]; return *this; }

	Color& Color::operator-=(const Color& v)
	{ mImpl->xyzw[0] -= v.mImpl->xyzw[0]; mImpl->xyzw[1] -= v.mImpl->xyzw[1]; mImpl->xyzw[2] -= v.mImpl->xyzw[2]; mImpl->xyzw[3] -= v.mImpl->xyzw[3]; return *this; }

	Color& Color::operator*=(const double v)
	{ mImpl->xyzw[0] *= v; mImpl->xyzw[1] *= v; mImpl->xyzw[2] *= v; mImpl->xyzw[3] *= v; return *this; }

	Color& Color::operator/=(const double v)
	{ mImpl->xyzw[0] /= v; mImpl->xyzw[1] /= v; mImpl->xyzw[2] /= v; mImpl->xyzw[3] /= v; return *this; }


	const double& Color::operator[](const int v) const { return mImpl->xyzw[v]; }
	double& Color::operator[](const int v) { return mImpl->xyzw[v]; }
	const double& Color::operator() (const int v) const { return mImpl->xyzw[v]; }

	const double Color::X() const { return mImpl->xyzw[0]; }
	const double Color::Y() const { return mImpl->xyzw[1]; }
	const double Color::Z() const { return mImpl->xyzw[2]; }
	const double Color::W() const { return mImpl->xyzw[3]; }

	//  Comparisons

	const bool Color::operator==(const Color& v) const
	{ return ( mImpl->xyzw[0]==v.mImpl->xyzw[0] && mImpl->xyzw[1]==v.mImpl->xyzw[1] && mImpl->xyzw[2]==v.mImpl->xyzw[2] && mImpl->xyzw[3] == v.mImpl->xyzw[3] ); }

	const bool Color::operator!=(const Color& v) const
	{ return ( mImpl->xyzw[0]!=v.mImpl->xyzw[0] || mImpl->xyzw[1]!=v.mImpl->xyzw[1] || mImpl->xyzw[2]!=v.mImpl->xyzw[2] || mImpl->xyzw[3] != v.mImpl->xyzw[3] ); }
				//  friend std::ostream& operator<<(std::ostream& os, const Color& col); 
			
















}}
