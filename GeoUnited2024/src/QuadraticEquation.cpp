// QuadraticEquation.cpp

#include "stdafx.h"
#include "QuadraticEquation.h"

QuadraticEquation::QuadraticEquation(const vreal &a, const vreal &b, const vreal &c)
:m_a(a),m_b(b),m_c(c)
{

}
int QuadraticEquation::GetRoots(std::vector<vreal> &roots) const
{
	vreal disc = m_b * m_b - 4.0 * m_a * m_c;
	if (disc < 0.0)
		return 0;
	
	if (fabs(disc) < EPSILON)
	{
		vreal x = -m_b / (2.0 * m_a);
		roots.push_back (x);
		return 1;
	}

	int sgnb = 1;
	if (m_b < 0.0)
		sgnb = -1;
	vreal q = -0.5 * (m_b + sgnb * sqrt (disc));
	vreal x1 = q / m_a;
	vreal x2 = m_c / q;
	roots.push_back (x1);
	roots.push_back (x2);
	return 2;
}
vreal QuadraticEquation::Evaluate(const vreal &x) const
{
	return m_a * x * x + m_b * x + m_c;
}

