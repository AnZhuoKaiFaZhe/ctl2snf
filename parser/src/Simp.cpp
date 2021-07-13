//
// Created by mzy on 2021-06-03.
//

#include "Simp.hpp"
Tree<ASTNode>* simp(Tree<ASTNode>* n,Tree<ASTNode>* l,Tree<ASTNode>* r){
    switch (n->root.type) {
        case NodeType_t::LogicAnd:
            if(l->root.type==NodeType_t::True || r->root.type==NodeType_t::False)return r;
            if(l->root.type==NodeType_t::False || r->root.type==NodeType_t::True)return l;
            return n;
        case NodeType_t::LogicOr:
            if(l->root.type==NodeType_t::True || r->root.type==NodeType_t::False)return l;
            if(l->root.type==NodeType_t::False || r->root.type==NodeType_t::True)return r;
            return n;
    }
}


