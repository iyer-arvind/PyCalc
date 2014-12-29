%{
#include <cstdio>
#include "LanguageStructures.h"
#include "Dimension.h"
void yyerror (char *s) {fprintf (stderr, "ERROR: %s\n", s);}
int yylex();

#define AS(TYP,PTR) *(dynamic_cast<TYP*>(PTR))

%}


%token NUMBER
%token IDENTIFIER
%token OPEQ OPADD OPSUB OPMUL OPDIV OPPOW OPSIM
%token BEGDIM ENDDIM DIMEL DIMMUL DIMDIV DIMPOW
%token UNIDENTIFIED

%%
program:
       	equation					{LOG("program");}
	| program equation				{LOG("program");}
	| program guess					{LOG("program");}
	;

equation:
	expression OPEQ expression			{LOG("equation");}
	;

guess:
     	expression OPSIM expression			{LOG("guess");}

expression:			
	exponent					{LOG("expression");}
	| expression OPADD exponent			{LOG("expression");}
	| expression OPSUB exponent			{LOG("expression");}
	| '(' expression ')'				{LOG("expression");}
	;
exponent:
	product						{LOG("exponent");}
	| exponent OPPOW product			{LOG("exponent");}
	| '(' exponent ')'				{LOG("exponent");}
	;
product:
	element						{LOG("product");}
	| product OPMUL element				{LOG("product");}
	| product OPDIV element				{LOG("product");}
	| '(' product ')'				{LOG("product");}
	;
element:
	IDENTIFIER					{LOG("element");$$=$1;}
	| dimensionedNumber				{LOG("element");$$=$1;}
	;

dimensionedNumber:
	NUMBER						{LOG("dimensionedNumber");$$=$1;}
	| dimensionedNumber dimension 			{LOG("dimensionedNumber");AS(Number,$1)*=AS(Dimension,$2);$$=$1;}
	;

dimension:						
	DIMEL						{LOG("dimension");$$=$1;}
	| dimension DIMEL				{LOG("dimension");AS(Dimension,$1)*=AS(Dimension,$2);$$=$1;}
	| dimension DIMDIV DIMEL			{LOG("dimension");AS(Dimension,$1)/=AS(Dimension,$3);$$=$1;}
	| dimension DIMPOW NUMBER			{LOG("dimension");AS(Dimension,$1)^=AS(Number,$3);$$=$1;}
	;


%%


