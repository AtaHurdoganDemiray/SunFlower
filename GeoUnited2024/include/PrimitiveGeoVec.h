// PrimitiveGeoVec.h

#ifndef __PrimitiveGeoVec_h__
#define __PrimitiveGeoVec_h__
#include "Bifstream.h"
#include "Bofstream.h"
#include "VPoint2d.h"
#include "AABB.h"
class TransformationMatrix;

template <class T,int N> class PrimitiveGeoVec
{
public:
	PrimitiveGeoVec();
	PrimitiveGeoVec(int initialcapacity);
	PrimitiveGeoVec(const std::vector<T> &std_vec);
	PrimitiveGeoVec(const PrimitiveGeoVec<T,N> &toCopy);
	PrimitiveGeoVec & operator = (const PrimitiveGeoVec<T,N> &toCopy);
	PrimitiveGeoVec & operator = (const std::vector<T> &std_vec);
	virtual ~PrimitiveGeoVec();
	const std::vector<T> & GetVec()const;
	std::vector<T> & GetVec();
	int Size()const;
	const T & At (int i)const;
	T & At (int i);
	const sVector<double,N> & GetMin()const;
	const sVector<double,N> & GetMax()const;
	void PushBack(const T &newitem);
	void Clear();
	void Serialize (Bofstream &outfile)const;
	void Serialize (Bifstream &infile);
	bool operator == (const PrimitiveGeoVec<T,N> &toCompare)const;
	bool operator != (const PrimitiveGeoVec<T,N> &toCompare)const;
	void Transform (const TransformationMatrix &tm);
	void AddBack (const PrimitiveGeoVec &toAdd);
	void Reserve (int newcapacity);
	void UpdateMinMax ();
protected:
	std::vector<T> m_vec;
	AABB<N> m_aabb;
};

#include "PrimitiveGeoVec.inl"

#endif
