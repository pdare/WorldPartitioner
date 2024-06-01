#include "worldpartition.h"
#include <..\godot-cpp-4.2\include\godot_cpp\core\class_db.hpp>
#include <..\godot-cpp-4.2\gen\include\godot_cpp\variant\utility_functions.hpp>
#include <..\godot-cpp-4.2\gen\include\godot_cpp\classes\node3d.hpp>
#include <..\godot-cpp-4.2\include\godot_cpp\core\object.hpp>

using namespace godot;

void WorldPartition::_bind_methods() {

}

WorldPartition::WorldPartition() {
}

WorldPartition::~WorldPartition() {}

void WorldPartition::_process(double delta) {
    
}

void WorldPartition::_ready() {
    children_nodes = get_children();
    int non_node3d_count = 0;
    for (int i = 0; i < children_nodes.size(); i++)
    {
        godot::Node3D* child = Object::cast_to<Node3D>(children_nodes[i]);
        if (child) {
            if (child->get_global_position() != Vector3(0.0, 0.0, 0.0)) {
                child->set_visible(false);
                child->set_process_mode(ProcessMode::PROCESS_MODE_DISABLED);
            }
            godot::UtilityFunctions::print(child->get_class());
            godot::UtilityFunctions::print(child->get_global_position());
        }
    }
    godot::UtilityFunctions::print(non_node3d_count);
}