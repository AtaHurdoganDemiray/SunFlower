// QuadraticEquation.h

#ifndef __QuadraticEquation_h__
#define __QuadraticEquation_h__

class QuadraticEquation
{
public:
	QuadraticEquation (const vreal &a , const vreal &b , const vreal &c);
	int GetRoots (std::vector<vreal> &roots)const;
	vreal Evaluate (const vreal &x)const;
private:
	vreal m_a,m_b,m_c;
};

#endif
