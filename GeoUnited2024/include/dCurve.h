// dCurve.h

#ifndef __dCurve_h__
#define __dCurve_h__

#include "DoubleVec.h"
#include "SequentialLineSet.h"
#include "LineSet.h"
#include "UntrimmedSurface.h"
#include "TransformationMatrix.h"
class TransformationMatrix;
class CurvePointGenerator;
class Edge;
class VArc;

class dCurve
{
public:
	dCurve (const vreal &tol /*, vreal roughTol*/ , IgesEntity *ent ,  IgesEntitySet *entset , bool isReversed);
	dCurve (const vreal tol /*, vreal roughTol */, Edge *ent , IgesEntitySet*entset , bool isReversed , int ei);
	virtual ~dCurve();
	const DoubleVec & GetParams()const;
	DoubleVec & GetParam();
	const SequentialLineSet & GetLines()const;
	SequentialLineSet & GetLines();
	const CurvePointGenerator * GetCurve()const;
	CurvePointGenerator * GetCurve();
	void Transform (const TransformationMatrix &tm);
	const PointSet & GetSnapPointSet()const;
	PointSet & GetSnapPointSet();
	LineSet & GetSnapLineSet();
	const LineSet & GetSnapLineSet()const;
	int GetDeNo()const;
	int GetTrimSrcDeNo ()const;
	void MakeCurve(IgesEntity *ent,IgesEntitySet *entset,bool isReversed);
	void MakeCurve(Edge *ent,IgesEntitySet *entset,bool isReversed,int ei);

	void MakeLines();
	void RemakeLines (const double tol);//,vreal roughTol);
	void MakeSnapPoints();
	void MakeSnapLines();
	void Reverse();
	const IgesEntity * GetIgesEntity()const;
	IgesEntity * GetIgesEntity();
	const IgesEntitySet * GetIgesEntitySet()const;
	double GetTol ()const;
	void TrimTo (vreal trimV0 , vreal trimV1);
	bool GetArcIf (VArc &a);
	bool GetLineIf (VLine &l);
	enum CurveType {line,arc,bspline,conicarc,parametricspline,copiousdata,unknown};
	CurveType GetCurveType ();
	void RollbackPreviousTransform();
	void GetSharpCorners (double angleLimitInDeg , std::vector<double> &sharpCorPar , double aDistanceTol , double &parInc);
	
private:
	vreal m_tol;
	//vreal m_roughTol;
	DoubleVec *m_params;
	SequentialLineSet *m_lines;
	CurvePointGenerator *m_crv;
	LineSet *m_snapl;
	PointSet *m_snapp;
	int m_deno;
	int m_trimSrcDeno;
	bool m_isTransformedBackUp;
	TransformationMatrix m_tmBackUp;
public:
	bool IsArc()const;
	bool IsLine()const;
	bool IsPolyline()const;
	const TransformationMatrix & GetTmBackUp()const;
	int GetEdgeiIf();// returns -1 if not edge
//private:
//	DoubleVec *m_quickParam;
//	SequentialLineSet *m_quickLines;
//public:
//	DoubleVec * GetQuickParam();
//	SequentialLineSet * GetQuickLines();
//	vreal GetRoughTol()const;
//	void SetRoughTol (vreal roughTol);
	static void GetMinMax(std::vector<dCurve *> &dvec, VPoint &mn, VPoint &mx);
};

#endif