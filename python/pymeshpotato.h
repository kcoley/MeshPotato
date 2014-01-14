#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <iostream>
#include <list>

std::list<std::vector<std::string> > getPythonList(boost::python::list& ls) {
        std::list<std::vector<std::string> > items;
        for (unsigned int i = 0; i < len(ls); ++i) {
                boost::python::list listing = (boost::python::extract<boost::python::list>(ls[i]));
                std::vector<std::string> vec;
                for (unsigned int j = 0; j < len(listing); ++j) {
                        std::string result = boost::python::extract<std::string>(listing[j]);
                        vec.push_back(result);
                }
                items.push_back(vec);
        }
        return items;
}
template <typename T>
std::list<T> getListFromPython(boost::python::list& ls) {
        std::list<T> items;
        for (unsigned int i = 0; i < len(ls); ++i) {
                T listing = (boost::python::extract<T>(ls[i]));
		items.push_back(listing);
        }
        return items;
}
