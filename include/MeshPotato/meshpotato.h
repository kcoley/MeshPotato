/*! \mainpage MeshPotato
 *
 * \section intro_sec Introduction
 * This is MeshPotato.
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
