// NurbsBlendFunc.h

#ifndef __NurbsBlendFunc_h__
#define __NurbsBlendFunc_h__

#define USENN 1

// This class is based on "Comparison of Surface and Derivative Evaluation Methods for
// the Rendering of NURB Surfaces" by WILLIAM L. LUKEN  IBM T.J. Watson Research Center
// and FUHUA (FRANK) CHENG University of Kentucky
// The document can be found in ../../doc 

class BSplineKnots;

class NurbsBlendFunc
{
public:
	NurbsBlendFunc (const BSplineKnots &t);
public:
	void CalcN (const vreal u , int &m);//from N[m][k] to N[m-k+1][k] are calculated , m is the span no for u
	std::vector<std::vector<vreal>> m_N;
	std::vector<std::vector<vreal>> ca;
private:
	const BSplineKnots &m_t;
	int m_k,m_n;
	
};

#endif