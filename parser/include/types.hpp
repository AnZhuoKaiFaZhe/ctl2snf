//
// Created by mzy on 2021-05-03.
//

#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <utility>

enum class NodeType_t:unsigned int {
    Literal,Atom,Deadlock,
    LogicAnd, LogicOr,Implies,
    Not,
    CompLess, CompLessEq, CompNeq, CompEq, CompGreater, CompGreaterEq,
    True,False,
    SubExpr,Expr,
    Exist_Future, Exist_Global, Exist_Until,Exist_Unless,All_Until,All_Unless,
    Exist_Next, All_Next,All_Future,All_Global,None,
    UNKNOWN
};
NodeType_t FromString(const std::string & string);
std::string ConvertToString(const NodeType_t&);
extern unsigned int AtomIndx;
extern unsigned int Eindx;
bool isLiteral(const NodeType_t&);

struct ASTNode{
    NodeType_t type;
    std::string token;
    unsigned int rules=0;
    unsigned int index=0;
    explicit ASTNode(std::string token):type{FromString(token)},token{std::move(token)}{}
    ASTNode(const NodeType_t& type,std::string token):type{type},token{std::move(token)}{}
    ASTNode(const NodeType_t& type,std::string token,unsigned int &idx):type{type},token{std::move(token)},index{++idx}{}
};
#endif //TYPES_H
