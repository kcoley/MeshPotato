


/* coherent noise function over 1, 2 or 3 dimensions */
/* (copyright Ken Perlin) */

#include <cstdlib>
#include <cstdio>
#include <cmath>

#include "MPNoise/PerlinNoise.h"
using namespace std;
namespace MeshPotato {
	namespace MPNoise {

#define BM 0xff

#define N 0x1000
#define NP 12   /* 2^N */
#define NM 0xfff

#define s_curve(t) ( t * t * (3. - 2. * t) )

#define lerp(t, a, b) ( a + t * (b - a) )

#define setup(i,b0,b1,r0,r1)\
		t = vec[i] + N;\
		b0 = ((int)t) & BM;\
		b1 = (b0+1) & BM;\
		r0 = t - (int)t;\
		r1 = r0 - 1.;

		const float PerlinNoise::eval( float arg ) const
		{
			int bx0, bx1;
			float rx0, rx1, sx, t, u, v, vec[1];

			vec[0] = arg;

			setup(0, bx0,bx1, rx0,rx1);

			sx = s_curve(rx0);

			u = rx0 * g1[ p[ bx0 ] ];
			v = rx1 * g1[ p[ bx1 ] ];

			return lerp(sx, u, v);
		}


		const float PerlinNoise::eval( const MeshPotato::MPUtils::MPVec3& vec) const
		{
			int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
			float rx0, rx1, ry0, ry1, rz0, rz1, sy, sz, a, b, c, d, t, u, v;
			register int i, j;

			setup(0, bx0,bx1, rx0,rx1);
			setup(1, by0,by1, ry0,ry1);
			setup(2, bz0,bz1, rz0,rz1);

			i = p[ bx0 ];
			j = p[ bx1 ];

			b00 = p[ i + by0 ];
			b10 = p[ j + by0 ];
			b01 = p[ i + by1 ];
			b11 = p[ j + by1 ];

			t  = s_curve(rx0);
			sy = s_curve(ry0);
			sz = s_curve(rz0);

#define at3(rx,ry,rz) ( rx * g3[i][0] + ry * g3[i][1] + rz * g3[i][2] )

			i = b00 + bz0; u = at3(rx0,ry0,rz0);
			i = b10 + bz0; v = at3(rx1,ry0,rz0);
			//q = g3[ b00 + bz0 ] ; u = at3(rx0,ry0,rz0);
			//q = g3[ b10 + bz0 ] ; v = at3(rx1,ry0,rz0);
			a = lerp(t, u, v);

			i = b01 + bz0; u = at3(rx0,ry1,rz0);
			i = b11 + bz0; v = at3(rx1,ry1,rz0);
			//q = g3[ b01 + bz0 ] ; u = at3(rx0,ry1,rz0);
			//q = g3[ b11 + bz0 ] ; v = at3(rx1,ry1,rz0);
			b = lerp(t, u, v);

			c = lerp(sy, a, b);

			i = b00 + bz1; u = at3(rx0,ry0,rz1);
			i = b10 + bz1; v = at3(rx1,ry0,rz1);
			//q = g3[ b00 + bz1 ] ; u = at3(rx0,ry0,rz1);
			//q = g3[ b10 + bz1 ] ; v = at3(rx1,ry0,rz1);
			a = lerp(t, u, v);

			i = b01 + bz1; u = at3(rx0,ry1,rz1);
			i = b11 + bz1; v = at3(rx1,ry1,rz1);
			//q = g3[ b01 + bz1 ] ; u = at3(rx0,ry1,rz1);
			//q = g3[ b11 + bz1 ] ; v = at3(rx1,ry1,rz1);
			b = lerp(t, u, v);

			d = lerp(sy, a, b);

			return lerp(sz, c, d);
		}

		static void normalize2(float v[2])
		{
			float s;

			s = sqrt(v[0] * v[0] + v[1] * v[1]);
			v[0] = v[0] / s;
			v[1] = v[1] / s;
		}

		static void normalize3(float v[3])
		{
			float s;

			s = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
			v[0] = v[0] / s;
			v[1] = v[1] / s;
			v[2] = v[2] / s;
		}

		void PerlinNoise::init(void)
		{
			int i, j, k;

			for (i = 0 ; i < B ; i++) {
				p[i] = i;

				g1[i] = (float)((random() % (B + B)) - B) / B;

				for (j = 0 ; j < 2 ; j++)
					g2[i][j] = (float)((random() % (B + B)) - B) / B;
				normalize2(g2[i]);

				for (j = 0 ; j < 3 ; j++)
					g3[i][j] = (float)((random() % (B + B)) - B) / B;
				normalize3(g3[i]);
			}

			while (--i) {
				k = p[i];
				p[i] = p[j = random() % B];
				p[j] = k;
			}

			for (i = 0 ; i < B + 2 ; i++) {
				p[B + i] = p[i];
				g1[B + i] = g1[i];
				for (j = 0 ; j < 2 ; j++)
					g2[B + i][j] = g2[i][j];
				for (j = 0 ; j < 3 ; j++)
					g3[B + i][j] = g3[i][j];
			}
		}




		static unsigned char perm[] = {151,160,137,91,90,15,
			131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
			190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
			88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
			77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
			102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
			135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
			5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
			223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
			129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
			251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
			49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
			138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
			151,160,137,91,90,15,
			131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
			190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
			88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
			77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
			102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
			135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
			5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
			223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
			129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
			251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
			49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
			138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 };



#define FADE(t) ( t * t * t * ( t * ( t * 6 - 15 ) + 10 ) )

#define FASTFLOOR(x) ( ((x)>0) ? ((int)x) : ((int)x-1 ) )
#define LERP(t, a, b) ((a) + (t)*((b)-(a)))


		const float PerlinNoiseGustavson::eval( const float x ) const
		{
			int ix0, ix1;
			float fx0, fx1;
			float s, n0, n1;

			ix0 = FASTFLOOR( x ); // Integer part of x
			fx0 = x - ix0;       // Fractional part of x
			fx1 = fx0 - 1.0f;
			ix1 = ( ix0+1 ) & 0xff;
			ix0 = ix0 & 0xff;    // Wrap to 0..255

			s = FADE( fx0 );

			n0 = grad1( perm[ ix0 ], fx0 );
			n1 = grad1( perm[ ix1 ], fx1 );
			return 0.188f * ( LERP( s, n0, n1 ) );
		}

		const float PerlinNoiseGustavson::eval( const MeshPotato::MPUtils::MPVec3& P ) const
		{
			int ix0, iy0, iz0, iw0, ix1, iy1, iz1, iw1;
			float fx0, fy0, fz0, fw0, fx1, fy1, fz1, fw1;
			float s, t, r, q;
			float nxyz0, nxyz1, nxy0, nxy1, nx0, nx1, n0, n1;

			ix0 = FASTFLOOR( P[0] ); // Integer part of x
			iy0 = FASTFLOOR( P[1] ); // Integer part of y
			iz0 = FASTFLOOR( P[2] ); // Integer part of y
			iw0 = FASTFLOOR( time ); // Integer part of w
			fx0 = P[0] - ix0;        // Fractional part of x
			fy0 = P[1] - iy0;        // Fractional part of y
			fz0 = P[2]- iz0;        // Fractional part of z
			fw0 = time - iw0;        // Fractional part of w
			fx1 = fx0 - 1.0f;
			fy1 = fy0 - 1.0f;
			fz1 = fz0 - 1.0f;
			fw1 = fw0 - 1.0f;
			ix1 = ( ix0 + 1 ) & 0xff;  // Wrap to 0..255
			iy1 = ( iy0 + 1 ) & 0xff;
			iz1 = ( iz0 + 1 ) & 0xff;
			iw1 = ( iw0 + 1 ) & 0xff;
			ix0 = ix0 & 0xff;
			iy0 = iy0 & 0xff;
			iz0 = iz0 & 0xff;
			iw0 = iw0 & 0xff;

			q = FADE( fw0 );
			r = FADE( fz0 );
			t = FADE( fy0 );
			s = FADE( fx0 );

			nxyz0 = grad4(perm[ix0 + perm[iy0 + perm[iz0 + perm[iw0]]]], fx0, fy0, fz0, fw0);
			nxyz1 = grad4(perm[ix0 + perm[iy0 + perm[iz0 + perm[iw1]]]], fx0, fy0, fz0, fw1);
			nxy0 = LERP( q, nxyz0, nxyz1 );

			nxyz0 = grad4(perm[ix0 + perm[iy0 + perm[iz1 + perm[iw0]]]], fx0, fy0, fz1, fw0);
			nxyz1 = grad4(perm[ix0 + perm[iy0 + perm[iz1 + perm[iw1]]]], fx0, fy0, fz1, fw1);
			nxy1 = LERP( q, nxyz0, nxyz1 );

			nx0 = LERP ( r, nxy0, nxy1 );

			nxyz0 = grad4(perm[ix0 + perm[iy1 + perm[iz0 + perm[iw0]]]], fx0, fy1, fz0, fw0);
			nxyz1 = grad4(perm[ix0 + perm[iy1 + perm[iz0 + perm[iw1]]]], fx0, fy1, fz0, fw1);
			nxy0 = LERP( q, nxyz0, nxyz1 );

			nxyz0 = grad4(perm[ix0 + perm[iy1 + perm[iz1 + perm[iw0]]]], fx0, fy1, fz1, fw0);
			nxyz1 = grad4(perm[ix0 + perm[iy1 + perm[iz1 + perm[iw1]]]], fx0, fy1, fz1, fw1);
			nxy1 = LERP( q, nxyz0, nxyz1 );

			nx1 = LERP ( r, nxy0, nxy1 );

			n0 = LERP( t, nx0, nx1 );

			nxyz0 = grad4(perm[ix1 + perm[iy0 + perm[iz0 + perm[iw0]]]], fx1, fy0, fz0, fw0);
			nxyz1 = grad4(perm[ix1 + perm[iy0 + perm[iz0 + perm[iw1]]]], fx1, fy0, fz0, fw1);
			nxy0 = LERP( q, nxyz0, nxyz1 );

			nxyz0 = grad4(perm[ix1 + perm[iy0 + perm[iz1 + perm[iw0]]]], fx1, fy0, fz1, fw0);
			nxyz1 = grad4(perm[ix1 + perm[iy0 + perm[iz1 + perm[iw1]]]], fx1, fy0, fz1, fw1);
			nxy1 = LERP( q, nxyz0, nxyz1 );

			nx0 = LERP ( r, nxy0, nxy1 );

			nxyz0 = grad4(perm[ix1 + perm[iy1 + perm[iz0 + perm[iw0]]]], fx1, fy1, fz0, fw0);
			nxyz1 = grad4(perm[ix1 + perm[iy1 + perm[iz0 + perm[iw1]]]], fx1, fy1, fz0, fw1);
			nxy0 = LERP( q, nxyz0, nxyz1 );

			nxyz0 = grad4(perm[ix1 + perm[iy1 + perm[iz1 + perm[iw0]]]], fx1, fy1, fz1, fw0);
			nxyz1 = grad4(perm[ix1 + perm[iy1 + perm[iz1 + perm[iw1]]]], fx1, fy1, fz1, fw1);
			nxy1 = LERP( q, nxyz0, nxyz1 );

			nx1 = LERP ( r, nxy0, nxy1 );

			n1 = LERP( t, nx0, nx1 );

			return 0.87f * ( LERP( s, n0, n1 ) );
		}
	}
}
