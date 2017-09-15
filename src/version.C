#include "MeshPotato/MPPlugins/version.h"

int Version::GetMajor() {
	return MESHPOTATO_API_MAJOR;
}
int Version::GetMinor() {
	return MESHPOTATO_API_MINOR;
}
int Version::GetPatch() {
	return MESHPOTATO_API_PATCH;
}

std::string Version::GetVersion() {
	std::ostringstream ss;
	ss << MESHPOTATO_API_MAJOR << "." 
		<< MESHPOTATO_API_MINOR << "."
		<< MESHPOTATO_API_PATCH;
	return ss.str();
}

bool Version::IsAtLeast(int major , int minor , int patch) {
	if (MESHPOTATO_API_MAJOR < major) return false;
	if (MESHPOTATO_API_MAJOR > major) return true;
	if (MESHPOTATO_API_MINOR < minor) return false;
	if (MESHPOTATO_API_MAJOR > minor) return true;
	if (MESHPOTATO_API_PATCH < patch) return false;
	if (MESHPOTATO_API_PATCH > patch) return true;
	return true;
}

bool Version::HasFeature(const std::string &name) {
	static std::set<std::string> features;
	if (features.empty()) {
		features.insert("DEEPIMAGE");
		features.insert("OPENVDB");
	}

	return features.find(name) != features.end();

}

