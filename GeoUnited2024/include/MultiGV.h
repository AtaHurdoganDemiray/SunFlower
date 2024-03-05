// MultiGV.h

#ifndef __MultiGV_h__
#define __MultiGV_h__
#include "VPoint.h"
#include "Bifstream.h"
#include "Bofstream.h"

struct VTriangleE;
class Triangles;
class Points;
struct VLine;
class Lines;

template <class G,class V> class MultiGV
{
public:
	MultiGV (const std::vector<const V*> &geoVecPointerVec);
	MultiGV (int totalSize , const std::vector<const V*> &geoVecPointerVec);
	MultiGV (const MultiGV<G,V> &toCopy);
	MultiGV & operator = (const MultiGV<G,V> &toCopy);
	int Size()const;
	const G operator [] (int i)const;
	const VPoint GetMin()const;
	const VPoint GetMax()const;
	std::vector<const V*> & GetGeoVecPointerVec();
	void Serialize (Bifstream &infile);
	void Serialize (Bofstream &outfile)const;
private:
	std::vector <const V*> m_geoVecPointerVec;
	int m_totalSize;
};

#include "MultiGV.inl"

#endif