#include <boost/python.hpp>
#include <MeshPotato/MPNoise/Noise.h>
#include <MeshPotato/MPNoise/PerlinNoise.h>
//#include <MPNoise/FFTNoise.h>
using namespace boost::python;

using namespace MeshPotato::MPNoise;
BOOST_PYTHON_MODULE(mpnoise) {
	class_<MeshPotato::MPNoise::Noise_t>("Noise_t")
		.def_readwrite("frequency", &Noise_t::frequency)
		.def_readwrite("translate", &Noise_t::translate)
		.def_readwrite("octaves", &Noise_t::octaves)
		.def_readwrite("amplitude", &Noise_t::amplitude)
		.def_readwrite("offset", &Noise_t::offset)
		.def_readwrite("fjump", &Noise_t::fjump)
		.def_readwrite("roughness", &Noise_t::roughness)
		.def_readwrite("radius", &Noise_t::radius)
		.def_readwrite("pscale", &Noise_t::pscale)
		.def_readwrite("gamma", &Noise_t::gamma)
		.def_readwrite("time", &Noise_t::time)
		.def_readwrite("fftLowCutofff", &Noise_t::fftLowCutoff)
		.def_readwrite("fftHighCutofff", &Noise_t::fftHighCutoff)
	;
	class_<MeshPotato::MPNoise::Wisp_t>("Wisp_t")
	;
	class_<MeshPotato::MPNoise::Noise>("Noise")
//		.def("eval",&MeshPotato::MPNoise::Noise::eval)
	;
}
