%{

#include <types.hpp>
#include <Tree.hpp>
#include "parser.hpp"
#include <cstring>

int readInputForLexer( char *buffer, int maxBytesToRead );

#undef YY_INPUT
#define YY_INPUT(buf,result,maxsize) { result = readInputForLexer(buf,maxsize); if(result<=0) result = YY_NULL; }

void ProvideToken() {
    if(yylval.tokenString == nullptr) yylval.tokenString = new std::string("");
    *yylval.tokenString = ""; // reset the string (This is memory safe, dw)
    *yylval.tokenString += yytext;
}


%}


%option nounistd
%option never-interactive

LP "("
RP ")"
EXPRESSION "Φ"
COMMENT "--".*?

%%

[ \t\r]		                // ignore whitespace
{COMMENT} 	                // ignore comments
deadlock                    {ProvideToken();  return DEADLOCK;}
{LP}				        {ProvideToken();  return Token_LP; }
{RP}				        {ProvideToken();  return Token_RP; }
"::="				        {ProvideToken();  return ASGN; }
"->"				        {ProvideToken();  return IMPLY; }
T|"true"				    {ProvideToken();  return TRUE; }
"&&"|"&"			        {ProvideToken();  return AND; }
"||"|"v"			        {/*printf(" %s\n",yytext);*/ProvideToken();  return OR; }
"~"|"!"			        	{ProvideToken();  return NOT;}
EX                          {ProvideToken();  return EX; }
EF                          {ProvideToken();  return EF; }
EG                          {ProvideToken();  return EG; }
AX                          {ProvideToken();  return AX; }
AF                          {ProvideToken();  return AF; }
AG                          {ProvideToken();  return AG; }
"E"			        	    {ProvideToken();  return EXIST; }
"A"			        	    {ProvideToken();  return ALL; }
"U"			        	    {ProvideToken();  return UNTIL; }
"W"			        	    {ProvideToken();  return UNLESS; }
[a-zBCDH-SVWYZ_][a-zA-TV-Z0-9_€ðđł]*					{ProvideToken(); return ATOM; }
(<=|<|!=|==|>=|>)	        {ProvideToken(); return COMPARATOR;}
(\+|\-|\*|\/)               {ProvideToken(); return OPERATOR;}
\n				            {ProvideToken(); return NEWLINE;}
.

%%

int yywrap() {
	return 1;
}

int globalReadOffset;
char *globalInputText = nullptr;

void scanMyThing(const std::string& ss) {
	globalInputText = (char*)calloc(ss.length()+30, 1);
	globalReadOffset = 0;
	memcpy(globalInputText, ss.c_str(), ss.size());
	YY_FLUSH_BUFFER;
	yyparse();
	free(globalInputText);
	BEGIN (INITIAL); // Reset the parser
}

int readInputForLexer( char *buffer, int maxBytesToRead ) {
    int numBytesToRead = maxBytesToRead;
    int bytesRemaining = strlen(globalInputText)-globalReadOffset;
    int i;
    if ( numBytesToRead > bytesRemaining ) { numBytesToRead = bytesRemaining; }
    for ( i = 0; i < numBytesToRead; i++ ) {
        buffer[i] = globalInputText[globalReadOffset+i];
    }
    globalReadOffset += numBytesToRead;
    return numBytesToRead;
}