#ifndef DIMENSION_H_INCLUDED
#define DIMENSION_H_INCLUDED
#include "LanguageStructures.h"

#define NDIMS 6


struct DimensionDef
{
	double __powers[NDIMS];
	std::string __name;
	std::string __baseName;
	double __factor;
};

extern std::map<std::string,DimensionDef> __dimensionCache;

void loadDimensions();


class Dimension:public LanguageStructure
{
	
	double __powers[NDIMS];
	double __factor;
public:
	Dimension();
	Dimension(std::string);
	const Dimension& operator*=(const Dimension& d);
	const Dimension& operator/=(const Dimension& d);
	double valueSI()const;

	~Dimension();

};

class Number:public LanguageStructure
{
	double __number;
	Dimension __dim;
public:
	void setDimension(const Dimension& dimension);
	double valueSI()const;
	const Number& operator*=(const Dimension&);
	virtual ~Number();
};

class Identifier:public LanguageStructure
{
	typedef std::map<std::string,Identifier> SymbolTable;
	static SymbolTable __symbolTable;

	std::string __name;
	Dimension __dim;
public:
	Identifier(std::string name);
	void setDimension(const Dimension& dimension);
	double valueSI()const;
	virtual ~Identifier();
};
#endif
