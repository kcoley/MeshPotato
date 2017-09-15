/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
/// Routines for writing and reading images using OpenImageIO
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

#ifndef __OIIOFILES_H__
#define __OIIOFILES_H__

#include "Image.h"
#include "DeepImage.h"
#include <map>
#include <string>
#include <vector>
using namespace std;
using namespace MeshPotato::MPUtils;
namespace MeshPotato{
namespace MPUtils{


void writeOIIOImage( const char* fname, Image& img, float brightness = 1.0, float gamma = 1.0 );
void writeOIIOImage( const char* fname, DeepImage& img, float brightness = 1.0, float gamma = 1.0 );
void writeOIIOImage( const char* fname, DeepImage& img, const map<string, string>& labels, float brightness = 1.0, float gamma = 1.0 );
void writeOIIOImage( const char* fname, DeepImage& img, const vector<string>& keys, const vector<string>& tags, float brightness = 1.0, float gamma = 1.0 );
void writeOIIOImage( const char* fname, Image& img, const map<string,string>& labels, float brightness = 1.0, float gamma = 1.0 );
void writeOIIOImage( const char* fname, Image& img, const vector<string>& keys, const vector<string>& tags,  float brightness = 1.0, float gamma = 1.0 );
void readOIIOImage( const char* fname, Image& img );
void readOIIOImage( const char* fname, DeepImage& img );
void readOIIOImage( const char* fname, Image& img, map<string,string>& labels );
void readOIIOImage( const char* fname, Image& img, vector<string>& keys, vector<string>& tags );
void printMetadata( const map<string,string>& meta );

float* convert( const Image& img, int channel );

}
}
#endif


