#include "Dimension.h"
#include <cstring>


std::map<std::string,DimensionDef> __dimensionCache;

void loadDimensions()
{
	if(__dimensionCache.size())return;
}


Dimension::Dimension():
	__factor(1.0)
{
	memset(__powers,sizeof(int)*NDIMS,0);
}

Dimension::Dimension(std::string)
{

}

double Dimension::valueSI()const
{
	return __factor;
}

const Dimension& Dimension::operator*=(const Dimension& dim)
{
	for(int i=0;i<NDIMS;i++){__powers[i]+=dim.__powers[i];}
	__factor*=dim.__factor;
	return *this;
}

const Dimension& Dimension::operator/=(const Dimension& dim)
{
	for(int i=0;i<NDIMS;i++){__powers[i]-=dim.__powers[i];}
	__factor/=dim.__factor;
	return *this;
}

Dimension::~Dimension()
{}

const Number& Number::operator*=(const Dimension& dimension)
{
	__dim*=dimension;
	return  *this;
}

double Number::valueSI()const
{
	return __number*__dim.valueSI();
}


Number::~Number()
{}

Identifier::Identifier(std::string name):__name(name)
{}

Identifier::SymbolTable Identifier::__symbolTable;

double Identifier::valueSI()const
{
	return 0;
}

Identifier::~Identifier()
{}
