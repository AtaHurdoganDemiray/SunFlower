// VPoint.h: interface for the VPoint class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __VPoint_h__
#define __VPoint_h__

#include "BasicDefinitions.h"

#include "Bifstream.h"
#include "Bofstream.h"
#include "sVector.h"


//#define TESTNEWPOINT 1

const VPoint & GETMINPOINT();
const VPoint & GETMAXPOINT();

struct VPoint
{
private:
	sVector<double,3> m_vector;
public:
	friend class SerializerPoint;
	friend const VPoint operator * (const scalerType &f , const VPoint &p);
	friend const VPoint operator * (const VPoint &p , const scalerType &f);
	friend const VPoint operator + (const VPoint &p1 , const VPoint &p2);
	friend const VPoint operator - (const VPoint &p1 , const VPoint &p2);
	friend const scalerType operator % (const VPoint &p1 , const VPoint &p2);
	friend const VPoint operator ^ (const VPoint &p1 , const VPoint &p2);

	enum VectorDirections {NOT_PARALLEL,PARALLEL_SAME,PARALLEL_OPPOSITE};
	scalerType & x();
	scalerType &y();
	scalerType &z();
	bool operator == (const VPoint&comp)const;
	bool operator != (const VPoint&comp)const;
	VPoint();
	VPoint (const scalerType &defaultValue); 
	VPoint (const scalerType pt[]);
	VPoint (const scalerType &px, const scalerType &py, const scalerType &pz);
	VPoint (const VPoint &toCopy);
	VPoint (const sVector<double,3> &v);
	VPoint & operator = (const VPoint& pt);
	VPoint & operator = (const scalerType pt[]);
	const VPoint operator -() const;
	VPoint & operator += (const VPoint& pt);
	VPoint & operator -= (const VPoint& pt);
	const VPoint & operator *= (const scalerType &f);
	const VPoint & operator /= (const scalerType &f);
	const scalerType & operator [] (int i)const;
	scalerType & operator [] (int i);
	const scalerType & x()const;
	const scalerType & y()const;
	const scalerType & z()const;
	void x (const scalerType &new_x);
	void y (const scalerType &new_y);
	void z (const scalerType &new_z);
	const scalerType * GetArray ()const;
	scalerType * GetArray();
	VPoint Unit() const;
	const VPoint operator / (const scalerType &f)const;
	bool ReplaceMin (const VPoint &source);
	bool ReplaceMax (const VPoint &source);
	bool UpdateMinMax (VPoint &minp,VPoint &maxp)const;
	scalerType GetLength()const;
	scalerType GetLength2()const;
	bool IsCoincident (const VPoint &comp)const;
	bool Compare (const VPoint &comp , const scalerType &comptol)const;
	void NormThis ();
	void operator << (VIFStream & infile);
	void operator >> (VofStream & outfile)const;
	VPoint (VIFStream & infile);
	void Serialize (Bofstream &binfile)const;
	void Serialize(Bifstream &binfile); 
	VPoint (Bifstream &infile);
	bool IsCorrupted ()const;
	void MAKEMAXPOINT();
	void MAKEMINPOINT();
	const sVector<double,3> &GetMin()const;
	const sVector<double,3> &GetMax()const;
	static bool IsColinear (const VPoint &p1,const VPoint &p2,const VPoint &p3,double tol);
	static bool IsCoordsExceed (const VPoint &p , double coordLimit);
	static bool IsSharpCorner (const VPoint &c0 , const VPoint &c1 , const VPoint &c2,double cosAngleLimit);//c0 , c1 and c2 are sequential positions on a travel
	static bool IsSharpCornerU  (const VPoint &v0 , const VPoint &v1 , double cosAngleLimit);// v0 and v1 must be unit vectors
	static bool IsSharpCorner (const VPoint &v0 , const VPoint &v1 , double cosAngleLimit);// v0 and v1 are vectors
	VPoint GetPointOnXY()const;
	VStr ToString()const;
	static VPoint FromString(VStr &str);
};

typedef VPoint VVector;
typedef VPoint UnitVector;
const VPoint UX(1.,0.,0.);
const VPoint UY(0.,1.,0.);
const VPoint UZ(0.,0.,1.);
const VPoint operator * (const scalerType &f , const VPoint &p);
const VPoint operator * (const VPoint &p , const scalerType &f);
const scalerType operator % (const VPoint &p1 , const VPoint &p2);
const VPoint operator ^ (const VPoint &p1 , const VPoint &p2);
const VPoint operator + (const VPoint &p1 , const VPoint &p2);
const VPoint operator - (const VPoint &p1 , const VPoint &p2);

bool isLessInX(const VPoint &p1,const VPoint &p2);
bool isLessInY(const VPoint &p1,const VPoint &p2);
bool isLessInZ(const VPoint &p1,const VPoint &p2);
bool isGreaterInX(const VPoint &p1,const VPoint &p2);
bool isGreaterInY(const VPoint &p1,const VPoint &p2);
bool isGreaterInZ(const VPoint &p1,const VPoint &p2);
VPoint::VectorDirections GetAlignTwoDirectionsParam(const VVector &v1,const VVector &v2,UnitVector &rotationAxis,vreal &thetaInDegrees);
bool GetAlignTwoDirectionsAround (const VVector &v1,const VVector &v2,const UnitVector &rotationAxisV1ToV2,vreal &thetaInDegrees);

#endif //__VPoint_h__

