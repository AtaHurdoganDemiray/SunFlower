// CopiousDataPointGenerator.cpp
#include "stdafx.h"
#include "CopiousDataPointGenerator.h"
#include "CopiousData.h"
#include "DoubleVec.h"
#include "LineSetToBSplineCurve.h"
#include "BSplineCurvePointGenerator.h"


CopiousDataPointGenerator::CopiousDataPointGenerator (CopiousData &pdata , IgesEntitySet &allent)
: CurvePointGenerator (&pdata,&allent)
, m_sp(pdata.m_N)
{

	if (pdata.m_IP == 1)
	{
		//2d
		for (int i=0;i<pdata.m_N;i++)
		{
			VPoint p;
			p.x(pdata.tuples[i][0]);
			p.y(pdata.tuples[i][1]);
			p.z(0.0);
			m_sp.InsertPoint (p);
		}
	}
	else if (pdata.m_IP == 2)
	{
		//3d
		for (int i=0;i<pdata.m_N;i++)
		{
			VPoint p;
			p.x(pdata.tuples[i][0]);
			p.y(pdata.tuples[i][1]);
			p.z(pdata.tuples[i][2]);
			m_sp.InsertPoint (p);
		}
	}
	else if (pdata.m_IP == 3)
	{
		//6d
		for (int i=0;i<pdata.m_N*2;i++)
		{
			VPoint p;
			p.x(pdata.tuples[i][0]);
			p.y(pdata.tuples[i][1]);
			p.z(pdata.tuples[i][2]);
			m_sp.InsertPoint (p);
		}
	}
	// Make BSpline and generator
	LineSetToBSplineCurve::Convert(m_sp,m_bs);
	m_bsGen = new BSplineCurvePointGenerator (m_bs,allent);
}

CopiousDataPointGenerator::~CopiousDataPointGenerator()
{
	delete m_bsGen;
}

VPoint CopiousDataPointGenerator::GetPoint ( const vreal & v ) const
{
	// non uniform scaling (each line has same par length regardless of real length)
	//vreal vv = ReverseIf (v);
	//if (m_sp.GetPointSize() == 1)
	//	return (*m_sp.GetPoint(0));
	//int mysize = m_sp.Size();
	//if (fabs(vv-1.0)< 1.e-6)
	//	return *(m_sp.End());

	//vreal single = (vreal)(1.) / mysize;
	//vreal div = vv / single;
	//int round = static_cast<int> (div);
	//vreal remain = vv - round * single;
	//const VPoint &p1 = *(m_sp.GetPoint(round));
	//const VPoint &p2 = *(m_sp.GetPoint(round+1));
	//vreal newV = remain / single;
	//VPoint p = ((vreal)(1.) - newV)*p1 + newV * p2;
	//TransformIf (p);
	//return p;
	vreal vv = ReverseIf (v);
	vv = TransformPar(vv);
	//if (m_sp.GetPointSize() == 1)
	//{
	//	return *m_sp.GetPoint(0);
	//}
	VPoint p =  m_bsGen->GetPoint(vv);
	TransformIf (p);
	return p;
}

void CopiousDataPointGenerator::GetPointAndTangent (const vreal &v,VPoint &p,VVector &tng)const
{
	// non uniform scaling (each line has same par length regardless of real length)
	//vreal vv = ReverseIf (v);
	//if (m_sp.GetPointSize() == 1)
	//{
	//	p = *(m_sp.GetPoint(0));
	//	tng = UX;
	//	ReverseTangentIf (tng);
	//	TransformIf (p,tng);
	//	return;
	//}

	//int mysize = m_sp.Size();
	//if (fabs(vv-1.0)< 1.e-6)
	//{
	//	p = *(m_sp.End());
	//	tng = m_sp.operator [](mysize-1).GetV().Unit();
	//	ReverseTangentIf (tng);
	//	TransformIf (p,tng);
	//	return;
	//}

	//vreal single = (vreal)(1.) / mysize;
	//vreal div = vv / single;
	//int round = static_cast<int> (div);
	//vreal remain = vv - round * single;
	//const VPoint &p1 = *(m_sp.GetPoint(round));
	//const VPoint &p2 = *(m_sp.GetPoint(round+1));
	//vreal newV = remain / single;
	//p = ((vreal)(1.) - newV)*p1 + newV * p2;
	//tng = p2 - p1;
	//tng.NormThis();
	//ReverseTangentIf (tng);
	//TransformIf (p,tng);

	vreal vv = ReverseIf (v);
	vv = TransformPar(vv);
	m_bsGen->GetPointAndTangent(vv,p,tng);
	ReverseTangentIf (tng);
	TransformIf (p,tng);
	//p = GetPoint (vv);
	//VPoint pafter , pafterb , pbefore , pbeforeb;
	//bool ckbefore = GetBeforePoints (v,pbeforeb,pbefore);
	//if (!ckbefore)
	//	return;
	//bool ckafter = GetAfterPoints (v,pafterb,pafter);
	//if (!ckafter)
	//	return;
	//VVector v1 = (pafter - pafterb).Unit();
	//VVector v2 = (pbefore - pbeforeb).Unit();
	////if (true || (v1 % v2) < cos (30.0 * DEGREE_TO_RAD))
	//{
	//	// sharp corner return previous tangent
	//	tng = v2;
	//	//LogFileWriter::WriteUnknownExceptionLog (("CopiousDataPointGenerator::GetPointAndTangent sharp corner!"));
	//	//CString temp1,temp2,temp3,temp4;
	//	//temp1.Format ("after 1:%.6f %.6f %.6f",pafterb.x(),pafterb.y(),pafterb.z());
	//	//temp2.Format ("after 2:%.6f %.6f %.6f",pafter.x(),pafter.y(),pafter.z());
	//	//temp3.Format ("before 1:%.6f %.6f %.6f",pbeforeb.x(),pbeforeb.y(),pbeforeb.z());
	//	//temp4.Format ("before 2:%.6f %.6f %.6f",pbefore.x(),pbefore.y(),pbefore.z());
	//	//LogFileWriter::WriteUnknownExceptionLog ((LPCTSTR)temp1);
	//	//LogFileWriter::WriteUnknownExceptionLog ((LPCTSTR)temp2);
	//	//LogFileWriter::WriteUnknownExceptionLog ((LPCTSTR)temp3);
	//	//LogFileWriter::WriteUnknownExceptionLog ((LPCTSTR)temp4);

	//}

	//tng = CurvePointGenerator::GetDervApr (vv , p);

}

bool CopiousDataPointGenerator::GetBeforePoints(double v, VPoint &pbeforeb, VPoint &pbefore)const
{
	double inc = 1.e-6;
	double vbef,vbefbef;
	double len;
	double tol2 (0.04*0.04);
	int counter = 0;
	int maxIter = 2000;
	do 
	{
		if (counter++ > maxIter)
			return false;
		bool ck = DecrementPar (v , inc , vbef , vbefbef);
		if (!ck)
			return false;
		pbeforeb = GetPoint (vbefbef);
		pbefore = GetPoint (vbef);
		len = (pbefore-pbeforeb).GetLength2();
		inc *= 2;
	}while (len<tol2);
	
	return true;
}

bool CopiousDataPointGenerator::GetAfterPoints (double v , VPoint &pafterb , VPoint &pafter)const
{
	double inc = 1.e-6;
	double vaft,vaftbef;
	double len;
	double tol2 (0.04*0.04);
	int counter = 0;
	int maxIter = 2000;
	do
	{
		if (counter++ > maxIter)
			return false;
		bool ck = IncrementPar (v , inc , vaft , vaftbef);
		if (!ck)
			return false;
		pafterb = GetPoint (vaftbef);
		pafter = GetPoint (vaft);
		len = (pafter-pafterb).GetLength2();
		inc *= 2.;
	}while (len<tol2);
	
	return true;
}
bool CopiousDataPointGenerator::DecrementPar(double v, double inc , double &vbef , double &vbefbef)const
{
	vbef = v-inc;
	vbefbef = vbef-inc;
	if (vbefbef < 0.)
	{
		if ((GetPoint(0.0)-GetPoint(1.0)).GetLength() > 1.e-3)//not closed curve
			return false;
		vbef = 1.0 + vbef;
		vbefbef = 1.0 + vbefbef;
	}
	return true;
}

bool CopiousDataPointGenerator::IncrementPar (double v , double inc , double &vaft , double &vaftbef)const
{
	vaftbef = v+inc;
	vaft = vaftbef+inc;
	if (vaft > 1.)
	{
		if ((GetPoint(0.) - GetPoint(1.)).GetLength() > 1.e-3)//not closed!!!
			return false;
		vaftbef = vaftbef - 1.0;
		vaft = vaft - 1.0;
	}
	return true;
}
void CopiousDataPointGenerator::GetDefIntervals ( DoubleVec & defint ) const
{
	//int mysize = m_sp.Size();
	//vreal single = (vreal)(1.) / mysize;
	//for (int i=0;i<mysize;i++)
	//{
	//	vreal currp = i*single;
	//	defint.Addvreal(currp);
	//	//defint.Addvreal(currp+1.e-5);
	//}
	//if(m_sp.GetPointSize() == 1)
	//	defint.Addvreal(0.0);
	//defint.Addvreal (1.);
	return m_bsGen->GetDefIntervals(defint);
}

void CopiousDataPointGenerator::Digitise(const vreal &tol, SequentialLineSet &result , DoubleVec &params) const
{
	DoubleVec myvec;
	GetDefIntervals (myvec);
	int mysize=myvec.Size();
	for (int i=0;i<mysize;i++)
	{
		vreal currpar = myvec[i];
		VPoint currpoint = GetPoint (currpar);
		bool ck = result.InsertPoint (currpoint);
		if (ck)
			params.Addvreal (currpar);
	}
}

void CopiousDataPointGenerator::Digitise(const vreal &tol
										 , SequentialLineSet &result 
										 , PointSet &tangents
										 , DoubleVec &params) const
{
	DoubleVec myvec;
	GetDefIntervals (myvec);
	int mysize=myvec.Size();
	for (int i=0;i<mysize;i++)
	{
		vreal currpar = myvec[i];
		VPoint currpoint,currtangent;
		GetPointAndTangent (currpar,currpoint,currtangent);
		bool ck = result.InsertPoint (currpoint);
		if (ck)
		{
			tangents.InsertPoint (currtangent);
			params.Addvreal (currpar);
		}
	}
}

void CopiousDataPointGenerator::GetSnapPoints (PointSet &snapp)const
{
	VPoint startp = GetPoint(0.0);
	snapp.InsertPoint(startp);
	VPoint midp = GetPoint (0.5);
	snapp.InsertPoint (midp);
	VPoint endp = GetPoint (1.0);
	if (startp.Compare (endp , 1.e-6) == false)
		snapp.InsertPoint(endp);
}
	
void CopiousDataPointGenerator::GetSnapLines (LineSet &snapl)const
{

}

vreal CopiousDataPointGenerator::GetRadiusOfCurvature(const vreal &v) const
{
	return m_bsGen->GetRadiusOfCurvature(v);
}

VPoint CopiousDataPointGenerator::GetTangentDerivative(const vreal &v) const
{
	return VPoint (0.0);
	//return GetTangentDerivativeGeneric(v);
}
