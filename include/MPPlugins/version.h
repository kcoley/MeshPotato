/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
///  MeshPotato API versioning class
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

#include <string>
#include <sstream>
#include <boost/scoped_ptr.hpp>
#include <set>

#define MESHPOTATO_API_MAJOR 0
#define MESHPOTATO_API_MINOR 3
#define MESHPOTATO_API_PATCH 0

class Version {
	public:
	static int GetMajor();
	static int GetMinor();
	static int GetPatch();
	static std::string GetVersion();
	static bool IsAtLeast(int major, int minor, int patch);
	static bool HasFeature(const std::string &name);
};

