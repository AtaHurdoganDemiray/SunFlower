// sVector.h

#ifndef __sVector_h__
#define __sVector_h__

#include "BasicDefinitions.h"
#include "Bifstream.h"
#include "Bofstream.h"
class TransformationMatrix;

template<class sT , int N> // N: dimension of scaler vector
	struct sVector
{
public:
	friend struct VPoint;
	friend struct VPoint2d;
	//friend const sVector<double,N> operator * (const sT &f , const sVector<double,N> &v);
	//friend const sVector<double,N> operator * (const sVector<double,N> &v , const sT &f);
	//friend const sVector<double,N> operator / (const sT &f , const sVector<double,N> &v);
	
	//friend const sVector<double,N> operator + (const sVector<double,N> &v1 , const sVector<double,N> &v2);
	//friend const sVector<double,N> operator - (const sVector<double,N> &v1 , const sVector<double,N> &v2);
	//friend const sT operator % (const sVector<double,N> &v1 , const sVector<double,N> &v2);

	sVector ();
	sVector (const sT &defaultvalue);
	sVector (const sVector<sT,N> &toCopy);
	sVector (const sT arr[]);
	sVector<sT,N> & operator = (const sVector<sT,N> & Foo);
	sVector<sT,N> & operator = (const sT arr[]);
	//virtual ~sVector ();
	operator const sT * ()const;
	operator sT * ();
	const sT & GetElement (int i) const;
	const sT & operator[](int i)const;
	sT & operator[](int i);
	//sT operator % (const sVector<double,N> &v2) const;
	//void Multiply (const T &f);
	sVector<sT,N> & operator *= (const sT &f);
	sVector<sT,N> & operator /= (const sT &f);
	void SetElement (const sT &value,int i);
	sT * GetArray ();
	const sT * GetArray()const;
	void Serialize (Bofstream & outfile)const;
	void Serialize (Bifstream & infile);
	bool operator == (const sVector<sT,N> &toCompare)const;
	bool operator != (const sVector<sT,N> &toCompare)const;
	const sVector<sT,N> operator - ()const;
	sVector<sT,N> & operator += (const sVector<sT,N> &v);
	sVector<sT,N> & operator -= (const sVector<sT,N> &v);
	const sVector<sT,N> operator / (const sT &f)const;
	const sVector<sT,N> operator + (const sVector<sT,N> &v)const;
	const sVector<sT,N> operator - (const sVector<sT,N> &v)const;
	const sVector<sT,N> operator * (const sT &f)const;
	const sT operator % (const sVector<sT,N> &v)const;
	//sVector<N,T> operator ^ (sVector<N,T> &v2) const;
	void ReplaceMin (const sVector<sT,N> &p);
	void ReplaceMax (const sVector<sT,N> &p);
	//void Transform (const TransformationMatrix &tm);
	const sT GetLength2 ()const;
	const sT GetLength ()const;
	void NormThis ();
	const sVector<sT,N> Unit() const;
private:
	sT m_array[N];
};

//template<int N>
//	const sVector<double,N> operator * (const sT &f,const sVector<double,N> &v);
//template<int N>
//	const sVector<double,N> operator * (const sVector<double,N> &v,const sT &f);
//template<int N>
//	const sVector<double,N> operator + (const sVector<double,N> &v1 ,const sVector<double,N> &v2);
//template<int N>
//	const sVector<double,N> operator - (const sVector<double,N> &v1 ,const sVector<double,N> &v2);
//template<int N>
//	const sT operator % (const sVector<double,N> &v1 , const sVector<double,N> &v2);

typedef sVector<double,3> p3d;
typedef sVector<double,2> p2d;
typedef sVector<float,3> p3f;
typedef sVector<float,2> p2f;
typedef sVector<int,3> p3i;
typedef sVector<int,2> p2i;


#include "sVector.inl"


#endif
