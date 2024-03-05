// EllipsePointGenerator.h

#ifndef ___EllipsePointGenerator_h__
#define ___EllipsePointGenerator_h__
#include "CurvePointGenerator.h"
#include "VEllipse.h"
#include "LineOrArc.h"

class IgesEntity;
class IgesEntitySet;

class EllipsePointGenerator:public CurvePointGenerator
{
public:
	////virtuals of Geometry
	//virtual void Transform (const TransformationMatrix &tm);
	//virtual void Serialize (Bofstream &outfile)const;
	//virtual void Serialize (Bifstream &infile);
	//virtual void AcceptVisitor(GeometryVisitor &visitor);
	//virtual void AcceptVisitor(ConstGeometryVisitor &visitor)const;		
public:
	//virtuals of _Curve
	virtual VPoint GetPoint (const vreal &v)const;
	virtual void GetPointAndTangent (const vreal &v , VPoint &p , VVector &tng)const;
	virtual void Digitise (const vreal &tol , SequentialLineSet &result , DoubleVec &params)const;
	virtual void Digitise (const vreal &tol , SequentialLineSet &result , PointSet &tangents , DoubleVec &params)const;
	virtual void GetDefIntervals (DoubleVec &defint)const;
	virtual void GetSnapPoints (PointSet &snapp)const;
	virtual void GetSnapLines (LineSet &snapl)const;
	virtual VPoint GetTangentDerivative (const vreal &v)const;
	virtual vreal GetRadiusOfCurvature (const vreal &v)const;
	static void EllipseToLineOrArcs (const VEllipse &e , std::vector<LineOrArc> &res , int stepno);
	static void DivideEllipse (const VEllipse &ellipse,int stepno,std::vector<VPoint> &pvec);
	static VPoint EllipsePointGenerator::GetEllipsePoint (const vreal t,const vreal xr2 , const vreal yr2,const vreal yr);

public:
	EllipsePointGenerator (IgesEntity *ent , IgesEntitySet &allent , const VEllipse&elps);
	//EllipsePointGenerator (const VEllipse &elps);
	EllipsePointGenerator (const EllipsePointGenerator &toCopy);		
	EllipsePointGenerator & operator =(const EllipsePointGenerator &toCopy);		
	virtual ~EllipsePointGenerator();
private:
	void Construct(const VEllipse &ellipse);
	vreal GetY(const vreal &x)const;
	vreal GetX(const vreal &y)const;
	vreal GetXForAngle (const vreal &angleInRadian)const;
	VEllipse m_ellipse;
	CoordinateSystem m_cs;
	vreal m_sweepangle1;
	vreal m_sweepangle2;
	vreal m_xr2,m_yr2;
};

#endif