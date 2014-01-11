#include <sstream>
template <typename T>
T stringToType(const std::string &string) {
        std::stringstream strm;
        T type_value;
        strm << string << std::endl;
        strm >> type_value;

        return type_value;
}
template <typename T>
std::string typeToString(const T &type) {
        std::stringstream strm;
        std::string type_value;
        strm << type << std::endl;
        strm >> type_value;

        return type_value;
}
