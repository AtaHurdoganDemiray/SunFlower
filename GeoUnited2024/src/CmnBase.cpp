// CmnBase.cpp

#include "stdafx.h"
#include "CmnBase.h"
#include "TypeConverters.h"

const UINT CmnBase::InterAppMsg = ::RegisterWindowMessage(( IDS_ViewPointInterA));
//HWND CmnBase::m_activeDllHWND = 0;
//HWND CmnBase::m_viewPointHWND = 0;

//CmnBase::CmnBase ()
//{
//	//m_uniqMsg = InterAppMsg;	
//}

//CmnBase::CmnBase(UINT uniqmsg , HWND recieverHWnd)
//{
//	m_activeDllHWND = recieverHWnd;
//	//m_uniqMsg = InterAppMsg;
//}

int CmnBase::SendString(const TCHAR *str)
{
	static const UINT halfShift =  sizeof(UINT)*4;		//16 for 32bit

	if(!m_activeDllHWND)
	{
		return CMN_NOTOK;
	}

	int len, res;

	if(str == 0)
	{
		return CMN_NOTOK;
	}

	len = strlen(str);

	if(len <= 0)
		return CMN_NOTOK;

	//initiate send string
	res = ::SendMessage((HWND)m_activeDllHWND, /*m_uniqMsg*/InterAppMsg, CMN_SENDSTR_INITIALIZE, len);
	if(res != CMN_OK)
		return res;

	//send VChrs
	for(int i = 0; i < len; i++)
	{
		res = ::SendMessage((HWND)m_activeDllHWND, /*m_uniqMsg*/InterAppMsg, CMN_SENDSTR_SENDING, ((UINT)i << halfShift) | (UINT)(str[i]));
		if(res != CMN_OK)
			return res;
	}

	//finalize send string
	res = ::SendMessage((HWND)m_activeDllHWND, /*m_uniqMsg*/InterAppMsg, CMN_SENDSTR_FINALIZE, len);
	if(res != CMN_OK)
		return res;

	return CMN_OK;
}

// For Can to implement
void CmnBase::SendStringByASIO(const char* str)
{
	// to be implemented...

}

int CmnBase::SendStringToVp(const TCHAR *str)
{
	static const UINT halfShift =  sizeof(UINT)*4;		//16 for 32bit

	if(!m_viewPointHWND)
	{
		return CMN_NOTOK;
	}

	int len, res;

	if(str == 0)
	{
		return CMN_NOTOK;
	}

	len = strlen(str);

	if(len <= 0)
		return CMN_NOTOK;

	//initiate send string
	res = ::SendMessage((HWND)m_viewPointHWND, /*m_uniqMsg*/InterAppMsg, CMN_SENDSTR_INITIALIZE, len);
	if(res != CMN_OK)
		return res;

	//send VChrs
	for(int i = 0; i < len; i++)
	{
		res = ::SendMessage((HWND)m_viewPointHWND, /*m_uniqMsg*/InterAppMsg, CMN_SENDSTR_SENDING, ((UINT)i << halfShift) | (UINT)(str[i]));
		if(res != CMN_OK)
			return res;
	}

	//finalize send string
	res = ::SendMessage((HWND)m_viewPointHWND, /*m_uniqMsg*/InterAppMsg, CMN_SENDSTR_FINALIZE, len);
	if(res != CMN_OK)
		return res;

	return CMN_OK;
}
CmnBase::CmnBase (VHWND recieverHWND)
{
	m_activeDllHWND = recieverHWND;
	//m_uniqMsg = InterAppMsg;
}

int CmnBase::SendMessageToVp (const std::vector<VStr> &messageVec)
{
	try
	{
		size_t mysize = messageVec.size();
		VStr mergedVec;
		TypeConverters::MergeString (messageVec , COMMA_CHR , mergedVec);	
		//HWND tmp = m_activeDllHWND;
		//m_activeDllHWND = m_viewPointHWND;
		this->SendStringToVp (mergedVec.c_str());
		//m_activeDllHWND = tmp;
		int res = ::SendMessage((HWND)m_viewPointHWND,/*m_uniqMsg*/InterAppMsg, CMN_COMMANDFROMVIEWPOINT, 0);
		return res;
	}
	catch (...)
	{
		//m_activeDllHWND = tmp;
		return CMN_NOTOK;
	}
}

VHWND CmnBase::GetViewPointHWnd ()
{
	return m_viewPointHWND;
}

void CmnBase::SetViewPointHWnd(VHWND viewPointHWnd)
{
	m_viewPointHWND = viewPointHWnd;
}

int CmnBase::SendKeyValuePairs(const std::map<VStr,VStr> &keyValueMap)
{
	VStr msg;
	std::map<VStr,VStr>::const_iterator i = keyValueMap.begin();
	std::map<VStr,VStr>::const_iterator e = keyValueMap.end();
	for (i;i!=e;++i)
	{
		msg+=i->first;
		msg+=',';
		msg+=i->second;
		msg+=',';
	}
	int ck = SendString (msg.c_str());
	return ck;
}

int CmnBase::SendKeyValuePairsAndInvokeApplicationRegisteredCallbackFunction (const std::map<VStr,VStr> &keyValueMap)
{
	int ck1 = SendKeyValuePairs (keyValueMap);
	int ck2 = ::SendMessage((HWND)m_activeDllHWND , InterAppMsg , CMN_CALLBACK_SOLIDCNCKEYVALPAIR , 0);
	return ck1;
}