#include <MeshPotato/meshpotato.h>
#include <MPUtils/Vector.h>
namespace MeshPotato {
	namespace MPPlugins {
		class OBJInput : public MeshInput {
			public:
				OBJInput() {init(); }
				virtual ~OBJInput () { delete [] name; }
				virtual const char * format_name (void) const { return "obj"; }
				virtual bool load (const std::string &name, const MeshSpec &spec);
				void registerPlugin(PluginManager &PM);
			private:
				char *name;
				MeshPotato::MPUtils::Vector3d *indices;
				MeshPotato::MPUtils::Vector3d *vertices;
				MeshPotato::MPUtils::Vector3d *normals;

				void init() { /*name = new char[strlen(1)];*/ }
		};


	}
}
