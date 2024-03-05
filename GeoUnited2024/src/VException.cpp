//	VException.cpp
#include "stdafx.h"


VException::VException (const VStr &message , const int &messageNumber)
  //:	exception(message.data()),	//member initializer for _m_what variable of base class
    :m_number(messageNumber),	//member initializer for m_number variable
    m_message(message)			//member initializer for m_message variable
{
  
}

VException::~VException(void)
{
}

int VException::GetNumber ()const
{
	return m_number;
}

VStr VException::GetMyMessage ()
{
	return m_message;
}







