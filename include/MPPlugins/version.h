#include <string>
#include <sstream>
#include <boost/scoped_ptr.hpp>
#include <set>

#define MESHPOTATO_API_MAJOR 0
#define MESHPOTATO_API_MINOR 2
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

