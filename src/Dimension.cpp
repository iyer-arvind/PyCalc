#include "Dimension.h"
#include <cstring>


std::map<std::string,DimensionDef> DimensionCache::__cache;

void  DimensionCache::__loadDimensions()
{
	//if(__cache.size())return;
	std::cout<<"Loading Cache\n";
	this->parse_file("/home/arvind/Codes/PyCalc/src/poscUnits22.xml");
}

void DimensionCache::on_start_element(const Glib::ustring& name,const AttributeList& properties)
{
	//std::cout<<"START name: "<<name<<" state: "<<__state<<"\n";
	if(__state==ROOT)
	{
		if(name=="UnitsDefinition")
		{
			//std::cout<<"UnitsDefinition";
			__state=UNITS_DEFINITION;
			return;
		}
	}
	if(__state==UNITS_DEFINITION)
	{
		__tempDim=DimensionDef();
		if(name=="UnitOfMeasure")
		{
			//std::cout<<"UnitOfMeasure";			
			__state=UNIT_OF_MEASURE;
			return;
		}
		else
		{
			std::cout<<"Error:"<<name<<"\n";
		}
	}
	if(__state==UNIT_OF_MEASURE)
	{
		if(name=="Name")
		{
			__state=NAME;
		}
		else if(name=="QuantityType")
		{
			__state=QUANTITY_TYPE;
		}
		else if(name=="DimensionalClass")
		{
			__state=DIMENSIONAL_CLASS;
		}		
		else if(name=="BaseUnit")
		{
			__state=BASE_UNIT;
		}				
		else
		{
			std::cout<<"UNHANDLED TAG->"<<name<<"\n";
		}
	}
}

void DimensionCache::on_characters(const Glib::ustring& characters)
{
	if(__state==NAME)
	{
		std::cout<<"Name: "<<characters<<"\n";
		__tempDim.name=characters;
	}
	if(__state==QUANTITY_TYPE)
		std::cout<<"Quantity type: "<<characters<<"\n";
	if(__state==DIMENSIONAL_CLASS)
		std::cout<<"DimensionalClass: "<<characters<<"\n";
}

void DimensionCache::on_end_element(const Glib::ustring& name)
{
	if	((__state==UNITS_DEFINITION) && (name=="UnitsDefinition"))__state=ROOT;
	else  if((__state==UNIT_OF_MEASURE ) &&(name=="UnitOfMeasure"))__state=UNITS_DEFINITION;
	else  if((__state==NAME )&&(name=="Name"))__state=UNIT_OF_MEASURE;
	else  if((__state==QUANTITY_TYPE )&&(name=="QuantityType"))__state=UNIT_OF_MEASURE;
	else  if((__state==DIMENSIONAL_CLASS )&&(name=="DimensionalClass"))__state=UNIT_OF_MEASURE;
	else  if((__state==BASE_UNIT )&&(name=="BaseUnit"))__state=UNIT_OF_MEASURE;
	//std::cout<<"END name: "<<name<<" state: "<<__state<<"\n";

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
