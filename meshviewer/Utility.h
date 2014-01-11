//----------------------------------------------
//    Definition of general utility routines
//----------------------------------------------
//
//  Programmer: Donald House
//  Date: March 8, 1999
//
//  Copyright (C) - Donald H. House. 2005
//

#ifndef _H_UTILITY
#define _H_UTILITY

#include <cmath>
#include <cstring>
#include <iostream>	// define C++ stream I/O routines
#include <iomanip>
#include <cstdlib>
#include <string>

/* range of real numbers */
#define SMALLNUMBER	1.0e-5
#define HUGENUMBER	1.0e10
/* Miscellaneous Scalar Math */
#define AbsMV(x)		(((x) < 0) ? (-(x)) : (x))
#define SqrMV(x)		((x) * (x))
#define MinMV(x1,x2)	((x1)<(x2)?(x1):(x2))
#define MaxMV(x1,x2)	((x1)>(x2)?(x1):(x2))
#define RoundMV(x, p)	(((int)((x)*pow(10.0,p)+((x)<0?-0.5:0.5)))/pow(10.0,p))
#define SignMV(x)		((x)>=0? 1: -1)
#define Swap(x1, x2)	{int tmp=x1; x1=x2; x2=tmp}
#define ApplySign(x, y)	((y) >= 0? AbsMV(x): -AbsMV(x))
/* Angle Conversions & Constants */

#ifndef PI
#define PI 3.1415926535897
#endif

#define RAD2DEG (180/PI)
#define DEG2RAD (PI/180)

#define DegToRad(x) ((x)*DEG2RAD)
#define RadToDeg(x) ((x)*RAD2DEG)

/*
  Boolean type
*/
enum boolean{FALSE, TRUE};

/*
  computes sqrt(a^2 + b^2) without destructive underflow or overflow
*/
double pythag(double a, double b);

/*
  Utility Error message routines
*/
// print s to stdout with trailing blank and no terminating end of line
void prompt(char *s);

// print s1, s2, s3 to stdout as blank separated fields with terminating eol
void message(char *s1, char *s2 = NULL, char *s3 = NULL);

// print Status: to stdout followed by message(s1, s2, s3)
void status(char *s1, char *s2 = NULL, char *s3 = NULL);

// print Error: followed by s1, s2 and s3 to stderr as blank separated fields 
// with terminating eol
void error(char *s1, char *s2 = NULL, char *s3 = NULL);

// print error(s1, s2, s3) and then exit program with code 1 
void abort(char *s1, char *s2 = NULL, char *s3 = NULL);

template <typename T>
T stringToType(const std::string &string);
template <typename T>
std::string typeToString(const T &type);


#include "Utility.tpp"
#endif
