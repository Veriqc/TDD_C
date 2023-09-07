#pragma once


#include "Definitions.hpp"
#include "Edge.hpp"
#include "Node.hpp"

#include <map>

namespace dd {

    struct Index {
        std::string key; //
        short idx;
    };


    struct TDD {

        Edge<mNode> e;
        std::vector<Index> index_set;
        std::vector<std::string> key_2_index;

    };


    struct key_2_new_key_node {
        short level;
        float new_key;
        std::map<float, key_2_new_key_node*> next;
        key_2_new_key_node* father;
    };

}