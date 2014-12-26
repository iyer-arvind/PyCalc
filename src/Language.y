%{
#include <cstdio>
#include "LanguageStructures.h"
#include "Dimension.h"
void yyerror (char *s) {fprintf (stderr, "ERROR: %s\n", s);}
int yylex();


%}


%token NUMBER
%token IDENTIFIER
%token OPEQ OPADD OPSUB OPMUL OPDIV OPPOW OPSIM
%token BEGDIM ENDDIM DIMEL DIMMUL DIMDIV DIMPOW
%token UNIDENTIFIED

%%
program:
       	equation					{printf("program:1 %d\n",__LINE__);}
	| program equation				{printf("program:2 %d\n",__LINE__);}
	| program guess					{printf("program:2 %d\n",__LINE__);}
	;

equation:
	expression OPEQ expression			{printf("equation:1 %d\n",__LINE__);}
	;

guess:
     	expression OPSIM expression			{printf("guess:1 %d\n",__LINE__);}

expression:			
	exponent					{printf("expression:1 %d\n",__LINE__);}
	| expression OPADD exponent			{printf("expression:2 %d\n",__LINE__);}
	| expression OPSUB exponent			{printf("expression:3 %d\n",__LINE__);}
	| '(' expression ')'				{printf("expression:4 %d\n",__LINE__);}
	;
exponent:
	product						{printf("exponent:1 %d\n",__LINE__);}
	| exponent OPPOW product			{printf("exponent:2 %d\n",__LINE__);}
	| '(' exponent ')'				{printf("exponent:3 %d\n",__LINE__);}
	;
product:
	element						{printf("product:1 %d\n",__LINE__);}
	| product OPMUL element				{printf("product:2 %d\n",__LINE__);}
	| product OPDIV element				{printf("product:3 %d\n",__LINE__);}
	| '(' product ')'				{printf("product:4 %d\n",__LINE__);}
	;
element:
	IDENTIFIER					{printf("element:1 %d\n",__LINE__);$$=$1;}
	| dimensionedNumber				{printf("element:2 %d\n",__LINE__);$$=$1;}
	;

dimensionedNumber:
	NUMBER						{printf("dimensionedNumber:1 %d\n",__LINE__);$$=$1;}
	| dimensionedNumber dimension 			{printf("dimensionedNumber:2 %d\n",__LINE__);(*(dynamic_cast<Number*>($1)))*=(*(dynamic_cast<Dimension*>($2)));$$=$1;}
	;

dimension:						
	DIMEL						{printf("dimension:2 %d\n",__LINE__);}
	| dimension DIMEL				{printf("dimension:3 %d\n",__LINE__);}
	| dimension DIMDIV DIMEL			{printf("dimension:4 %d\n",__LINE__);}
	| dimension DIMPOW NUMBER			{printf("dimension:5 %d\n",__LINE__);}
	;


%%


