// Function.h

#ifndef __Function_h__
#define __Function_h__

class Function
{
public:
	virtual double GetY(double x) = 0;
	virtual double GetDerv (double x) = 0;
};

#endif