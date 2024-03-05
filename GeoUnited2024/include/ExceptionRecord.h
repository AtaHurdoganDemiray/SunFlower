//	Class ExceptionRecord definition.

//	Class used for holding and processing exception properties
#pragma once

#include<stdexcept>

//Variable arrays' lengths
#define TYPE_STRING_LENGTH 15
#define MESSAGE_STRING_LENGTH 40
#define FILE_NAME_STRING_LENGTH 20
#define LINE_LENGTH 7
#define CURRENT_TIME_STRING_LENGTH 30

class ExceptionRecord
{
public:
	ExceptionRecord();
	ExceptionRecord(const std::exception&, const VChr*, int);
	~ExceptionRecord(void);
	bool write(const VStr& fName);
	//bool read(const VStr& fName);

private:
	//METHODS
	void setType(const VStr&);
	void setMessage(const VStr&);
	void setFileName(const VChr*);
	void setLine(int);
	void setCurrentTime();
	void setMember(VChr*, const VStr&, int);
	//VARIABLES
	//VChr type[TYPE_STRING_LENGTH];					//Type of exception
	//VChr message[MESSAGE_STRING_LENGTH];			//Exception message
	//VChr fileName[FILE_NAME_STRING_LENGTH];			//In which file exception has occured
	//VChr line[LINE_LENGTH];							//In which line exception occured
	//VChr currentTime[CURRENT_TIME_STRING_LENGTH];	//Time of exception
	VStr m_type;
	VStr m_message;
	VStr m_fileName;
	VStr m_line;
	VStr m_currentTime;
};
// end class ExceptionRecord definition