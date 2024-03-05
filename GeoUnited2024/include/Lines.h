// Lines.h

#ifndef __Lines_h__
#define __Lines_h__

struct VLine;
struct VPoint;

//interface class for lines vector
class Lines 
{
public:
	Lines();
	Lines (const Lines &toCopy);
	Lines & operator = (const Lines &toCopy);
	virtual ~Lines();
	virtual const VLine operator [] (int i)const =0;
	virtual VLine operator [] (int i)=0;
	virtual int Size()const = 0;
	virtual const VPoint GetMin()const=0;
	virtual const VPoint GetMax()const=0;
	unsigned int GetPointer ()const;
};

#endif