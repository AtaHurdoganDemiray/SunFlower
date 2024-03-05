//ExceptionRecord.cpp
#include "stdafx.h"
#include "ExceptionRecord.h"

#include<time.h>
#include<stdexcept>
#include<string>




//using VStr;

ExceptionRecord::ExceptionRecord()
{
}
/*	constructor function takes 3 arguments
		exception object's reference (exception&)
		name of file where the exception has occured (VChr*)
		line of file where the exception has occured (int)		*/
ExceptionRecord::ExceptionRecord(const std::exception &exc, const VChr *fName, int line)
{
	setType(typeid(exc).name());
	setMessage(exc.what());
	setFileName(fName);
	setLine(line);
	setCurrentTime();
}

ExceptionRecord::~ExceptionRecord(void)
{ 
}

void ExceptionRecord::setType(const VStr &str)
{
	//setMember(this->type, str, TYPE_STRING_LENGTH);
	m_type = str;
}

void ExceptionRecord::setMessage(const VStr &str)
{
	//setMember(this->message, str, MESSAGE_STRING_LENGTH);
	m_message = str;
}

void ExceptionRecord::setFileName(const VChr *chr)
{
	//strcpy(this->fileName, chr);
	m_fileName = chr;
}

void ExceptionRecord::setLine(const int i)
{
	//VChr *chr = new VChr[10];
	//itoa(i, chr, 10);
	//strcpy(this->line, chr);
	//delete[] chr;
	VOStrStream str;
	str << i;
	m_line = str.str();
}

void ExceptionRecord::setCurrentTime()
{
	time_t t;
	t = time(&t);
	VChr *cT = VTime(&t);

	//setMember(this->currentTime, cT, CURRENT_TIME_STRING_LENGTH);
	m_currentTime = cT;
}

/*	Function for copying unkown sized VStr to the limited size VChr array.
	Takes string, VChr array and an integer that is the length of VChr array and copies string.
	If VStr is longer than array length, it copies first length-1  VChracters of VStr and adds 
	an '\0' after that*/
	
void ExceptionRecord::setMember(VChr *member, const VStr &str, int destArrayLength)
{
	int length = str.size();

	length = ( length < destArrayLength ? length : destArrayLength-1 );
	
	const VChr *swap = str.data();

	strncpy(member, swap, length);	//partial copying
	*(member+length) = '\0';		//add end VChracter
}

bool ExceptionRecord::write(const VStr &destinationFile)
{
	bool success = true;
	try
    {

		VStr recPath = VGlobalFunctions::GetSolidCncExeFolder();//GetCommandLine();
		//VStr recPath = cl.substr (1,cl.rfind ('\\'));
		recPath+=( IDS_VpETxt);

       //VStr recPath =("VpExceptionLog.txt");

		VofStream os(/*destinationFile.data()*/recPath.c_str(), std::ios::app);

		//Could file opened successfully?
 		if (!os)
		{
			os.close();
			throw VException (( IDS_ExcWriExcFilCou),1);
		}

		//os.write(reinterpret_cast< const VChr * >(type), TYPE_STRING_LENGTH);
		//os.write(reinterpret_cast< const VChr * >(message), MESSAGE_STRING_LENGTH);
		//os.write(reinterpret_cast< const VChr * >(fileName), FILE_NAME_STRING_LENGTH);
		//os.write(reinterpret_cast< const VChr * >(line), LINE_LENGTH);
		//os.write(reinterpret_cast< const VChr * >(currentTime), CURRENT_TIME_STRING_LENGTH);		
		//os.close();
		os << m_type.c_str() << ( IDS_N);
		os << m_message.c_str() << ( IDS_N);
		os << m_fileName.c_str() << ( IDS_N);
		os << m_line.c_str() << ( IDS_N);
		os << m_currentTime.c_str() << ( IDS_N);
		os.close();
	}
	catch(...)
	{		
		success = false;
	}	
	return success;
}

//bool ExceptionRecord::read(const VStr &destinationFile)
//{
//	bool success = true;
//	try
//	{
//		VIFStream is(destinationFile.data(), std::ios::in);
//
//		if (!is)
//		{
//			is.close();
//			throw(("File could not be opened"));
//		}
//		while (is && !is.eof())
//		{
//			is.read(this->type, TYPE_STRING_LENGTH);
//			is.read(this->message, MESSAGE_STRING_LENGTH);
//			is.read(this->fileName, FILE_NAME_STRING_LENGTH);
//			is.read(this->line, LINE_LENGTH);
//			is.read(this->currentTime, CURRENT_TIME_STRING_LENGTH);		
//
//			if (is.eof())
//			{
//				break;
//			}
//		}
//	}
//	catch(...)
//	{		
//		success = false;
//	}	
//	return success;		
//}
