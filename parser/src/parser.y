%{

/* Definition section */
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include "types.hpp"
#include <Tree.hpp>

#include "Nnf.hpp"
#include "Simp.hpp"

unsigned int indx=0;
extern int yylex();
extern int yylex_destroy();
extern void scanMyThing(const std::string&);
extern int readInputForLexer( char *buffer, int maxBytesToRead );

Tree<ASTNode>* parsedQuery = nullptr;
int yyerror(char const*);
extern char* yytext;
extern Tree<ASTNode>* ParseQuery(const std::string&);
extern unsigned int AtomIndx;
extern unsigned int Eindx;

enum VerbosityLevel {
    V_SILENT = 0,
    V_ERRORS = 1,
    V_ALL = 2
};


void SetVerbosity(unsigned int verbosityLevel);

%}

%union{
	Tree<ASTNode>* tree;
	std::string* tokenString;
}
/* Rules types */
%type <tree> comparable tense source stmt expr query with_tense bool_op
%type <tokenString>  comp_op lit atom operator

/*Token specifications */
%token Token_LP
%token Token_RP
%token AND
%token OR
%token EXIST
%token ALL
%token UNTIL
%token UNLESS
%token EX
%token EF
%token EG
%token AX
%token AF
%token AG
%token EU
%token AU
%token ASGN
%token IMPLY
%token TRUE
%token DEADLOCK
%token COMMENT
%token LITERAL ATOM COMPARATOR OPERATOR NEWLINE NOT

%start stmt

%right IMPLY
%right AU
%right EU
%left OR
%left AND
%right AG
%right AF
%right AX
%right EG
%right EF
%right EX
%nonassoc NOT

%%

stmt:query              {parsedQuery = $1;}
        ;
query:source                 {/*init*/ auto* n = new Tree<ASTNode>{ASTNode{"->"}};auto * q=new Tree<ASTNode>{ASTNode{NodeType_t::Atom, "p",AtomIndx}};n->insert(*q);n->insert(*$1);$$=n;}
        ;
source:Token_LP source Token_RP		            { $$=$2; }
     |source IMPLY source             {auto* n = new Tree<ASTNode>{ASTNode{"v"}};$1->tree_apply_xianxu([](ASTNode& a) { Nnf(a); });n->insert(*$1);n->insert(*$3);$$=n;}
     |source AND source             {/*SIMPLIFY*/ auto* n = new Tree<ASTNode>{ASTNode{"&"}}; n->insert(*$1);n->insert(*$3); $$=simp(n,$1,$3);}
     |source OR source             {/*SIMPLIFY*/ auto* n = new Tree<ASTNode>{ASTNode{"v"}}; n->insert(*$1);n->insert(*$3); $$=simp(n,$1,$3);}
     |NOT source                    { /*NNF settlement*/ $2->tree_apply_xianxu([](ASTNode& a) { Nnf(a); }); $$ = $2;  }
     |AG source                 { auto* n = new Tree<ASTNode>{ASTNode{NodeType_t::All_Global, "AG"}};n->insert(*$2); $$ = n; }
     |AF source                 { auto* n = new Tree<ASTNode>{ASTNode{NodeType_t::All_Future, "AF"}}; n->insert(*$2); $$ = n; }
     |AX source                 { auto* n = new Tree<ASTNode>{ASTNode{NodeType_t::All_Next, "AX"}}; n->insert(*$2); $$ = n; }
     |EG source                 { auto* n = new Tree<ASTNode>{ASTNode{NodeType_t::Exist_Global, "EG",Eindx}}; n->insert(*$2); $$ = n; }
     |EF source                 { auto* n = new Tree<ASTNode>{ASTNode{NodeType_t::Exist_Future, "EF",Eindx}}; n->insert(*$2); $$ = n; }
     |EX source                 { auto* n = new Tree<ASTNode>{ASTNode{NodeType_t::Exist_Next, "EX",Eindx}}; n->insert(*$2); $$ = n; }
     |AU source                 { auto* n = new Tree<ASTNode>{ASTNode{NodeType_t::All_Future, "AU"}}; n->insert(*$2); $$ = n; }
     |EXIST Token_LP source UNTIL source Token_RP     {auto* n = new Tree<ASTNode>{ASTNode{NodeType_t::Exist_Until,"EU",Eindx}};n->insert(*$3);n->insert(*$5);$$=n;}
     |ALL Token_LP source UNTIL source Token_RP     {auto* n = new Tree<ASTNode>{ASTNode{NodeType_t::All_Until,"AU"}};n->insert(*$3);n->insert(*$5);$$=n;}
     |EXIST Token_LP source UNLESS source Token_RP     {auto* n = new Tree<ASTNode>{ASTNode{NodeType_t::Exist_Unless,"EW",Eindx}};n->insert(*$3);n->insert(*$5);$$=n;}
     |ALL Token_LP source UNLESS source Token_RP     {auto* n = new Tree<ASTNode>{ASTNode{NodeType_t::All_Unless,"AW"}};n->insert(*$3);n->insert(*$5);$$=n;}
     |comparable                 { $$ = $1; }
     |TRUE                      {auto* n = new Tree<ASTNode>{ASTNode{NodeType_t::True, "T"}};$$ = n;}
     ;
comparable:atom                 { $$ = new Tree<ASTNode>{ASTNode{NodeType_t::Atom, *$1}};    }
                |lit            { $$ = new Tree<ASTNode>{ASTNode{NodeType_t::Literal, "~"+(*$1)}}; }
                ;
comp_op: COMPARATOR             { $$ = new std::string(*yylval.tokenString); } ;
lit: NOT ATOM                   { $$ = new std::string(*yylval.tokenString); } ;
atom: ATOM                      { $$ = new std::string(*yylval.tokenString); } ;
operator: OPERATOR              { $$ = new std::string(*yylval.tokenString); } ;

%%
//code section
unsigned int vlvl = V_ALL;

int yyerror(char const*msg) {
    extern int yylineno;	// defined and maintained in lex.c
      extern char *yytext;	// defined and maintained in lex.c

      printf ("ERROR: %s at symbol \"%s", msg, yytext);
      printf ("\" on line %d", yylineno);
      exit(1);
}

Tree<ASTNode>* ParseQuery(const std::string& str) {
    if(parsedQuery != nullptr)
        parsedQuery = nullptr;
       if(vlvl >= V_ALL)
            printf("Parsing formula:%s\n", str.c_str());
        scanMyThing(str);
	return parsedQuery;
}

void SetVerbosity(unsigned int level) {
    vlvl = level;
}