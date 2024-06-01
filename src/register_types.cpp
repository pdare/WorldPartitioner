#include "register_types.h"
#include "worldpartition.h"

#include <gdextension_interface.h>
#include <..\godot-cpp-4.2\include\godot_cpp\core\defs.hpp>
#include <..\godot-cpp-4.2\include\godot_cpp\godot.hpp>

using namespace godot;

void initialize_world_partition_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    ClassDB::register_class<WorldPartition>();
}

void uninitialize_world_partition_module(ModuleInitializationLevel p_level){
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE){
        return;
    }
}

extern "C" {
    GDExtensionBool GDE_EXPORT world_partition_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

        init_obj.register_initializer(initialize_world_partition_module);
        init_obj.register_terminator(uninitialize_world_partition_module);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }
}