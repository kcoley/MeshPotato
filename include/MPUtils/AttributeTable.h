#ifndef __ATTRIBUTETABLE_H__
#define __ATTRIBUTETABLE_H__
#include <boost/shared_ptr.hpp>
#include "MPUtils/Vector.h"
namespace MeshPotato {
	namespace MPUtils {
		class AttributeTable {
		public:
		AttributeTable();
		AttributeTable(const AttributeTable&);
		void mergeTable(const AttributeTable&);
		std::vector<std::string> stringAttrNames() const;
		std::vector<std::string> intAttrNames() const;
		std::vector<std::string> doubleAttrNames() const;
		std::vector<std::string> vectorAttrNames() const;

		void addIntAttr(const std::string &attrName,
		    const int &defaults);
		void addDoubleAttr(const std::string &attrName,
		    const double &defaults);
		void addVectorAttr(
		    const std::string &attrName,
		    const MeshPotato::MPUtils::MPVec3 &defaults);
		void addStringAttr(const std::string &attrName,
		    const std::string &defaults);

		double findDoubleAttr(std::string attrName) const;
		int findIntAttr(std::string attrName) const;
		MPUtils::MPVec3 findVectorAttr(std::string attrName) const;
		std::string findStringAttr(std::string attrName) const;
		private:
			class Impl;
			boost::shared_ptr<Impl> mImpl;
		};
	}
}
#endif // __ATTRIBUTE_TABLE_H__
