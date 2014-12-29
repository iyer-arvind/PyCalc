#ifndef MACROS_H_INCLUDED
#define MACROS_H_INCLUDED
#include <iostream>
#include <iomanip>

#define STREAM_WRITER std::ostream& toStream(std::ostream& stream)const;
#define STREAM_OPERATOR(T) inline std::ostream& operator<<(std::ostream& stream,const T &obj){return obj.toStream(stream);}

#define LOG(MSG) {const char *f=__FILE__;std::cerr<<"\033[31;2m"<<std::setw(25)<<(f[0]=='/'?f+26:f)<<" \033[37;1m["<<__LINE__<<"]\033[0m:"<<MSG<<"\n";}
#define SELF(CLASS) LOG(#CLASS<<*this);

#endif
