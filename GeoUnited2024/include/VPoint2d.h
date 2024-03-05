// VPoint2d.h: interface for the VPoint2d class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __VPoint2d_h__
#define __VPoint2d_h__

#include "BasicDefinitions.h"

#include "Bifstream.h"
#include "Bofstream.h"
#include "sVector.h"

//#define TESTNEWPOINT 1

struct VPoint2d  
{
private:
	sVector<double,2> m_vector;
public:
	friend class SerializerPoint;
	friend const VPoint2d operator * (const scalerType &scale_factor , const VPoint2d &pt);
	friend const VPoint2d operator * (const VPoint2d &pt , const scalerType &scale_factor);
	friend const VPoint2d operator + (const VPoint2d &p1 , const VPoint2d &p2);
	friend const VPoint2d operator - (const VPoint2d &p1 , const VPoint2d &p2);
	friend const scalerType operator % (const VPoint2d &p1 , const VPoint2d &p2);
	friend const scalerType operator ^ (const VPoint2d &p1 , const VPoint2d &p2);

	VPoint2d();	
	VPoint2d (const scalerType &defaultValue);
	VPoint2d (const scalerType pt[]);
	VPoint2d (const scalerType &px,const scalerType &py);
	VPoint2d (const VPoint2d &toCopy);
	VPoint2d (const sVector<double,2> &v);
	VPoint2d & operator = (const VPoint2d &pt);
	VPoint2d & operator = (const scalerType pt[]);
	scalerType &x();
	scalerType &y();
	bool operator == (const VPoint2d &comp)const;
	bool operator != (const VPoint2d &comp)const;
	const VPoint2d operator -()const;
	VPoint2d & operator += (const VPoint2d& pt);
	VPoint2d & operator -= (const VPoint2d& pt);
	VPoint2d & operator *= (const scalerType &scalef);
	VPoint2d & operator /= (const scalerType &dividef);
	scalerType & operator [] (int i);
	const scalerType & operator [] (int i)const;
	const scalerType & x() const;
	const scalerType & y() const;
	void x( const scalerType &new_x);
	void y( const scalerType &new_y);
	const scalerType * GetArray ()const;
	scalerType * GetArray();
	VPoint2d Unit()const;
	const VPoint2d operator / (const scalerType &f)const;
	void ReplaceMin (const VPoint2d &source);
	void ReplaceMax (const VPoint2d &source);
	void UpdateMinMax(VPoint2d &minp,VPoint2d &maxp)const;
	scalerType GetLength()const;
	scalerType GetLength2()const;
	bool IsCoincident (const VPoint2d &comp)const;
	bool Compare (const VPoint2d & comp , const scalerType & comptol)const;
	void NormThis();
	void operator << (VIFStream & infile);
	void operator >> (VofStream & outfile)const;
	VPoint2d (VIFStream & infile);
	void Serialize (Bofstream &binfile)const;
	void Serialize (Bifstream &binfile); 
	VPoint2d (Bifstream & infile);
	bool IsCorrupted ()const;
	void MAKEMAXPOINT();
	void MAKEMINPOINT();
	void MakeTenDigits ();
	const sVector<double,2> &GetMin()const;
	const sVector<double,2> &GetMax()const;
};

//#############################################################################
//#############################################################################

	const VPoint2d UX2D(1.,0.);
	const VPoint2d UY2D(0.,1.);
	bool isLessInX(std::list<VPoint2d>::const_iterator p1,std::list<VPoint2d>::const_iterator p2);
	bool isLessInY(std::list<VPoint2d>::const_iterator p1,std::list<VPoint2d>::const_iterator p2);
	bool isGreaterInX(std::list<VPoint2d>::const_iterator p1,std::list<VPoint2d>::const_iterator p2);
	bool isGreaterInY(std::list<VPoint2d>::const_iterator p1,std::list<VPoint2d>::const_iterator p2);
	const VPoint2d operator * (const scalerType &scale_factor , const VPoint2d &pt);	
	const VPoint2d operator * (const VPoint2d &pt , const scalerType &scale_factor);
	const VPoint2d operator + (const VPoint2d &p1 , const VPoint2d &p2);
	const VPoint2d operator - (const VPoint2d &p1 , const VPoint2d &p2);
	const scalerType operator % (const VPoint2d &p1 , const VPoint2d &p2);
	const scalerType operator ^ (const VPoint2d &p1 , const VPoint2d &p2);
	typedef VPoint2d VVector2d;
	typedef VPoint2d UnitVector2d;
	const VPoint2d UX2d(1.,0.);
	const VPoint2d UY2d(0.,1.);
	const VPoint2d UZ2d(0.,0.);
	const VPoint2d & GETMINPOINT2D();
	const VPoint2d & GETMAXPOINT2D();

#endif //__VPoint2d_h__
