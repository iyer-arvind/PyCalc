%{
#include "LanguageStructures.h"
#include "Dimension.h"
#include "LanguageParser.hpp"
#include <stdio.h>
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
{startdim}		{ printf("\033[31mBEGDIM::     %s\033[0m\n",yytext);	BEGIN(DIMENSION);       	}
<DIMENSION>\/		{ printf("\033[31mDIMDIV::     %s\033[0m\n",yytext);	return DIMDIV;			}
<DIMENSION>\^		{ printf("\033[31mDIMPOE::     %s\033[0m\n",yytext);	return DIMPOW;			}
<DIMENSION>{dimension}	{ printf("\033[31mDIMEL::     %s\033[0m\n",yytext);	return DIMEL;			}
<DIMENSION>{enddim}	{ printf("\033[31mENDDIM::     %s\033[0m\n",yytext);	BEGIN(0);			}
{number}		{ printf("\033[31mNUMBER::     %s\033[0m\n",yytext);	return NUMBER; 			}
{identifier} 		{ printf("\033[31mIDENTIFIER:: %s\033[0m\n",yytext);	yylval=new Identifier(yytext);return IDENTIFIER;	}
=			{ printf("\033[31mEQUALS::     %s\033[0m\n",yytext);	return OPEQ;			}
~			{ printf("\033[31mEQUALS::     %s\033[0m\n",yytext);	return OPSIM;			}
\+			{ printf("\033[31mADD::        %s\033[0m\n",yytext);	return OPADD;			}
-			{ printf("\033[31mSUB::        %s\033[0m\n",yytext);	return OPSUB;			}
\*			{ printf("\033[31mDIV::        %s\033[0m\n",yytext);	return OPMUL;			}
\/			{ printf("\033[31mMUL::        %s\033[0m\n",yytext);	return OPDIV;			}
\n			{ printf("\n---------------------------------------------------\n");}
[ \t]*
%%

int yywrap()
{return 1;}


