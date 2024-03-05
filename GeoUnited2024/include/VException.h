//	Class VException definition.
#ifndef __VException_h__
#define __VException_h__


//VException class is extended from standart exception class
class VException //: public std::exception
{
public:
	VException (const VStr &message , const int &messageNumber);
    virtual ~VException(void);

	int GetNumber ()const;
	VStr GetMyMessage();
private:
	VStr m_message;
	int m_number;
};
//	end class VException

#endif
