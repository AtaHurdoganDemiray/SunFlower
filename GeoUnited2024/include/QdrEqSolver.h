// QdrEqSolver.h

#ifndef __QdrEqSolver_h__
#define __QdrEqSolver_h__

class VInterval;

// a x^2 + b x + c = QdrEq

template <class T> class QdrEqSolver
{
public:
	// checks if qdr. eq. is positive for all x , where x E [0,1]
	// a should be NON ZERO
	static bool IsPositiveIn01 (const T a
								,const T b
								,const T c);

	// returns the number of roots(0,1 or 2) and records them in roots[2]
	// a should be NON ZERO
	static int FindRoots (const T a
						,const T b
						,const T c
						, T roots[2]);
	
	static std::vector<VInterval> GetPositive (const T a , const T b , const T c);
	static std::vector<VInterval> GetNegative (const T a , const T b , const T c);
	// for infinite MAX_NUMBER_LIMIT and MIN_NUMBER_LIMIT values are used
};

#include "QdrEqSolver.inl"

#endif