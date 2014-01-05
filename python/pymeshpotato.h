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
                std::cout << "###################################################" << std::endl;
        }
        return items;
}

