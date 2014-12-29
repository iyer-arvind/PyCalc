%{
#include "LanguageStructures.h"
#include "Dimension.h"
#include "LanguageParser.hpp"
#include <stdio.h>
#include "Macros.h"
%}


dseq      ([[:digit:]]+)
dseq_opt  ([[:digit:]]*)
frac      (({dseq_opt}"."{dseq})|{dseq}".")
exp       ([eE][+-]?{dseq})
exp_opt   ({exp}?)
fsuff     [flFL]
fsuff_opt ({fsuff}?)
hpref     (0[xX])
hdseq     ([[:xdigit:]]+)
hdseq_opt ([[:xdigit:]]*)
hfrac     (({hdseq_opt}"."{hdseq})|({hdseq}"."))
bexp      ([pP][+-]?{dseq})
dfc       (({frac}{exp_opt}{fsuff_opt})|({dseq}{exp}{fsuff_opt}))
hfc       (({hpref}{hfrac}{bexp}{fsuff_opt})|({hpref}{hdseq}{bexp}{fsuff_opt}))


number  	({dfc}|{hfc}|{dseq})
identifier 	([a-zA-Z]([a-zA-Z_]|[[:digit:]])*)
dimension	([a-zA-Z]+)
dimsp		([[:space:]])

startdim	(\[)
enddim		(\])

%s DIMENSION

%%
{startdim}		{ LOG("BEGDIM::     "<<yytext);	BEGIN(DIMENSION);       	}
<DIMENSION>\/		{ LOG("DIMDIV::     "<<yytext);	return DIMDIV;			}
<DIMENSION>\^		{ LOG("DIMPOE::     "<<yytext);	return DIMPOW;			}
<DIMENSION>{dimension}	{ LOG("DIMEL::      "<<yytext);	yylval=new Dimension(yytext);return DIMEL;		}
<DIMENSION>{enddim}	{ LOG("ENDDIM::     "<<yytext);	BEGIN(0);			}
<DIMENSION>{number}	{ LOG("DIMNUM::     "<<yytext);	yylval=new Number(atof(yytext));return NUMBER;		}
{number}		{ LOG("NUMBER::     "<<yytext);	yylval=new Number(atof(yytext));return NUMBER; 		}
{identifier} 		{ LOG("IDENTIFIER:: "<<yytext);	yylval=new Identifier(yytext);return IDENTIFIER;	}
=			{ LOG("EQUALS::     "<<yytext);	return OPEQ;			}
~			{ LOG("EQUALS::     "<<yytext);	return OPSIM;			}
\+			{ LOG("ADD::        "<<yytext);	return OPADD;			}
-			{ LOG("SUB::        "<<yytext);	return OPSUB;			}
\*			{ LOG("DIV::        "<<yytext);	return OPMUL;			}
\/			{ LOG("MUL::        "<<yytext);	return OPDIV;			}
\n			{ printf("\n---------------------------------------------------\n");}
[ \t]*
%%

int yywrap()
{return 1;}


