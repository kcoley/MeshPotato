/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
///  Table for holding various attributes for data types
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
