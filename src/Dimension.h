#ifndef DIMENSION_H_INCLUDED
#define DIMENSION_H_INCLUDED
#include "LanguageStructures.h"
#include <iostream>

#define NDIMS 6

#include <libxml++/libxml++.h>

struct DimensionDef
{
	std::string name,id,baseName;
	double powers[NDIMS];
	bool isBaseUnit;
	double factor;
	DimensionDef():
		isBaseUnit(false),
		factor(1)
	{
		memset(powers,NDIMS*sizeof(double),0);
	}
};

class DimensionCache:public xmlpp::SaxParser
{
	static std::map<std::string,DimensionDef> __cache;
	void __loadDimensions();
	enum STATE {ROOT=0,UNITS_DEFINITION,UNIT_OF_MEASURE,NAME,QUANTITY_TYPE,DIMENSIONAL_CLASS,BASE_UNIT};
	int __state;
	DimensionDef __tempDim;
public:
	DimensionCache():__state(ROOT)
	{
		__loadDimensions();
	}

	void on_start_element(const Glib::ustring& name,const AttributeList& properties);
	void on_characters(const Glib::ustring& characters);
	void on_end_element(const Glib::ustring& name);

};

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
