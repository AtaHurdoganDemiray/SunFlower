// LogFileWriter.h

#ifndef __LogFileWriter_h__
#define __LogFileWriter_h__

#include "CmnBase.h"

class VException;

class LogFileWriter
{
public:
	LogFileWriter ();
	static void InitFileName ();
	static void WriteExceptionLog (VException &e , VStr catcherClassName = "");
	static void WriteExceptionLog (std::exception &e , VStr catcherClassName = "");
	static void WriteUnknownExceptionLog (const VStr toWrite , VStr catcherClassName = "");
	static void WriteMessage (const VStr &message,bool logTime=true);
	static void WriteStrMap (const std::map<VStr,VStr> &epm , VofStream &f);
private:
	static void LogTime (VofStream &f);
	static VStr m_lastLog;
	static CmnBase *m_cmn;
public:
	static VStr GetTimeStampAsYearMonthDayHourMin();
	static VStr GetFileName();
	static VStr m_fileName;
	static VStr m_changeFileName;
	static void PrintMessageToGraphicWnd(const VStr &message);
	static void SetCmn(CmnBase *cmn);
};

#endif