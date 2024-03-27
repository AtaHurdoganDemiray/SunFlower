// QdrEqSolver.inl

#ifndef __QdrEqSolver_inl__
#define __QdrEqSolver_inl__

// Qdr. Eq. = a x^2 + b x + c
// a should be NON ZERO

template <class T>
bool QdrEqSolver<T>::IsPositiveIn01 (const T a
									,const T b
									,const T c)
{
	if (c <= 0) // eval at x=0
		return false;
	if (a+b+c <= 0)// eval at x = 1.
		return false;

	T peak = -b / (2*a);
	if (peak >= 0 && peak <= 1)
	{
		T peakEval = c - b*b/(4*a);
		if (peakEval > 0)
			return true; // extr. point is positive
		else
			return false; // extr. point is negative
	}
	else
	{
		// evals are positive on x = 0 and x = 1 
		// and no extremum point in x E [0,1]
		return true;
	}
}

template <class T>
int QdrEqSolver<T>::FindRoots(const T a
							  , const T b
							  , const T c
							  , T roots[])
{
	if (fabs (a) < EPSILON)
	{// lineer eq
		if (fabs(b) <EPSILON)
			return 0;
		else
		{
			roots[0] = -c / b;
			return 1;
		}
	}

	vreal disc = b*b - 4*a*c;
	if (disc < 0)
		return 0;

	if (disc < EPSILON)
	{
		roots[0] = -b / (2 * a);
		roots[1] = roots[0];
		return 1;
	}
	else
	{
		roots[0] = (-b - sqrt(disc) ) / ( 2*a);
		roots[1] = (-b + sqrt(disc) ) / (2*a);
		return 2;
	}
}

template <class T>
std::vector<VInterval> QdrEqSolver<T>::GetPositive (const T a , const T b , const T c)
{
	T roots[2];
	int rtno = FindRoots (a,b,c,roots);
	std::vector<VInterval> res;
	if (rtno == 2)
	{
		T rm = (roots[0] + roots[1] ) * 0.5;
		T eval1 = a * rm * rm + b * rm + c;
		if (eval1 > 0.0)
		{
			VInterval res1 ((double)roots[0] , (double)roots[1]);
			res.push_back (res1);
			return res;
		}
		else
		{
			VInterval res1 (MIN_NUMBER_LIMIT , (double)roots[0]);
			VInterval res2 ((double)roots[1] , MAX_NUMBER_LIMIT);
			res.push_back (res1);
			res.push_back (res2);
			return res;
		}
	}
	else if (rtno == 0)
	{
		// eval for x=0
		if (c > 0)
		{
			VInterval res1 (MIN_NUMBER_LIMIT , MAX_NUMBER_LIMIT);
			res.push_back (res1);
			return res;
		}
	}
	else if (rtno == 1)
	{
		// eval for x = root + 1
		T x = roots[0] + 1;
		T eval3 = a * x*x + b * x + c;
		if (eval3 > 0)
		{
			VInterval res1 (MIN_NUMBER_LIMIT , MAX_NUMBER_LIMIT);
			res.push_back (res1);
			return res;
		}
	}
	return res;
}

template <class T>
std::vector<VInterval> QdrEqSolver<T>::GetNegative (const T a , const T b , const T c)
{
	T roots[2];
	int rtno = FindRoots (a,b,c,roots);
	std::vector<VInterval> res;
	if (rtno == 2)
	{
		T rm = (roots[0] + roots[1] ) * 0.5;
		T eval1 = a * rm * rm + b * rm + c;
		if (eval1 < 0.0)
		{
			VInterval res1 ((double)roots[0] , (double)roots[1]);
			res.push_back (res1);
			return res;
		}
		else
		{
			VInterval res1 (MIN_NUMBER_LIMIT , (double)roots[0]);
			VInterval res2 ((double)roots[1] , MAX_NUMBER_LIMIT);
			res.push_back (res1);
			res.push_back (res2);
			return res;
		}
	}
	else if (rtno == 0)
	{
		// eval for x=0
		if (c < 0)
		{
			VInterval res1 (MIN_NUMBER_LIMIT , MAX_NUMBER_LIMIT);
			res.push_back (res1);
			return res;
		}
	}
	else if (rtno == 1)
	{
		// eval for x = root + 1
		T x = roots[0] + 1;
		T eval3 = a * x*x + b * x + c;
		if (eval3 < 0)
		{
			VInterval res1 (MIN_NUMBER_LIMIT , MAX_NUMBER_LIMIT);
			res.push_back (res1);
			return res;
		}
	}
	return res;
}
#endif