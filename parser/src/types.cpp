//
// Created by mzy on 2021-05-07.
//
#include <types.hpp>

bool isLiteral(const NodeType_t &a){if(a==NodeType_t::Atom||a==NodeType_t::Literal)return true;return false;}

NodeType_t FromString(const std::string &string) {
    // Comparator
    if (string == "<") return NodeType_t::CompLess;
    if (string == "<=") return NodeType_t::CompLessEq;
    if (string == "!=") return NodeType_t::CompNeq;
    if (string == "==") return NodeType_t::CompEq;
    if (string == ">=") return NodeType_t::CompGreaterEq;
    if (string == ">") return NodeType_t::CompGreater;
    if (string == "->") return NodeType_t::Implies;
    // Logic and / or
    if (string == "&&") return NodeType_t::LogicAnd;
    if (string == "&") return NodeType_t::LogicAnd;
    if (string == "||") return NodeType_t::LogicOr;
    if (string == "v") return NodeType_t::LogicOr;
    // temperal
    if (string == "AW") return NodeType_t::All_Unless;
    if (string == "AU") return NodeType_t::All_Until;
    if (string == "AG") return NodeType_t::All_Global;
    if (string == "AF") return NodeType_t::All_Future;
    if (string == "AX") return NodeType_t::All_Next;
    if (string == "EW") return NodeType_t::Exist_Unless;
    if (string == "EU") return NodeType_t::Exist_Until;
    if (string == "EG") return NodeType_t::Exist_Global;
    if (string == "EF") return NodeType_t::Exist_Future;
    if (string == "EX") return NodeType_t::Exist_Next;

    //True False
    if (string == "T") return NodeType_t::True;
    if (string == "~T") return NodeType_t::False;
    return NodeType_t::UNKNOWN;
}

// Should be used primarily for debugging
std::string ConvertToString(const NodeType_t &t) {
    switch (t) {
        case NodeType_t::Implies:
            return "Implies";
        case NodeType_t::Literal:
            return "Literal";
        case NodeType_t::Atom:
            return "Atom";
        case NodeType_t::Expr:
            return "Atom";
        case NodeType_t::Deadlock:
            return "Deadlock";
        case NodeType_t::LogicAnd:
            return "LogicAnd";
        case NodeType_t::LogicOr:
            return "LogicOr";
        case NodeType_t::Not:
            return "Not";
        case NodeType_t::CompLess:
            return "CompLess";
        case NodeType_t::CompLessEq:
            return "CompLessEq";
        case NodeType_t::CompNeq:
            return "CompNeq";
        case NodeType_t::CompEq:
            return "CompEq";
        case NodeType_t::CompGreater:
            return "CompGreater";
        case NodeType_t::CompGreaterEq:
            return "CompGreaterEq";
        case NodeType_t::SubExpr:
            return "SubExpr";
        case NodeType_t::All_Future:
            return "All_Future";
        case NodeType_t::All_Global:
            return "All_Global";
        case NodeType_t::All_Until:
            return "All_Until";
        case NodeType_t::All_Unless:
            return "All_Unless";
        case NodeType_t::All_Next:
            return "All_Next";
        case NodeType_t::Exist_Next:
            return "Exist_Next";
        case NodeType_t::Exist_Future:
            return "Exist_Future";
        case NodeType_t::Exist_Global:
            return "Exist_Global";
        case NodeType_t::Exist_Until:
            return "Exist_Until";
        case NodeType_t::Exist_Unless:
            return "Exist_Unless";
        case NodeType_t::True:
            return "True";
        case NodeType_t::False:
            return "False";
        case NodeType_t::None:
            return "";
        default:
            return "UNKNOWN";
    }
}

