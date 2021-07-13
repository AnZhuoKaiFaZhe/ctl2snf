//
// Created by mzy on 2021-05-30.
//

#include "Nnf.hpp"

void Nnf(ASTNode& n) {
    switch (n.type) {
        case NodeType_t::Not:
            n.type = NodeType_t::None;
            n.token = "";
            break;
        case NodeType_t::True:
            n.type = NodeType_t::False;
            n.token = "F";
            break;
        case NodeType_t::False:
            n.type = NodeType_t::True;
            n.token = "T";
            break;
        case NodeType_t::LogicOr:
            n.type = NodeType_t::LogicAnd;
            n.token = "&";
            break;
        case NodeType_t::LogicAnd:
            n.type = NodeType_t::LogicOr;
            n.token = "V";
            break;
        case NodeType_t::Exist_Next:
            n.type = NodeType_t::All_Next;
            n.token = "AX";
            break;
        case NodeType_t::Exist_Future:
            n.type = NodeType_t::All_Global;
            n.token = "AG";
            break;
        case NodeType_t::Exist_Global:
            n.type = NodeType_t::All_Future;
            n.token = "AF";
            break;
        case NodeType_t::Exist_Until:
            n.type = NodeType_t::All_Unless;
            n.token = "AW";
            break;
        case NodeType_t::Exist_Unless:
            n.type = NodeType_t::All_Until;
            n.token = "AU";
            break;
        case NodeType_t::All_Unless:
            n.type = NodeType_t::Exist_Until;
            n.index=++Eindx;
            n.token = "EU";
            break;
        case NodeType_t::All_Until:
            n.type = NodeType_t::Exist_Unless;
            n.index=++Eindx;
            n.token = "EW";
            break;
        case NodeType_t::All_Future:
            n.index=++Eindx;
            n.type = NodeType_t::Exist_Global;
            n.token = "EG";
            break;
        case NodeType_t::All_Global:
            n.type = NodeType_t::Exist_Future;
            n.index=++Eindx;
            n.token = "EF";
            break;
        case NodeType_t::All_Next:
            n.type = NodeType_t::Exist_Future;
            n.token = "EF";
            break;
        case NodeType_t::Atom:
            n.type = NodeType_t::Literal;
            n.token = "~" + n.token;
            break;
        case NodeType_t::Literal:
            n.type = NodeType_t::Atom;
            n.token = n.token.erase(0, 1);
            break;
    }
}