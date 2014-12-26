#ifndef LANGUAGE_STRUCTURES_H_INCLUDED
#define LANGUAGE_STRUCTURES_H_INCLUDED

#include <memory>
#include <map>
#include <string>


class LanguageStructure
{
public:

	virtual double valueSI()const=0;
	virtual ~LanguageStructure()
	{}
};
typedef LanguageStructure* LanguageStructurePtr;

#define YYSTYPE LanguageStructurePtr


#endif
