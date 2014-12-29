#ifndef DIMENSION_H_INCLUDED
#define DIMENSION_H_INCLUDED
#include "LanguageStructures.h"
#include <iostream>
#include <cstring>

#define NDIMS 10


#include <libxml++/libxml++.h>

enum DIMENSIONS {MASS=0,LENGTH,TIME,TEMPERATURE,MOLE,CHARGE,ANGLE,SOLID_ANGLE,LUMINOUS_INTENSITY,GAPI};


struct DimensionDef
{
	std::string name,id,baseUnit,annotation;
	double power[NDIMS];
	bool isBaseUnit;
	bool __isValid;
	double factor;
	DimensionDef():
		isBaseUnit(false),
		__isValid(true),
		factor(1)
	{
		for(int i=0;i<NDIMS;i++)power[i]=0.0;
	}
};


std::ostream& operator<<(std::ostream& stream, const DimensionDef &d);

class DimensionCache:public xmlpp::SaxParser
{
	static std::map<std::string,DimensionDef> __cache;
	void __loadDimensions();
	enum STATE {ROOT=0,UNITS_DEFINITION,UNIT_OF_MEASURE,NAME,DIMENSIONAL_CLASS,BASE_UNIT,CONVERSION_TO_BASE_UNIT,FACTOR,FRACTION,NUMERATOR,DENOMINATOR};
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







class Number;
class Dimension:public LanguageStructure
{
	
	double __power[NDIMS];
	double __factor;
public:
	Dimension();
	Dimension(std::string);
	const Dimension& operator*=(const Dimension& d);
	const Dimension& operator/=(const Dimension& d);
	const Dimension& operator^=(const Number& d);
	double valueSI()const;

	~Dimension();

};

class Number:public LanguageStructure
{
	double __number;
	Dimension __dim;
public:
	Number(double num):
		__number(num)
	{
		LOG("Created number:"<<num)
	}
	void setDimension(const Dimension& dimension);
	double valueSI()const;

	const Number& operator*=(const Dimension&);
	STREAM_WRITER
	virtual ~Number();
};
STREAM_OPERATOR(Number)

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
