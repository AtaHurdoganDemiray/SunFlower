// LogFileWriter.cpp

#include "stdafx.h"
#include "CmnBase.h"

VStr LogFileWriter::m_fileName = ("");
VStr LogFileWriter::m_lastLog = ("");
CmnBase * LogFileWriter::m_cmn = NULL;// CmnBase();
VStr LogFileWriter::m_changeFileName = "";
LogFileWriter::LogFileWriter()
{
	
}

void LogFileWriter::InitFileName ()
{

    VStr appPath = VGlobalFunctions::GetSolidCncExeFolder();//GetCommandLine ();
	//appPath = appPath.substr(1,appPath.rfind('\\'));
	appPath += "MessageLog\\";//( IDS_MesVieHtm);
	VStr timeStamp = GetTimeStampAsYearMonthDayHourMin();
	appPath += timeStamp;
	m_changeFileName = appPath;
	appPath += ".txt";
	m_fileName = appPath;
	m_changeFileName += "State.txt";

    //m_fileName = "LogFileWriter.cpp";

}

void LogFileWriter::WriteExceptionLog (VException &e , VStr catcherClassName)
{	
	VStr tmp = e.GetMyMessage() + " " + catcherClassName;
	if (m_lastLog == tmp)
		return;
	m_lastLog = tmp;
	if (m_fileName == (""))
		InitFileName();
	VofStream f (m_fileName.c_str() , std::ios::app);
	VofStream fstate (m_changeFileName.c_str(),std::ios::trunc);
	//f << ( IDS_PN);
	f << e.GetMyMessage().c_str() << " " << catcherClassName <<( IDS_N);
	//f << ( IDS_BrN);
	LogTime(f);
	fstate << '1';
	//VStr tmp2(e.what());
	//tmp2 += " ";
	//tmp2 += catcherClassName;
	PrintMessageToGraphicWnd(tmp);//(tmp2);
}
void LogFileWriter::WriteExceptionLog (std::exception &e , VStr catcherClassName)
{
	VStr tmp (e.what()+catcherClassName);
	if (m_lastLog == tmp)
		return;
	m_lastLog = tmp;
	if (m_fileName == (""))
		InitFileName();
	VofStream f (m_fileName.c_str() , std::ios::app);
	VofStream fstate (m_changeFileName.c_str(),std::ios::trunc);
	//f << ( IDS_PN);
	f << e.what() << " " << catcherClassName <<  ( IDS_N);
	//f << ( IDS_BrN);
	LogTime(f);
	fstate << '1';
	//f<< ( IDS_PN1);
	//f << ( IDS_HrN);

	VStr tmp2 (e.what());
	tmp2 += " ";
	tmp2 += catcherClassName;
	PrintMessageToGraphicWnd(tmp2);
}
void LogFileWriter::WriteUnknownExceptionLog (const VStr toWrite , VStr catcherClassName)
{
	VStr tmp = toWrite + catcherClassName;
	if (m_lastLog == tmp)
		return;
	m_lastLog = tmp;
	if (m_fileName == (""))
		InitFileName();
	VofStream f (m_fileName.c_str() , std::ios::app);
	VofStream fstate (m_changeFileName.c_str(),std::ios::trunc);
	//f << ( IDS_PN);
	f << toWrite.c_str() << " " << catcherClassName << ( IDS_N);
	//f << ( IDS_BrN);
	LogTime(f);
	fstate << '1';

	VStr tmp2(toWrite);
	tmp2 += " ";
	tmp2 += catcherClassName;
	PrintMessageToGraphicWnd(tmp2);
}
void LogFileWriter::WriteMessage (const VStr &message,bool logTime)
{
	if (m_fileName == (""))
		InitFileName();
	VofStream f (m_fileName.c_str() , std::ios::app);
	VofStream fstate (m_changeFileName.c_str(),std::ios::trunc);
	//f << ( IDS_PN);
	f << message.c_str() << ( IDS_N);
	if (logTime)
	{
		//f << ( IDS_BrN);
		LogTime(f);
	}
	fstate << '1';
	//f<< ( IDS_PN1);
	//f << ( IDS_HrN);
}

void LogFileWriter::LogTime(VofStream &f)
{
    VOStrStream mystr;

	ATL::VTime mytime = VTime::GetCurrentTime();
    mystr << mytime.GetDay() << ("/") << mytime.GetMonth() << ("/") << mytime.GetYear() << ("   ") << mytime.GetHour() << (":") << mytime.GetMinute()<<( IDS_N);

    //f << ( IDS_PN);
	f << mystr.str().c_str() << ( IDS_N);
	//f << ( IDS_PN1);
}

void LogFileWriter::WriteStrMap (const std::map<VStr,VStr> &epm , VofStream &f)
{
	std::map<VStr,VStr>::const_iterator sii = epm.begin();
	std::map<VStr,VStr>::const_iterator see = epm.end();
	VStr tomsg;
	for (sii;sii!=see;++sii)
	{
		tomsg+=sii->first;
		tomsg+=" : ";
		tomsg+=sii->second;
		tomsg+= IDS_N;
	}
	f << tomsg;
}

VStr LogFileWriter::GetTimeStampAsYearMonthDayHourMin ()
{
	VOStrStream mystr;
	ATL::VTime mytime = VTime::GetCurrentTime();
    mystr << mytime.GetYear() << ("_")<< mytime.GetMonth()<< ("_") << mytime.GetDay() << ("_") << mytime.GetHour() << ("_") << mytime.GetMinute();;
	VStr tmp (mystr.str());
	return tmp;
}

VStr LogFileWriter::GetFileName()
{
	return m_fileName;
}

void LogFileWriter::PrintMessageToGraphicWnd(const VStr &message)
{
	if (m_cmn != NULL)
	{
		std::vector<VStr> strvec;
		strvec.push_back((IDS_Command));
		strvec.push_back((IDS_MessageToStatus));
		strvec.push_back((IDS_Message));
		strvec.push_back(message);
		m_cmn->SendMessageToVp(strvec);
	}
}

void LogFileWriter::SetCmn(CmnBase *cmn)
{
	m_cmn = cmn;
}
