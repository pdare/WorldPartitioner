#include "worldpartition.h"
#include <..\godot-cpp-4.2\include\godot_cpp\core\class_db.hpp>
#include <..\godot-cpp-4.2\gen\include\godot_cpp\variant\utility_functions.hpp>
#include <..\godot-cpp-4.2\gen\include\godot_cpp\classes\node3d.hpp>
#include <..\godot-cpp-4.2\include\godot_cpp\core\object.hpp>

using namespace godot;

void WorldPartition::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_chunk_size"), &WorldPartition::get_chunk_size);
    ClassDB::bind_method(D_METHOD("set_chunk_size", "p_chunk_size"), &WorldPartition::set_chunk_size);
    ClassDB::add_property("WorldPartition", PropertyInfo(Variant::VECTOR3, "chunk_size"), "set_chunk_size", "get_chunk_size");

    ClassDB::bind_method(D_METHOD("get_map_size"), &WorldPartition::get_map_size);
    ClassDB::bind_method(D_METHOD("set_map_size", "p_map_size"), &WorldPartition::set_map_size);
    ClassDB::add_property("WorldPartition", PropertyInfo(Variant::VECTOR3, "map_size"), "set_map_size", "get_map_size");

    ClassDB::bind_method(D_METHOD("get_show_chunk_markers"), &WorldPartition::get_show_chunk_markers);
    ClassDB::bind_method(D_METHOD("set_show_chunk_markers", "p_show_chunk_markers"), &WorldPartition::set_show_chunk_markers);
    ClassDB::add_property("WorldPartition", PropertyInfo(Variant::BOOL, "show_chunk_markers"), "set_show_chunk_markers", "get_show_chunk_markers");

    ClassDB::bind_method(D_METHOD("get_auto_mapsize"), &WorldPartition::get_auto_mapsize);
    ClassDB::bind_method(D_METHOD("set_auto_mapsize", "p_use_auto_mapsize"), &WorldPartition::set_auto_mapsize);
    ClassDB::add_property("WorldPartition", PropertyInfo(Variant::BOOL, "use_auto_mapsize"), "set_auto_mapsize", "get_auto_mapsize");
}

WorldPartition::WorldPartition() {
    //chunk_size = Vector3(0.0, 0.0, 0.0);
    //map_size = Vector3(0.0, 0.0, 0.0);
    //show_chunk_markers = false;
}

WorldPartition::~WorldPartition() {}

void WorldPartition::_ready() {
    marker_thickness = 0.2;
    children_nodes = get_children();
    int non_node3d_count = 0;
    number_of_columns = map_size.x / chunk_size.x;
    number_of_rows = map_size.z / chunk_size.z;
    godot::StringName test_name = "Player";
    for (int i = 0; i < children_nodes.size(); i++)
    {
        godot::Node3D* child = Object::cast_to<Node3D>(children_nodes[i]);
        if (child && child->get_name() != test_name) {
            nodes_to_partition.append(child);
        }
        else if (child->get_name() == test_name) { player_node = child; }
    }
    godot::UtilityFunctions::print(non_node3d_count);
    use_auto_mapsize = false;
    show_chunk_markers = true;
    generate_chunks();
    if (show_chunk_markers){generate_markers();}

    for (int i = 0; i < chunk_points.size(); i++) {
        if (check_in_chunk(chunk_points[i], player_node->get_global_position()))
        {current_chunk = i; break;}
    }

    generate_debug_markers();
    generate_point_markers();
    godot::UtilityFunctions::print("columns and rows");
    godot::UtilityFunctions::print(number_of_columns);
    godot::UtilityFunctions::print(number_of_rows);
    godot::UtilityFunctions::print(chunk_points.size());
}

void WorldPartition::_process(double delta) {

   check_if_chunk_changed();
}

void WorldPartition::set_chunk_size(const Vector3 p_chunk_size) {chunk_size = p_chunk_size;}
Vector3 WorldPartition::get_chunk_size() const {return chunk_size;}

void WorldPartition::set_map_size(const Vector3 p_map_size) {map_size = p_map_size;}
Vector3 WorldPartition::get_map_size() const {return  map_size;}

void WorldPartition::set_show_chunk_markers(const bool p_show_chunk_markers) {show_chunk_markers = p_show_chunk_markers;}
bool WorldPartition::get_show_chunk_markers() const {return show_chunk_markers;}

void WorldPartition::set_auto_mapsize(const bool p_use_auto_mapsize) {use_auto_mapsize = p_use_auto_mapsize;}
bool WorldPartition::get_auto_mapsize() const {return use_auto_mapsize;}

void WorldPartition::generate_chunks() {
    godot::UtilityFunctions::print("generating chunks");
    if (use_auto_mapsize){
        godot::UtilityFunctions::print("using auto mapsize");
    } else {
        int x_points = ceill(map_size.x / chunk_size.x);
        int z_points = ceill(map_size.z / chunk_size.z);
        godot::UtilityFunctions::print("Number of points x and z");
        godot::UtilityFunctions::print(x_points);
        godot::UtilityFunctions::print(z_points);

        Vector3 starting_point = Vector3(map_size.x / 2, map_size.y / 2, map_size.z / 2);
        Vector3 current_point = Vector3(starting_point.x + (chunk_size.x / 2), starting_point.y, starting_point.z + (chunk_size.z / 2));

        Vector3 lowest_point = Vector3((-map_size.x / 2) + (chunk_size.x / 2), (-map_size.y / 2)  + (chunk_size.y / 2), (-map_size.z / 2) + (chunk_size.z / 2));

        godot::UtilityFunctions::print(current_point);
        godot::UtilityFunctions::print(lowest_point);
        godot::UtilityFunctions::print("\n");

        Vector3 last_point = starting_point;
        for (int i = 0; i < x_points; i++) {
            current_point.x = current_point.x - (chunk_size.x);
            last_point.z = starting_point.z + (chunk_size.z / 2);
            for (int j = 0; j < z_points; j++) {
                current_point = Vector3(current_point.x, 0.0, last_point.z - (chunk_size.z));
                chunk_points.append(current_point);
                godot::UtilityFunctions::print(current_point);
                last_point = current_point;
            }
        }

        /*
        while (current_point.z >= lowest_point.z)
        {
            while (current_point.x >= lowest_point.x)
            {
                chunk_points.append(current_point);
                godot::UtilityFunctions::print(current_point);
                current_point = Vector3(current_point.x - (chunk_size.x), current_point.y, current_point.z);
            }

            current_point = Vector3(starting_point.x, current_point.y, current_point.z  - (chunk_size.z));
        }
        */
    }
    

    for (int i = 0; i < number_of_columns; i++) {
        edge_chunks.append(i);
        if (i == 0 || i == number_of_columns - 1 || i / number_of_columns) {
            edge_chunks.append(i);
        }
    }
}

void WorldPartition::generate_markers() {
    godot::UtilityFunctions::print("generating chunk markers");
    float x_point = chunk_size.x / 2;
    float z_point = chunk_size.z / 2;
    float y_point = chunk_size.y / 2;
    // front marker
    // top of mesh
    mesh_verts.push_back(Vector3(x_point, 0.0, -z_point));
    mesh_verts.push_back(Vector3(-x_point, 0.0, -(z_point) + marker_thickness));
    mesh_verts.push_back(Vector3(-x_point, 0.0, -z_point));
    
    mesh_verts.push_back(Vector3(x_point, 0.0, -z_point));
    mesh_verts.push_back(Vector3(x_point, 0.0, -(z_point) + marker_thickness));
    mesh_verts.push_back(Vector3(-x_point, 0.0, -(z_point) + marker_thickness));
    
    // front of mesh
    mesh_verts.push_back(Vector3(x_point, 0.0, -(z_point) + marker_thickness));
    mesh_verts.push_back(Vector3(-x_point, -marker_thickness, -(z_point) + marker_thickness));
    mesh_verts.push_back(Vector3(-x_point, 0.0, -(z_point) + marker_thickness));
    
    mesh_verts.push_back(Vector3(x_point, 0.0, -(z_point) + marker_thickness));
    mesh_verts.push_back(Vector3(x_point, -marker_thickness, -(z_point) + marker_thickness));
    mesh_verts.push_back(Vector3(-x_point, -marker_thickness, -(z_point) + marker_thickness));
    
    
    // back marker
    // top of mesh
    mesh_verts.push_back(Vector3(x_point, 0.0, z_point));
    mesh_verts.push_back(Vector3(-x_point, 0.0, z_point));
    mesh_verts.push_back(Vector3(-x_point, 0.0, (z_point) - marker_thickness));
    
    mesh_verts.push_back(Vector3(x_point, 0.0, z_point));
    mesh_verts.push_back(Vector3(-x_point, 0.0, (z_point) - marker_thickness));
    mesh_verts.push_back(Vector3(x_point, 0.0, (z_point) - marker_thickness));
    
    // front of mesh
    mesh_verts.push_back(Vector3(x_point, 0.0, (z_point) - marker_thickness));
    mesh_verts.push_back(Vector3(-x_point, 0.0, (z_point) - marker_thickness));
    mesh_verts.push_back(Vector3(-x_point, -marker_thickness, (z_point) - marker_thickness));
    
    mesh_verts.push_back(Vector3(x_point, 0.0, (z_point) - marker_thickness));
    mesh_verts.push_back(Vector3(-x_point, -marker_thickness, (z_point) - marker_thickness));
    mesh_verts.push_back(Vector3(x_point, -marker_thickness, (z_point) - marker_thickness));
    
    
    // right marker
    // top of mesh
    mesh_verts.push_back(Vector3((x_point) - marker_thickness, 0.0, z_point));
    mesh_verts.push_back(Vector3((x_point), 0.0, -z_point));
    mesh_verts.push_back(Vector3((x_point), 0.0, z_point));
    
    mesh_verts.push_back(Vector3((x_point) - marker_thickness, 0.0, -z_point));
    mesh_verts.push_back(Vector3(x_point, 0.0, -z_point));
    mesh_verts.push_back(Vector3((x_point) - marker_thickness, 0.0, z_point));
    // front of mesh
    mesh_verts.push_back(Vector3((x_point) - marker_thickness, 0.0, z_point));
    mesh_verts.push_back(Vector3((x_point) - marker_thickness, -marker_thickness, z_point));
    mesh_verts.push_back(Vector3((x_point) - marker_thickness, -marker_thickness, -z_point));
    
    mesh_verts.push_back(Vector3((x_point) - marker_thickness, 0.0, -z_point));
    mesh_verts.push_back(Vector3((x_point) - marker_thickness, 0.0, z_point));
    mesh_verts.push_back(Vector3((x_point) - marker_thickness, -marker_thickness, -z_point));
    
    
    // left marker
    // top of mesh
    mesh_verts.push_back(Vector3(-(x_point) - marker_thickness, 0.0, z_point));
    mesh_verts.push_back(Vector3(-(x_point), 0.0, -z_point));
    mesh_verts.push_back(Vector3(-(x_point), 0.0, z_point));
    
    mesh_verts.push_back(Vector3(-(x_point) - marker_thickness, 0.0, -z_point));
    mesh_verts.push_back(Vector3(-x_point, 0.0, -z_point));
    mesh_verts.push_back(Vector3(-(x_point) - marker_thickness, 0.0, z_point));
    // front of mesh
    mesh_verts.push_back(Vector3(-(x_point), 0.0, z_point));
    mesh_verts.push_back(Vector3(-(x_point), -marker_thickness, -z_point));
    mesh_verts.push_back(Vector3(-(x_point), -marker_thickness, z_point));
    
    mesh_verts.push_back(Vector3(-(x_point), 0.0, -z_point));
    mesh_verts.push_back(Vector3(-(x_point), -marker_thickness, -z_point));
    mesh_verts.push_back(Vector3(-(x_point), 0.0, z_point));
    
    // test single marker is generated correctly 
    /*
    Array surface_array = Array();
    surface_array.resize(Mesh::ARRAY_MAX);

    SurfaceTool *surface_tool = memnew(SurfaceTool);
    surface_tool->begin(Mesh::PRIMITIVE_TRIANGLES);
    surface_tool->generate_normals();
    mesh_instance->set_mesh(memnew(ArrayMesh)); 

    for (int i = 0; i < mesh_verts.size(); i++) {
        surface_tool->add_vertex(mesh_verts[i]);
    }

    mesh_instance->set_mesh(surface_tool->commit());
    mesh_instance->initialize_class();
    add_child(mesh_instance);
    mesh_instance->set_position(chunk_points[0]);
    */

    
    for (int i = 0; i < chunk_points.size(); i++) {
        MeshInstance3D *another_mesh = memnew(MeshInstance3D);
        //godot::UtilityFunctions::print(chunk_points[i]);
        Array surface_array = Array();
        surface_array.resize(Mesh::ARRAY_MAX);

        SurfaceTool *surface_tool = memnew(SurfaceTool);
        surface_tool->begin(Mesh::PRIMITIVE_TRIANGLES);
        surface_tool->generate_normals();
        another_mesh->set_mesh(memnew(ArrayMesh));

        for (int i = 0; i < mesh_verts.size(); i++) {
            surface_tool->add_vertex(mesh_verts[i]);
        }

        another_mesh->set_mesh(surface_tool->commit());
        another_mesh->initialize_class();
        add_child(another_mesh);
        another_mesh->set_position(chunk_points[i]);
    }
    
}

void WorldPartition::generate_debug_markers() {
    godot::UtilityFunctions::print("Generating debug markers");
    debug_mesh_verts.push_back(Vector3(chunk_size.x / 2, 0.0, chunk_size.z / 2));
    debug_mesh_verts.push_back(Vector3(-chunk_size.x / 2, 0.0, -chunk_size.z / 2));
    debug_mesh_verts.push_back(Vector3(chunk_size.x / 2, 0.0, -chunk_size.z / 2));
    
    debug_mesh_verts.push_back(Vector3(chunk_size.x / 2, 0.0, chunk_size.z / 2));
    debug_mesh_verts.push_back(Vector3(-chunk_size.x / 2, 0.0, chunk_size.z / 2));
    debug_mesh_verts.push_back(Vector3(-chunk_size.x / 2, 0.0, -chunk_size.z / 2));
    
    Array debug_surface_array = Array();
    debug_surface_array.resize(Mesh::ARRAY_MAX);

    SurfaceTool *debug_surface_tool = memnew(SurfaceTool);
    debug_surface_tool->begin(Mesh::PRIMITIVE_TRIANGLES);
    debug_surface_tool->generate_normals();
    debug_mesh_instance->set_mesh(memnew(ArrayMesh));

    for (int i = 0; i < debug_mesh_verts.size(); i++) {
        debug_surface_tool->add_vertex(debug_mesh_verts[i]);
    }

    debug_mesh_instance->set_mesh(debug_surface_tool->commit());
    debug_mesh_instance->initialize_class();
    add_child(debug_mesh_instance);
    debug_mesh_instance->set_position(chunk_points[current_chunk]);
    //debug_mesh_instance->set_position(player_node->get_global_position());
    godot::UtilityFunctions::print("current chunk");
    godot::UtilityFunctions::print(current_chunk);
    godot::UtilityFunctions::print(chunk_points[current_chunk]);
    godot:UtilityFunctions::print(debug_mesh_instance->get_global_position());
}

void WorldPartition::generate_point_markers() {
    PackedVector3Array point_marker_verts = PackedVector3Array();

    point_marker_verts.push_back(Vector3(5.0, 0.0, 5.0));
    point_marker_verts.push_back(Vector3(-5.0, 0.0, -5.0));
    point_marker_verts.push_back(Vector3(5.0, 0.0, -5.0));
    
    point_marker_verts.push_back(Vector3(5.0, 0.0, 5.0));
    point_marker_verts.push_back(Vector3(-5.0, 0.0, 5.0));
    point_marker_verts.push_back(Vector3(-5.0, 0.0, -5.0));

    /*
    PackedVector3Array point_marker_normals = PackedVector3Array();
    PackedVector2Array point_marker_uvs = PackedVector2Array();
    PackedInt32Array point_marker_indices = PackedInt32Array();

    int rings = 50;
    int radial_segments = 50;
    int radius = 1;

    float this_row = 0;
    float prev_row = 0;
    float point = 0;

    for (int i = 0; i < rings; i++) {
        float v = i / rings;
        float w = sin(Math_PI * v);
        float y = cos(Math_PI * v);

        for (int j = 0; j < radial_segments; j++) {
            float u = j / radial_segments;
            float x = sin(u * Math_PI * 2.0);
            float z = cos(u * Math_PI * 2.0);

            Vector3 vert = Vector3(x * radius * w, y * radius, z * radius * w);
            point_marker_verts.append(vert);
            point_marker_normals.append(vert.normalized());
            point_marker_uvs.append(Vector2(u, v));
            point++;

            if (i > 0 && j > 0) {
                point_marker_indices.append(prev_row + j - 1);
                point_marker_indices.append(prev_row + j);
                point_marker_indices.append(this_row + j - 1);

                point_marker_indices.append(prev_row + j);
                point_marker_indices.append(this_row + j);
                point_marker_indices.append(this_row + j - 1);
            }

            prev_row = this_row;
            this_row = point;
        }
    }
    */

    for (int i = 0; i < chunk_points.size(); i++) {
        Array point_marker_surface_array = Array();
        point_marker_surface_array.resize(Mesh::ARRAY_MAX);

        SurfaceTool *point_marker_surface_tool = memnew(SurfaceTool);
        MeshInstance3D *point_marker_mesh = memnew(MeshInstance3D);
        point_marker_surface_tool->begin(Mesh::PRIMITIVE_TRIANGLES);
        point_marker_surface_tool->generate_normals();
        point_marker_mesh->set_mesh(memnew(ArrayMesh));

        for (int i = 0; i < point_marker_verts.size(); i++) {
            point_marker_surface_tool->add_vertex(point_marker_verts[i]);
        }

        point_marker_mesh->set_mesh(point_marker_surface_tool->commit());
        point_marker_mesh->initialize_class();
        add_child(point_marker_mesh);
        point_marker_mesh->set_position(chunk_points[i]);
    }
}

bool WorldPartition::check_in_chunk(Vector3 chunk_point, Vector3 test_point) {
    float positive_x = chunk_point.x + chunk_size.x / 2;
    float negative_x = chunk_point.x - chunk_size.x / 2;
    float positive_z = chunk_point.z + chunk_size.z / 2;
    float negative_z = chunk_point.z - chunk_size.z / 2;

    return (test_point.x <= positive_x && test_point.x >= negative_x) && (test_point.z <= positive_z && test_point.z >= negative_z);
}

void WorldPartition::check_chunks() {

}

void WorldPartition::check_if_chunk_changed() {
   int chunk_ahead = current_chunk + 1;
   int chunk_behind = current_chunk - 1;
   int chunk_left = current_chunk + 10;
   int chunk_right = current_chunk - 10;

   if (check_in_chunk(chunk_points[chunk_ahead], player_node->get_global_position())) {
        current_chunk = chunk_ahead;
        debug_mesh_instance->set_global_position(chunk_points[current_chunk]);
   } else if (check_in_chunk(chunk_points[chunk_behind], player_node->get_global_position())) {
        current_chunk = chunk_behind;
        debug_mesh_instance->set_global_position(chunk_points[current_chunk]);
   } else if (check_in_chunk(chunk_points[chunk_left], player_node->get_global_position())) {
        current_chunk = chunk_left;
        debug_mesh_instance->set_global_position(chunk_points[current_chunk]);
   } else if (check_in_chunk(chunk_points[chunk_right], player_node->get_global_position())) {
        current_chunk = chunk_right;
        debug_mesh_instance->set_global_position(chunk_points[current_chunk]);
   }
}

// caclculate number of columns
// check if current chunk is an edge chunk
// if it's an edge chunk we can ignore the missing sides
// check side chunks first chunk_to_check = i - 1, chunk_to_check = i + 1
// check top and bottom chunks next chunk_to_check = i - numb_of_columns, chunk_to_check = i + numb_of_columns
// check corner chunks:
//   chunk_to_check = i - numb_of_columns - 1, chunk_to_check = i - numb_of_columns + 1
//   chunk_to_check = i + numb_of_columns - 1, chunk_to_check = i + numb_of_columns + 1