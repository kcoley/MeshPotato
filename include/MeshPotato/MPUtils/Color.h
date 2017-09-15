/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
///  4D color class
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


#ifndef __COLOR_H__
#define __COLOR_H__
#include <iostream>
#include <boost/shared_ptr.hpp>

namespace MeshPotato {
	namespace MPUtils 
	{
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
