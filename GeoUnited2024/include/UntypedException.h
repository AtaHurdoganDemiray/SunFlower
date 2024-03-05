/*	Class UntypedException definition
	
	Exceptions that are neither standart exception 
	nor	VException(developer defined exception type).

*/
#pragma once


class UntypedException :public VException
{
public:
	UntypedException(const VStr& message, int m_number);
	~UntypedException(void);
};
// end class UntypedException