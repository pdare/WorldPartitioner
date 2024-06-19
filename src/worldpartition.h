#ifndef WORKLDPARTITION_H
#define WORKLDPARTITION_H

#include <..\godot-cpp-4.2\gen\include\godot_cpp\classes\node.hpp>
#include <..\godot-cpp-4.2\gen\include\godot_cpp\classes\packed_scene.hpp>
#include <..\godot-cpp-4.2\gen\include\godot_cpp\classes\mesh_instance3d.hpp>
#include <..\godot-cpp-4.2\gen\include\godot_cpp\classes\static_body3d.hpp>
#include <..\godot-cpp-4.2\gen\include\godot_cpp\classes\surface_tool.hpp>

namespace godot {
    class WorldPartition : public Node {
        GDCLASS(WorldPartition, Node)

        private:
            Array children_nodes;
            Array nodes_to_partition;
            Array edge_chunks;
            PackedVector3Array chunk_points;
            Node3D* player_node;
            Vector3 chunk_size;
            Vector3 map_size;
            bool show_chunk_markers;
            bool use_auto_mapsize;
            float marker_thickness;
            int current_chunk;
            int number_of_columns;
            int number_of_rows;

            MeshInstance3D *mesh_instance = memnew(MeshInstance3D);
            PackedVector3Array mesh_verts = PackedVector3Array();

            MeshInstance3D *debug_mesh_instance = memnew(MeshInstance3D);
            PackedVector3Array debug_mesh_verts = PackedVector3Array();

        protected:
            static void _bind_methods();

        public:
            void set_chunk_size(const Vector3 p_chunk_size);
            Vector3 get_chunk_size() const;

            void set_map_size(const Vector3 p_map_size);
            Vector3 get_map_size() const;

            void set_show_chunk_markers(bool p_show_chunk_markers);
            bool get_show_chunk_markers() const;

            void set_auto_mapsize(const bool p_use_auto_mapsize);
            bool get_auto_mapsize() const;

            void generate_chunks();
            void generate_markers();
            void generate_debug_markers();
            bool check_in_chunk(Vector3 chunk_point, Vector3 test_point);
            void check_chunks();
            void check_if_chunk_changed();
            void generate_point_markers();


            WorldPartition();
            ~WorldPartition();
            void _process(double delta) override;
            void _ready() override;
    };
}

#endif