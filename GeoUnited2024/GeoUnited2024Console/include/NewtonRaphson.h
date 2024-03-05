// NewtonRaphson.h

#ifndef __NewtonRaphson_h__
#define __NewtonRaphson_h__
class Function;
class NewtonRaphson
{
public:
	
	// return value is true if method can find a root , otherwise false
	bool GetRoot (double x0 , double errortol , int maxiter , Function &f , double &root);
	
	double GetNextX (double x0 , Function &f , double &fx , double &fdx);

};

#endif