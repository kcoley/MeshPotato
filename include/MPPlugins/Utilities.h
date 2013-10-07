#ifndef __UTILITIES_H__
#define __UTILITIES_H__
struct vertex {
	vertex() {}
	vertex(float _x, float _y, float _z) : x(_x), y(_y), z(_z), w(0) {}
	vertex(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
        float x, y, z, w;
};

#endif // __UTILITIES_H__
