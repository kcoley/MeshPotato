/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
///  Class for holding Mesh Face data
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

#include <list>
namespace MeshPotato {
namespace MPUtils {
class Face {
public:
  Face() {}
  Face(const std::list<int> &_vIndexes, const std::list<int> &_nIndexes, const std::list<int> &_tIndexes) : vIndexes(_vIndexes), nIndexes(_nIndexes), tIndexes(_tIndexes) {}
  ~Face() {}

  std::list<int> vIndexes;     // These two vectors will hold vertex and normal
  std::list<int> nIndexes;     // information for the face
  std::list<int> tIndexes;
  int mat;                // Represents index of material to be used
};
}
}
