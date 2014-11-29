//*******************************************************************
//
//   Color.h
//
// 4D color class in the namespace MeshPotato::MPUtils
//
//
//
//*******************************************************************

#ifndef __COLOR_H__
#define __COLOR_H__
#include <iostream>
#include <boost/shared_ptr.hpp>

namespace MeshPotato {
	namespace MPUtils 
	{
		//class Color;
		//std::ostream& operator<<(std::ostream& out, const MeshPotato::MPUtils::Color& col);
		class Color
		{
			public:

				Color();

				Color(const Color& v);

				Color(const double a, const double b, const double c, const double d);

				~Color();

				//!  Set all four components
				void set( const float vx, const float vy, const float vz, const float vw );

				//! Add two colors together
				const Color operator+(const Color& v) const ;

				//! Subtract one color from another
				const Color operator-(const Color& v) const;

				//! Unary minus
				friend const Color operator- (const Color& v);

				//! Multiplication of a constant with a Color 
				friend const Color operator* (const double w, const Color& v);

				//! Multiplication of a Color with a constant
				const Color operator*(const double v) const;

				const Color operator/(const double v) const;

				//! component product
				const Color operator*(const Color& v) const ;

				Color& operator=(const Color& v);

				Color& operator+=(const Color& v);

				Color& operator-=(const Color& v);

				Color& operator*=(const double v);

				Color& operator/=(const double v);


				const double& operator[] (const int v) const;
				double& operator[] (const int v);
				const double& operator() (const int v) const;

				const double X() const;
				const double Y() const;
				const double Z() const;
				const double W() const;

				//  Comparisons

				const bool operator==(const Color& v) const;

				const bool operator!=(const Color& v) const;
				//  friend std::ostream& operator<<(std::ostream& os, const Color& col); 
			private:
				class Impl;
				boost::shared_ptr<Impl> mImpl;
		};
		std::ostream& operator<<(std::ostream& out, const Color& col); 

	}

}

#endif
