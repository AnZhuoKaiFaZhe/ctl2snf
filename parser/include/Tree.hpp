#ifndef TREE_HPP
#define TREE_HPP
#include <vector>
#include <iostream>
#include "types.hpp"
#include <functional>

extern void Nnf(ASTNode & n);
extern unsigned int AtomIndx;
extern unsigned int Eindx;


//// N-Tree implementation
template<typename T>
struct Tree {
    T root;
    std::vector<Tree<T>> children;
    struct Tree<T> * next= nullptr;
    Tree() = delete;

    explicit Tree(const T &r) : root(r), children{} {}

    explicit Tree(T &&r) : root{std::forward<T>(r)}, children{} {}

    /** * Moves the provided subtree into this tree's children */
    inline void emplace(Tree<T> &&t) { children.emplace_back(std::forward(t)); }

    /** * Copies the provided subtree into this tree's children */
    void insert(const Tree<T> &t) {
        children.push_back(t);
    }

    void join(const Tree<T> *t) {
        children.push_back(*t);
    }

    template<typename F>
    void tree_accumulate(std::function<F(T &, const F &)> f, F start_val) {
        auto r = f(root, start_val);
        for (auto &c : children) c.tree_accumulate(f, r);
    }

    void Print(ASTNode &n)
    {
        if(n.type==NodeType_t::Exist_Future){std::cout << "E"<<"<"<<n.index<<">F"<<" ";}
        else if(n.type==NodeType_t::Exist_Next){std::cout << "E"<<"<"<<n.index<<">X"<<" ";}
        else if(n.type==NodeType_t::Exist_Global){std::cout << "E"<<"<"<<n.index<<">G"<<" ";}
        else if((n.type==NodeType_t::Atom || n.type==NodeType_t::Literal) &&n.index!=0) {std::cout << n.token<<n.index<<" ";}
        else
            std::cout << n.token<<" ";
    }

//    void PrintFormula(std::function<void(T &)> f) {
//        while(cur!= nullptr){
//        if (children.size() <= 1) {
//            f(root);
//            for (auto &c : children) c.tree_apply_xianxu(f);
//        }
//        else if (children.size() == 2) {
//            children[0].midOrder(f);
//            f(root);
//            children[1].midOrder(f);
//        }
//            cur=cur->next;
//            printf("\n");
//            cur->PrintFormula(f);
//        }
//    }
    void PrintFormula() {
            if (children.size() <= 1) {
                Print(root);
                for (auto &c : children) c.midOrder();
            }
            else if (children.size() == 2) {
                if(root.type!=NodeType_t::Implies){
                    printf("(");
                    children[0].midOrder();
                    Print(root);
                    children[1].midOrder();
                    printf(")");
                } else{
                    children[0].midOrder();
                    Print(root);
                    children[1].midOrder();
            }
        }
    }

    void midOrder() {
        if (children.size() <= 1) {
            Print(root);
            for (auto &c : children) c.midOrder();
        }
        else if(children.size() == 2){
            printf("(");
            children[0].midOrder();
            Print(root);
            children[1].midOrder();
            printf(")");
        }
    }
        void tree_apply_xianxu(std::function<void(T &)> f) {
            f(root);
            for (auto &c : children) c.tree_apply_xianxu(f);
        }

        void tree_apply_reverse(std::function<void(T &)> f) {
            for (auto &c : children) c.tree_apply(f);
            f(root);
        }
        void tree_apply(std::function<void(const T &)> f) const {
            f(*this);
            for (auto &c : children) c.tree_apply(f);
        }
        void tree_apply(std::function<void(const Tree<T> &)> f) const {
            f(*this);
            for (auto &c : children) c.tree_apply(f);
        }
        void tree_apply(std::function<void(Tree<T> &)> f) {
            f(*this);
            for (auto &c : children) { c.tree_apply(f); }
        }
    bool isDisjunction(){ if (root.type==NodeType_t::LogicOr&&isLiteral(children[1].root.type)&&isLiteral(children[0].root.type)||isLiteral(root.type))return true; else return false;}
};
    template<typename T>
    void apply_tree_func_on_non_tree(T *orig, std::function<T *(T *)> getnext, std::function<void(T *)> f) {
        T *n = getnext(orig);
        if (n) apply_tree_func_on_non_tree(n, getnext, f);
        f(orig);
    }
#endif //TREE_HPP

