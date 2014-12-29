#include "Dimension.h"
#include <cstring>
#include <cassert>
#include <iomanip>

std::map<std::string,DimensionDef> DimensionCache::__cache;

int is_empty(const char *s) {
  while (*s != '\0') {
    if (!isspace(*s))
      return 0;
    s++;
  }
  return 1;
}

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
			__state=UNITS_DEFINITION;
		}
	}
	else if(__state==UNITS_DEFINITION)
	{
		__tempDim=DimensionDef();
		for(AttributeList::const_iterator i=properties.begin();i!=properties.end();i++)
		{
			if(i->name=="id")
			{
				__tempDim.id=i->value;
				//std::cout<<"\033[33m"<<__tempDim.id<<"\033[0m\n";
			}
			else if(i->name=="annotation")
			{
				__tempDim.annotation=i->value;
			}
		}
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
	else if(__state==UNIT_OF_MEASURE)
	{
		if(name=="Name")
		{
			__state=NAME;
		}
		else if(name=="DimensionalClass")
		{
			__state=DIMENSIONAL_CLASS;
		}		
		else if(name=="BaseUnit")
		{
			__tempDim.isBaseUnit=true;
			__tempDim.baseUnit=__tempDim.annotation;
			__state=BASE_UNIT;
		}
		else if(name=="ConversionToBaseUnit")		
		{
			__tempDim.isBaseUnit=false;
			__state=CONVERSION_TO_BASE_UNIT;
			for(AttributeList::const_iterator i=properties.begin();i!=properties.end();i++)
			{
				if(i->name=="baseUnit")
				{
					__tempDim.baseUnit=i->value;
				}
			}
		}
		else
		{
			//std::cout<<"UNHANDLED TAG->"<<__LINE__<<name<<"\n";
		}
	}
	else if(__state==CONVERSION_TO_BASE_UNIT)
	{
		if(name=="Factor")
		{
			__state=FACTOR;
		}
		else if(name=="Fraction")
		{	
			__state=FRACTION;
		}
		else
		{
			__tempDim.__isValid=false;
	//		std::cout<<"UNHANDLED TAG->"<<__LINE__<<name<<"\n";
		}
	}
	else if(__state==FRACTION)
	{
		if(name=="Numerator")
		{
			__state=NUMERATOR;
		}
		else if (name=="Denominator")
		{
			__state=DENOMINATOR;
		}
		else
		{
			std::cout<<"UNHANDLED TAG->"<<__LINE__<<name<<"\n";
		}
	}
}

//#define TAB(NAME,VALUE) "\033[34;1m"<<std::setw(12)<<#NAME<<"\033[0m: \033[36m|"<<VALUE<<"|\033[0m\n"
#define TAB(NAME,VALUE) std::setw(12)<<#NAME<<": "<<VALUE<<"\n"

std::ostream& operator<<(std::ostream& stream, const DimensionDef &d)
{
	const char* DNAMES="MLTKNCASBU";
	stream<<TAB(id,d.id);
	stream<<TAB(name,d.name);
	stream<<TAB(annotation,d.annotation);
	stream<<std::setw(12)<<"Power"<<": ";
	for(int i=0;i<NDIMS;i++)
	{	
		if(d.power[i]!=0)
		stream<<DNAMES[i]<<"^("<<d.power[i]<<") ";
	}
	stream<<"\n";
	if(d.isBaseUnit)stream<<std::setw(12)<<"BASE UNIT"<<"\n";
	else    	stream<<TAB(Base Unit,d.baseUnit);
	stream<<TAB(factor,d.factor);
	return stream;
}

void DimensionCache::on_characters(const Glib::ustring& characters)
{
	if(is_empty(characters.c_str()))return;
	if(__state==NAME)
	{
		__tempDim.name=characters;
	}
	else if(__state==DIMENSIONAL_CLASS)
	{
		const char*c=characters.c_str();
		int lastIndex=-1;
		bool prevNum=true;
		int mul=1;
		for(int i=0;i<characters.size();i++)
		{
			if(isalpha(c[i]))
			{
				prevNum=false;
				if(c[i]=='M') 		lastIndex=MASS;
				else if(c[i]=='L')	lastIndex=LENGTH;
				else if(c[i]=='T')	lastIndex=TIME;
				else if(c[i]=='K')	lastIndex=TEMPERATURE;
				else if(c[i]=='N')	lastIndex=MOLE;
				else if(c[i]=='C')	lastIndex=CHARGE;
				else if(c[i]=='A')	lastIndex=ANGLE;
				else if(c[i]=='S')	lastIndex=SOLID_ANGLE;
				else if(c[i]=='B')	lastIndex=LUMINOUS_INTENSITY;
				else if(c[i]=='U')	lastIndex=GAPI;
				else
				{
					assert(false);
				}
				__tempDim.power[lastIndex]=1*mul;
			}
			else if(isdigit(c[i]))
			{
				if(lastIndex<0){assert(c[i]=='1');}
				else
				{
					assert(!prevNum);
					prevNum=true;
					int I=c[i]-'0';
					__tempDim.power[lastIndex]=I*mul;
				}
			}
			else if(c[i]=='/')
			{
				mul=-1;
			}
			else
			{
				std::cout<<"->"<<c[i]<<"<-\n";
				assert(false);
			}
		}
	}
	else if(__state==CONVERSION_TO_BASE_UNIT)
	{
		__tempDim.__isValid=false;
	//	std::cout<<"BAD Text: "<<characters<<__LINE__<<"\n";
	}
	else if(__state==FACTOR)
	{
		__tempDim.factor=atof(characters.c_str());
	}
	else if(__state==FRACTION)
	{
		std::cout<<"BAD Text: "<<characters<<__LINE__<<"\n";
	}
	else if(__state==NUMERATOR)
	{
		__tempDim.factor*=atof(characters.c_str());
	}
	else if(__state==DENOMINATOR)
	{
		__tempDim.factor/=atof(characters.c_str());
	}


}

void DimensionCache::on_end_element(const Glib::ustring& name)
{
	if	((__state==UNITS_DEFINITION) && (name=="UnitsDefinition"))__state=ROOT;
	else  if((__state==UNIT_OF_MEASURE ) &&(name=="UnitOfMeasure"))
	{
		assert(__cache.find(__tempDim.annotation) == __cache.end());
		__cache[__tempDim.annotation]=__tempDim;
		__state=UNITS_DEFINITION;
		//std::cout<<__tempDim;
	}
	else  if((__state==NAME )&&(name=="Name"))__state=UNIT_OF_MEASURE;
	else  if((__state==DIMENSIONAL_CLASS )&&(name=="DimensionalClass"))__state=UNIT_OF_MEASURE;
	else  if((__state==BASE_UNIT )&&(name=="BaseUnit"))__state=UNIT_OF_MEASURE;
	else  if((__state==CONVERSION_TO_BASE_UNIT )&&(name=="ConversionToBaseUnit"))__state=UNIT_OF_MEASURE;
	else  if((__state==FACTOR )&&(name=="Factor"))__state=CONVERSION_TO_BASE_UNIT;
	else  if((__state==FRACTION )&&(name=="Fraction"))__state=CONVERSION_TO_BASE_UNIT;
	else  if((__state==NUMERATOR )&&(name=="Numerator"))__state=FRACTION;
	else  if((__state==DENOMINATOR )&&(name=="Denominator"))__state=FRACTION;
	//else  std::cout<<"END name: "<<name<<" state: "<<__state<<"\n";

}





















Dimension::Dimension():
	__factor(1.0)
{
	for(int i=0;i<NDIMS;i++)__power[i]=0.0;
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
	for(int i=0;i<NDIMS;i++){__power[i]+=dim.__power[i];}
	__factor*=dim.__factor;
	return *this;
}

const Dimension& Dimension::operator/=(const Dimension& dim)
{
	for(int i=0;i<NDIMS;i++){__power[i]-=dim.__power[i];}
	__factor/=dim.__factor;
	return *this;
}
const Dimension& Dimension::operator^=(const Number& num)
{
}

Dimension::~Dimension()
{

}

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
