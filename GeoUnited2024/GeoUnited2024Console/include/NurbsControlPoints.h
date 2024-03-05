// NurbsControlPoints.h

#ifndef __NurbsControlPoints_h__
#define __NurbsControlPoints_h__

struct VPoint;
template <class T,int N> struct sVector;

class NurbsControlPoints
{
public:
	NurbsControlPoints (const int pointno
		, const std::vector<vreal> &coordinates
		, const std::vector<vreal> &weights 
		, bool isRational);
	VPoint Blend (const vreal N , const int i)const;
	sVector<double,4> Blend4d (const vreal N , const int i)const;
	const vreal & GetWeight (const int i)const;
	VPoint GetPoint (const int i)const;
	sVector<double,4> GetPoint4d (const int i)const;
	bool IsRational()const;
private:
	const std::vector<vreal> &m_coordinates;
	const std::vector<vreal> &m_weights;
	std::vector<vreal>m_weighcoord;
	int m_n;
	int m_pointno;
	bool m_isRational;
};

#endif