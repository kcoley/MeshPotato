#include "MPUtils/AttributeTable.h"
#include  <map>
namespace MeshPotato {
  namespace MPUtils {
    class AttributeTable::Impl {
    public:
    std::vector<std::string> stringAttrNames() const {
	std::vector<std::string> names;
    	    for (std::map<std::string, std::string>::const_iterator it =
			stringMap.begin(); it != stringMap.end(); ++it) {
		names.push_back(it->first);
	    }
	    return names;
    }
    const std::vector<std::string> intAttrNames() const {
	std::vector<std::string> names;
    	    for (std::map<std::string, int>::const_iterator it =
			intMap.begin(); it != intMap.end(); ++it) {
		names.push_back(it->first);
	    }
	    return names;
    }
    const std::vector<std::string> vectorAttrNames() const {
	std::vector<std::string> names;
    	    for (std::map<std::string, MPUtils::MPVec3>::const_iterator it =
			vectorMap.begin(); it != vectorMap.end(); ++it) {
		names.push_back(it->first);
	    }
	    return names;
    }
    const std::vector<std::string> doubleAttrNames() const {
	std::vector<std::string> names;
    	    for (std::map<std::string, double>::const_iterator it =
			doubleMap.begin(); it != doubleMap.end(); ++it) {
		names.push_back(it->first);
	    }
	    return names;
    }
    const double findDoubleAttr(std::string attrName) const {
	std::map<std::string, double>::const_iterator it;
	it = doubleMap.find(attrName);
	       return it->second;
    }
    const int findIntAttr(std::string attrName) const {
	std::map<std::string, int>::const_iterator it;
	it = intMap.find(attrName);
	       return it->second;
    }
    const MPUtils::MPVec3 findVectorAttr(std::string attrName) const {
	std::map<std::string, MPUtils::MPVec3>::const_iterator it;
	it = vectorMap.find(attrName);
	       return it->second;
    }
    const std::string findStringAttr(std::string attrName) const {
	std::map<std::string, std::string>::const_iterator it;
	it = stringMap.find(attrName);
	       return it->second;
    }
    void mergeTable(const MPUtils::AttributeTable &table) {
	for (std::map<std::string, std::string>::const_iterator it =
	table.mImpl->stringMap.begin(); it != table.mImpl->stringMap.end(); 
	++it) 
	{
		stringMap[it->first] = it->second;
		std::cout << stringMap[it->first] << std::endl;
	}
	for (std::map<std::string, int>::const_iterator it =
	table.mImpl->intMap.begin(); it != table.mImpl->intMap.end(); 
	++it) 
	{
		intMap[it->first] = it->second;
	}
	for (std::map<std::string, MPUtils::MPVec3>::const_iterator it =
	table.mImpl->vectorMap.begin(); it != table.mImpl->vectorMap.end(); 
	++it) 
	{
		vectorMap[it->first] = it->second;
	}
	for (std::map<std::string, double>::const_iterator it =
	table.mImpl->doubleMap.begin(); it != table.mImpl->doubleMap.end(); 
	++it) 
	{
		doubleMap[it->first] = it->second;
	}

    }
	    std::map<std::string, int> intMap;
	    std::map<std::string, double> doubleMap;
	    std::map<std::string, MPUtils::MPVec3> vectorMap;
	    std::map<std::string, std::string> stringMap;
    };
   AttributeTable::AttributeTable()
	   : mImpl(new AttributeTable::Impl()) {
   }
   AttributeTable::AttributeTable(const AttributeTable &other) 
	   :mImpl(new AttributeTable::Impl())
   {
	mImpl->intMap    = other.mImpl->intMap;		
	mImpl->doubleMap = other.mImpl->doubleMap;		
	mImpl->vectorMap = other.mImpl->vectorMap;		
	mImpl->stringMap = other.mImpl->stringMap;		
   }
   void AttributeTable::mergeTable(const MPUtils::AttributeTable & table) {
	mImpl->mergeTable(table);
   }
   void AttributeTable::addIntAttr(
      const std::string &attrName,
      const int &defaults) {
        mImpl->intMap[attrName] = defaults;
      }
    void AttributeTable::addDoubleAttr(
      const std::string &attrName,
      const double &defaults) {
        mImpl->doubleMap[attrName] = defaults;
      }
    void AttributeTable::addStringAttr(
      const std::string &attrName,
      const std::string &defaults) {
        mImpl->stringMap[attrName] = defaults;
      }
    void AttributeTable::addVectorAttr(
      const std::string &attrName,
      const MPUtils::MPVec3 &defaults) {
        mImpl->vectorMap[attrName] = defaults;
    }

    std::vector<std::string> AttributeTable::stringAttrNames() const {
	return mImpl->stringAttrNames();
    }
    std::vector<std::string> AttributeTable::intAttrNames() const {
	return mImpl->intAttrNames();
    }
    std::vector<std::string> AttributeTable::vectorAttrNames() const {
	return mImpl->vectorAttrNames();
    }
    std::vector<std::string> AttributeTable::doubleAttrNames() const {
	return mImpl->doubleAttrNames();
    }
    double AttributeTable::findDoubleAttr(std::string attrname) const {
	return mImpl->findDoubleAttr(attrname);
    }
    int AttributeTable::findIntAttr(std::string attrname) const {
	return mImpl->findIntAttr(attrname);
    }
    MPUtils::MPVec3 AttributeTable::findVectorAttr(std::string attrname) const {
	return mImpl->findVectorAttr(attrname);
    }
    std::string AttributeTable::findStringAttr(std::string attrname) const {
	return mImpl->findStringAttr(attrname);
    }
  }
}
