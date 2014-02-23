#include "MPUtils/Color.h"
namespace MeshPotato {
namespace MPUtils {
 std::ostream& operator<<(std::ostream& out, const Color& col) {
        out << "[" << col[0] << ", " << col[1] << ", " << col[2] << ", " << col[3] << "]";
        return out;
  }
}}
