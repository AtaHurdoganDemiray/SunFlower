// NewtonRaphson.cpp

#include "stdafx.h"
#include "NewtonRaphson.h"

#include "Function.h"

#include "UntypedException.h"
#include "ExceptionLogger.h"
#include <stdexcept>
// x1 = x0 - f(x0) / f'(x0)

bool NewtonRaphson::GetRoot(double x0, double errortol , int maxiter , Function &f, double &root)
{
	try
	{
		int counter = 0;
		double xp = x0;
		double fx,fdx;
		double prevfx = MAX_NUMBER_LIMIT;
		while (counter < maxiter)
		{
			root = GetNextX(xp,f,fx,fdx);
			if (fabs(fx) < errortol)
			{
				return true;
			}
			else if (xp == root)
				return false;
			else
				xp = root;
			++counter;
		}
		return false;
	}
	catch (std::exception &exc)
	{
		ExceptionLogger::log(exc, __FILE__, __LINE__);
		return false;
    }

	catch (CMemoryException* e)
	{
		throw VException (( IDS_MemExcInNewGet),1);
		return false;
	}
	catch (CException* e)
	{
		throw VException (( IDS_CExInNewGet),1);
		return false;
	}

	catch(...)
	{
		UntypedException uexc(( IDS_UnkXceInNewGet),999);
		ExceptionLogger::log(uexc, __FILE__, __LINE__);
		return false;
	}
}

double NewtonRaphson::GetNextX(double x0, Function &f , double &fx , double &fdx)
{
	fx = f.GetY(x0);
	fdx = f.GetDerv(x0);
	if (fabs(fdx)<EPSILON)
		throw VException (( IDS_NewGetExcDerIs), 1);
	double x1 = x0 - fx / fdx;
	double fx1 = f.GetY(x1);
	if (fabs(fx1) < fabs(fx))
	{	
		fx = fx1; 
		return x1;
	}
	else 
		return x0;
}
