//
// Created by mzy on 2021-05-07.
//
#include <iostream>
#include <types.hpp>
#include <vector>
#include <Tree.hpp>
#include <fstream>
#include <transform.hpp>


extern Tree<ASTNode>* ParseQuery(const std::string&);
extern void SetVerbosity(unsigned int lvl);
unsigned int AtomIndx=0;
unsigned int Eindx=0;

int Check(std::string readline)
{
    for(int n=0;n<readline.length();n++)
    {
        if(readline[n]!=9&&readline[n]!=32&&readline[n]!=13)
        {
            return 1;
        }
    }
    return 0;
}



int main(int argc,char** argv) {
//    std::string formula=argv[1];
    // Query: "Does it, for all paths, always hold that the 'my_test_var' is true?"
    SetVerbosity(2); // 0 = NONE, 1 = ERRORS_ONLY, 2 = ALL

//读取文件
        std::ifstream in(argv[1]);
        std::string formula;
        if(!in.is_open()){
            std::cout<<"can not open file"<<"\n";
            return 0;
        }
    while (!in.eof()){
        getline(in,formula);
        if(Check(formula)){
            auto* q = ParseQuery(formula);
            auto *cur1=q;

            //Translation
            std::cout<<"The SNF clauses are as follows:"<<"\n";
            std::cout<<"start -> p1"<<"\n";
            while(cur1!= nullptr) {
                if (cur1->root.type == NodeType_t::Implies) transform(*cur1);
                cur1=cur1->next;
            }
            cur1=q;
            while(cur1!= nullptr){
                cur1->PrintFormula();
                printf("\n");
                cur1=cur1->next;
            }
            formula="";
            printf("\n");
            delete q;
        }
        }

}


