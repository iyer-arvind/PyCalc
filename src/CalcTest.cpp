#include "LanguageParser.hpp"
#include <stdio.h>


int main (int argc, const char **argv) 
{
	if(argc<2)return 1;
        extern FILE* yyin;
        const char *sourceFile=argv[1];
        yyin=fopen(sourceFile,"r");
	return yyparse ( );
}
