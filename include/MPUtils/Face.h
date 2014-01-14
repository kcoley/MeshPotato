
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
