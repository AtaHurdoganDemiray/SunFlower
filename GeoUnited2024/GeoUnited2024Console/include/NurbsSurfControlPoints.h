// NurbsSurfControlPoints.h

#ifndef __NurbsSurfControlPoints_h__
#define __NurbsSurfControlPoints_h__

struct VPoint;
template <class T,int N> struct sVector;

class NurbsSurfControlPoints
{
public:
	NurbsSurfControlPoints (const int upointno
							,const int vpointno
							,const std::vector<vreal> &coordinates
							,const std::vector<vreal> &weights
							,bool isRational );
	VPoint GetPoint (const int i , const int j)const;
	sVector<double,4> GetPoint4d (const int i,const int j)const;
	VPoint Blend (const vreal N,const int i,const int j)const;
	sVector<double,4> Blend4d (const vreal N,const int i,const int j)const;
	void UpdateWeighCoord();
private:
	int m_upointno;
	int m_vpointno;
	bool m_isRational;
	const std::vector<vreal> &m_coordinates;
	const std::vector<vreal> &m_weights;
	std::vector<vreal> m_weighcoord;
};

#endif