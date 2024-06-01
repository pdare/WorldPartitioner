#ifndef WORKLDPARTITION_H
#define WORKLDPARTITION_H

#include <..\godot-cpp-4.2\gen\include\godot_cpp\classes\node.hpp>

namespace godot {
    class WorldPartition : public Node {
        GDCLASS(WorldPartition, Node)

        private:
            Array children_nodes;

        protected:
            static void _bind_methods();

        public:

            WorldPartition();
            ~WorldPartition();
            void _process(double delta) override;
            void _ready() override;
    };
}

#endif