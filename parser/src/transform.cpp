//
// Created by mzy on 2021-06-20.
//
#include "transform.hpp"

void exchange(Tree<ASTNode>& a,Tree<ASTNode>& b){
    Tree<ASTNode> tmp = a;
    a=b;
    b=tmp;
}

void transform(Tree<ASTNode>& n){
NodeType_t &child = n.children[1].root.type;

//rule3
if(child==NodeType_t::LogicAnd){
    auto *T = new Tree<ASTNode>{ASTNode{"->"}};
    T->insert(n.children[0]);T->insert(n.children[1].children[0]);
    n.children[1]=n.children[1].children[1];
    transform(n);

    auto *it = n.next;
    if(it!= nullptr){
        while (it->next!= nullptr)it=it->next;
        it->next=T;
    }
    else n.next=T;
    return;
}

//rule4
if(child==NodeType_t::LogicOr && !n.children[1].children[1].isDisjunction()){
    auto *p = new Tree<ASTNode>{ASTNode{NodeType_t::Atom, "p", AtomIndx}};
    auto *T = new Tree<ASTNode>{ASTNode{"->"}};
    T->insert(*p);T->insert(n.children[1].children[1]);
    auto* fresh = new Tree<ASTNode>{ASTNode{"v"}};
    fresh->insert(n.children[1].children[0]);fresh->insert(*p);
    n.children[1]=*fresh;

    auto *it = n.next;
    if(it!= nullptr){
        while (it->next!= nullptr)it=it->next;
        it->next=T;
    }
    else n.next=T;
    transform(n);

    return;
}
if(child==NodeType_t::LogicOr && !n.children[1].children[0].isDisjunction()){
    auto *p = new Tree<ASTNode>{ASTNode{NodeType_t::Atom, "p", AtomIndx}};
    auto *T = new Tree<ASTNode>{ASTNode{"->"}};
    T->insert(*p);T->insert(n.children[1].children[0]);
    auto* fresh = new Tree<ASTNode>{ASTNode{"v"}};
    fresh->insert(n.children[1].children[1]);fresh->insert(*p);
    n.children[1]=*fresh;

    auto *it = n.next;
    if(it!= nullptr){
        while (it->next!= nullptr)it=it->next;
        it->next=T;
    }
    else n.next=T;
    transform(n);

    return;
}

//rule5
if(child==NodeType_t::True){
    n.children.clear();
    n.root.type=NodeType_t::None;
    n.root.token="";
    return;
}
if(child==NodeType_t::False){
    Nnf(n.children[0].root);
    Nnf(n.children[1].root);
    exchange(n.children[0],n.children[1]);
    return;
}
if(n.children[1].isDisjunction()){
    Nnf(n.children[0].root);
    auto* fresh = new Tree<ASTNode>{ASTNode{"v"}};
    fresh->insert(n.children[0]);fresh->insert(n.children[1]);
    n.children[0].root.token="T";
    n.children[0].root.type=NodeType_t::True;
    n.children[1]=*fresh;
    return;
}

//rule6
if(child==NodeType_t::Exist_Next && !n.children[1].children[0].isDisjunction()){
    auto *p = new Tree<ASTNode>{ASTNode{NodeType_t::Atom, "p", AtomIndx}};
    auto *T = new Tree<ASTNode>{ASTNode{"->"}};
    T->insert(*p);T->insert(n.children[1].children[0]);
    auto* fresh = new Tree<ASTNode>{ASTNode{NodeType_t::Exist_Next, "EX"}};
    fresh->insert(*p);fresh->root.index=n.children[1].root.index;
    n.children[1]=*fresh;

    auto *it = n.next;
    if(it!= nullptr){
        while (it->next!= nullptr)it=it->next;
        it->next=T;
    }
    else n.next=T;

    return;
}
if(child==NodeType_t::All_Next && !n.children[1].children[0].isDisjunction()){
    auto *p = new Tree<ASTNode>{ASTNode{NodeType_t::Atom, "p", AtomIndx}};
    auto *T = new Tree<ASTNode>{ASTNode{"->"}};
    T->insert(*p);T->insert(n.children[1].children[0]);
    auto* fresh = new Tree<ASTNode>{ASTNode{NodeType_t::All_Next, "AX"}};
    fresh->insert(*p);fresh->root.index=n.children[1].root.index;
    n.children[1]=*fresh;

    auto *it = n.next;
    if(it!= nullptr){
        while (it->next!= nullptr)it=it->next;
        it->next=T;
    }
    else n.next=T;

    return;
}

//rule7
if(child==NodeType_t::Exist_Future && !(n.children[1].children[0].root.type==NodeType_t::Atom||n.children[1].children[0].root.type==NodeType_t::Literal)){
    auto *p = new Tree<ASTNode>{ASTNode{NodeType_t::Atom, "p", AtomIndx}};
    auto *T = new Tree<ASTNode>{ASTNode{"->"}};
    T->insert(*p);T->insert(n.children[1].children[0]);
    auto* fresh = new Tree<ASTNode>{ASTNode{NodeType_t::Exist_Future, "EF"}};
    fresh->insert(*p);fresh->root.index=n.children[1].root.index;
    n.children[1]=*fresh;

    auto *it = n.next;
    if(it!= nullptr){
        while (it->next!= nullptr)it=it->next;
        it->next=T;
    }
    else n.next=T;
    return;
}
if(child==NodeType_t::All_Future && !(n.children[1].children[0].root.type==NodeType_t::Atom||n.children[1].children[0].root.type==NodeType_t::Literal)){
    auto *p = new Tree<ASTNode>{ASTNode{NodeType_t::Atom, "p", AtomIndx}};
    auto *T = new Tree<ASTNode>{ASTNode{"->"}};
    T->insert(*p);T->insert(n.children[1].children[0]);
    auto* fresh = new Tree<ASTNode>{ASTNode{NodeType_t::All_Future, "AF"}};
    fresh->insert(*p);fresh->root.index=n.children[1].root.index;
    n.children[1]=*fresh;

    auto *it = n.next;
    if(it!= nullptr){
        while (it->next!= nullptr)it=it->next;
        it->next=T;
    }
    else n.next=T;

    return;
}

//rule8
if(child==NodeType_t::Exist_Until && !(n.children[1].children[1].root.type==NodeType_t::Atom||n.children[1].children[1].root.type==NodeType_t::Literal)){
    auto *p = new Tree<ASTNode>{ASTNode{NodeType_t::Atom, "p", AtomIndx}};
    auto *T = new Tree<ASTNode>{ASTNode{"->"}};
    T->insert(*p);T->insert(n.children[1].children[1]);

    auto *it = n.next;
    if(it!= nullptr){
        while (it->next!= nullptr)it=it->next;
        it->next=T;
    }
    else n.next=T;

    auto* fresh = new Tree<ASTNode>{ASTNode{NodeType_t::Exist_Until, "EU"}};
    fresh->insert(n.children[1].children[0]);fresh->insert(*p);fresh->root.index=n.children[1].root.index;
    n.children[1]=*fresh;
    return;
}
if(child==NodeType_t::All_Until && !(n.children[1].children[1].root.type==NodeType_t::Atom||n.children[1].children[1].root.type==NodeType_t::Literal)){
    auto *p = new Tree<ASTNode>{ASTNode{NodeType_t::Atom, "p", AtomIndx}};
    auto *T = new Tree<ASTNode>{ASTNode{"->"}};
    T->insert(*p);T->insert(n.children[1].children[1]);

    auto *it = n.next;
    if(it!= nullptr){
        while (it->next!= nullptr)it=it->next;
        it->next=T;
    }
    else n.next=T;

    auto* fresh = new Tree<ASTNode>{ASTNode{NodeType_t::All_Until, "AU"}};
    fresh->insert(n.children[1].children[0]);fresh->insert(*p);fresh->root.index=n.children[1].root.index;
    n.children[1]=*fresh;

    return;
}

//rule9
if(child==NodeType_t::Exist_Until && !(n.children[1].children[1].root.type==NodeType_t::Atom||n.children[1].children[1].root.type==NodeType_t::Literal)){
    auto *p = new Tree<ASTNode>{ASTNode{NodeType_t::Atom, "p", AtomIndx}};
    auto *T = new Tree<ASTNode>{ASTNode{"->"}};
    T->insert(*p);T->insert(n.children[1].children[1]);

    auto *it = n.next;
    if(it!= nullptr){
        while (it->next!= nullptr)it=it->next;
        it->next=T;
    }
    else n.next=T;

    auto* fresh = new Tree<ASTNode>{ASTNode{NodeType_t::Exist_Unless, "EW"}};
    fresh->insert(n.children[1].children[0]);fresh->insert(*p);fresh->root.index=n.children[1].root.index;
    n.children[1]=*fresh;

    return;
}
if(child==NodeType_t::All_Until && !(n.children[1].children[1].root.type==NodeType_t::Atom||n.children[1].children[1].root.type==NodeType_t::Literal)){
    auto *p = new Tree<ASTNode>{ASTNode{NodeType_t::Atom, "p", AtomIndx}};
    auto *T = new Tree<ASTNode>{ASTNode{"->"}};
    T->insert(*p);T->insert(n.children[1].children[1]);

    auto *it = n.next;
    if(it!= nullptr){
        while (it->next!= nullptr)it=it->next;
        it->next=T;
    }
    else n.next=T;

    auto* fresh = new Tree<ASTNode>{ASTNode{NodeType_t::All_Unless, "AW"}};
    fresh->insert(n.children[1].children[0]);fresh->insert(*p);fresh->root.index=n.children[1].root.index;
    n.children[1]=*fresh;

    return;
}

//rule10
if(child==NodeType_t::Exist_Global){
    auto *p = new Tree<ASTNode>{ASTNode{NodeType_t::Atom, "p", AtomIndx}};
    auto *T1 = new Tree<ASTNode>{ASTNode{"->"}};auto *T2 = new Tree<ASTNode>{ASTNode{"->"}};
    T1->insert(*p);T1->insert(n.children[1].children[0]);
    auto* fresh = new Tree<ASTNode>{ASTNode{NodeType_t::Exist_Next, "EX"}};
    fresh->insert(*p);fresh->root.index=n.children[1].root.index;
    T2->insert(*p);T2->insert(*fresh);
    T1->next=T2;
    n.children[1]=*p;
    transform(n);

    auto *it = n.next;
    if(it!= nullptr){
        while (it->next!= nullptr)it=it->next;
        it->next=T1;
    }
    else n.next=T1;
    return;
}
if(child==NodeType_t::All_Global){
    auto *p = new Tree<ASTNode>{ASTNode{NodeType_t::Atom, "p", AtomIndx}};
    auto *T1 = new Tree<ASTNode>{ASTNode{"->"}};auto *T2 = new Tree<ASTNode>{ASTNode{"->"}};
    T1->insert(*p);T1->insert(n.children[1].children[0]);
    auto* fresh = new Tree<ASTNode>{ASTNode{NodeType_t::All_Next, "AX"}};
    fresh->insert(*p);fresh->root.index=n.children[1].root.index;
    T2->insert(*p);T2->insert(*fresh);
    T1->next=T2;
    n.children[1]=*p;
    transform(n);

    auto *it = n.next;
    if(it!= nullptr){
        while (it->next!= nullptr)it=it->next;
        it->next=T1;
    }
    else n.next=T1;
    return;
}

//rule11
if(child==NodeType_t::Exist_Until && (n.children[1].children[1].root.type==NodeType_t::Atom||n.children[1].children[1].root.type==NodeType_t::Literal)){
    auto *p = new Tree<ASTNode>{ASTNode{NodeType_t::Atom, "p", AtomIndx}};
    auto *T1 = new Tree<ASTNode>{ASTNode{"v"}};
    auto *T2 = new Tree<ASTNode>{ASTNode{"->"}};
    auto *T3 = new Tree<ASTNode>{ASTNode{"->"}};
    auto *T4 = new Tree<ASTNode>{ASTNode{"->"}};

    T1->insert(n.children[1].children[1]);T1->insert(*p);

    T2->insert(*p);T2->insert(n.children[1].children[0]);
    T1->next=T2;

    auto* fresh = new Tree<ASTNode>{ASTNode{NodeType_t::Exist_Next, "EX"}};
    fresh->insert(*T1);fresh->root.index=n.children[1].root.index;
    T3->insert(*p);T3->insert(*fresh);
    T2->next=T3;

    auto* fresh2 = new Tree<ASTNode>{ASTNode{NodeType_t::Exist_Future, "EF"}};
    fresh2->insert(n.children[1].children[1]);fresh2->root.index=n.children[1].root.index;
    T4->insert(n.children[0]);T4->insert(*fresh2);
    T3->next=T4;

    n.children[1]=*T1;

    auto *it = n.next;
    if(it!= nullptr){
        while (it->next!= nullptr)it=it->next;
        it->next=T2;
    }
    else n.next=T2;
    return;
}
if(child==NodeType_t::All_Until && (n.children[1].children[1].root.type==NodeType_t::Atom||n.children[1].children[1].root.type==NodeType_t::Literal)){
    auto *p = new Tree<ASTNode>{ASTNode{NodeType_t::Atom, "p", AtomIndx}};
    auto *T1 = new Tree<ASTNode>{ASTNode{"v"}};
    auto *T2 = new Tree<ASTNode>{ASTNode{"->"}};
    auto *T3 = new Tree<ASTNode>{ASTNode{"->"}};
    auto *T4 = new Tree<ASTNode>{ASTNode{"->"}};

    T1->insert(n.children[1].children[1]);T1->insert(*p);

    T2->insert(*p);T2->insert(n.children[1].children[0]);
    T1->next=T2;

    auto* fresh = new Tree<ASTNode>{ASTNode{NodeType_t::All_Next, "AX"}};
    fresh->insert(*T1);fresh->root.index=n.children[1].root.index;
    T3->insert(*p);T3->insert(*fresh);
    T2->next=T3;

    auto* fresh2 = new Tree<ASTNode>{ASTNode{NodeType_t::All_Future, "AF"}};
    fresh2->insert(n.children[1].children[1]);fresh2->root.index=n.children[1].root.index;
    T4->insert(n.children[0]);T4->insert(*fresh2);
    T3->next=T4;

    n.children[1]=*T1;

    auto *it = n.next;
    if(it!= nullptr){
        while (it->next!= nullptr)it=it->next;
        it->next=T2;
    }
    else n.next=T2;
    return;
}

//rule12
if(child==NodeType_t::Exist_Unless && (n.children[1].children[0].root.type==NodeType_t::Atom||n.children[1].children[0].root.type==NodeType_t::Literal)){
    auto *p = new Tree<ASTNode>{ASTNode{NodeType_t::Atom, "p", AtomIndx}};
    auto *T1 = new Tree<ASTNode>{ASTNode{"v"}};
    auto *T2 = new Tree<ASTNode>{ASTNode{"->"}};
    auto *T3 = new Tree<ASTNode>{ASTNode{"->"}};

    T1->insert(n.children[1].children[1]);T1->insert(*p);

    T2->insert(*p);T2->insert(n.children[1].children[0]);
    T1->next=T2;

    auto* fresh = new Tree<ASTNode>{ASTNode{NodeType_t::Exist_Next, "EX"}};
    fresh->insert(*T1);fresh->root.index=n.children[1].root.index;
    T3->insert(*p);T3->insert(*fresh);
    T2->next=T3;

    n.children[1]=*T1;
    auto *it = n.next;
    if(it!= nullptr){
        while (it->next!= nullptr)it=it->next;
        it->next=T2;
    }
    else n.next=T2;
    return;
}
if(child==NodeType_t::All_Unless && (n.children[1].children[0].root.type==NodeType_t::Atom||n.children[1].children[0].root.type==NodeType_t::Literal)){
    auto *p = new Tree<ASTNode>{ASTNode{NodeType_t::Atom, "p", AtomIndx}};
    auto *T1 = new Tree<ASTNode>{ASTNode{"v"}};
    auto *T2 = new Tree<ASTNode>{ASTNode{"->"}};
    auto *T3 = new Tree<ASTNode>{ASTNode{"->"}};

    T1->insert(n.children[1].children[1]);T1->insert(*p);

    T2->insert(*p);T2->insert(n.children[1].children[0]);
    T1->next=T2;

    auto* fresh = new Tree<ASTNode>{ASTNode{NodeType_t::All_Next, "AX"}};
    fresh->insert(*T1);fresh->root.index=n.children[1].root.index;
    T3->insert(*p);T3->insert(*fresh);
    T2->next=T3;

    n.children[1]=*T1;
    auto *it = n.next;
    if(it!= nullptr){
        while (it->next!= nullptr)it=it->next;
        it->next=T2;
    }
    else n.next=T2;
    return;
}

}