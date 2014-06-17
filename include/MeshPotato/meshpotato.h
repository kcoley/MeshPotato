/*! \mainpage %MeshPotato
 *
 * \section intro_sec Introduction
 * %MeshPotato is a C++/Python volume modeling and rendering API
 * designed to simplify the creation of volumetric effects.
 * \page page1 A documentation page
 * \tableofcontents
 * Leading text.
 * <A HREF=test.txt>test</A>
 * \section install_sec Installation
 * \subsection step1 Step1: Ask Kacey...
 *
 */

#ifndef __MESHPOTATO_H__
#define __MESHPOTATO_H__
#include <iostream>
#include <string.h>
namespace MeshPotato {
	class MeshSpec {
	public:
		MeshSpec(): voxelSize(0.05), exBandWidth(3), inBandWidth(3) {}
		MeshSpec(const float &vs, const float &eb, const float &ib) : voxelSize(vs), exBandWidth(eb), inBandWidth(ib) {}
		float voxelSize;
		float exBandWidth;
		float inBandWidth;
	}; //!< Define the specification for mesh data

	struct DeepData {}; //!< Strut for deep data

	class MeshInput {
		public:
			MeshInput();
			MeshInput(const MeshInput &mesh);
			~MeshInput();
			//! Attempt to open the mesh file
			/*!
			 \param const std::string & - Mesh Name
			 \return an input Mesh handle
			*/
			MeshInput *create(const std::string &);
			virtual bool create(const std::string &, const MeshSpec &newspec) = 0;

	}; //! Input Mesh

	class MeshOutput {}; //! Output Mesh




}
#endif // __MESHPOTATO_H__
