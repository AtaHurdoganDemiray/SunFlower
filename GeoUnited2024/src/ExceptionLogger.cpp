//	ExceptionLogger.cpp
#include "stdafx.h"
#include "ExceptionLogger.h"
#include "ExceptionRecord.h"
#include <stdexcept>



ExceptionLogger::ExceptionLogger(void)
{
}

ExceptionLogger::~ExceptionLogger(void)
{
}
/*
bool ExceptionLogger::log(ExceptionRecord& exc)
{
	return true;
}*/
bool ExceptionLogger::log (VException &e , const VChr *fName , int lineNumber)
{
	LogFileWriter::WriteExceptionLog (e);
	return true;
}
bool ExceptionLogger::log(std::exception &e, const VChr *fName, int lineNumber)
{
	//ExceptionRecord record(exc, fName, lineNumber);
	
	//record.write(DEFAULT_DESTINATION_FILE);
	LogFileWriter::WriteExceptionLog (e);
	return true;
}

