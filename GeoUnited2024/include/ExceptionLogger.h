//	Class ExceptionLogger definition.
#pragma once
#include <stdexcept>
#include "ExceptionRecord.h"

#define DEFAULT_DESTINATION_FILE "exceptionLog.dat"	//default file name
class VException;
class ExceptionLogger
{
public:
	ExceptionLogger(void);
	~ExceptionLogger(void);	
		//	static write function takes exception objects and writes it to default file
	bool static log(std::exception& exc, const VChr *fName, int lineNumber);
	static bool log (VException &e , const VChr *fName , int lineNumber);
};
// end class ExceptionLogger