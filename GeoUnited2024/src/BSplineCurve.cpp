// BSplineCurve.cpp

// K:upper index of sum (int)
// M:degree of basis functions (int)
// m_PROP1 (int) 0 = nonplanar 1 = planar
// m_PROP2 (int) 0 = opencurve 1 = closed
// m_PROP3 (int) 0 = rational 1 = polynomial
// m_PROP4 (int) 0 = non periodic 1 = periodic
// rem: N = 1 + K - M    A = N + 2 * M
// m( number of m_T is A+1 )
// m_W ( number of m_W is K+1 )
// controlpoints ( number of control points is (K+1) points (K+1)*3 vreals values )


#include "stdafx.h"
#include "BSplineCurve.h"
#include "Test_IgesSerialize.h"
#include "IgesEntityVisitor.h"
#include "IgesEntitySet.h"
#include "SequentialLineSet.h"
#include "TransformationMatrix.h"

BSplineCurve::BSplineCurve()
{
	entitytype = 126;
}

BSplineCurve::BSplineCurve (int degreeOfBasisFunctions
							, int controlPointCount
							, bool isRational
							, bool isPeriodic
							, const std::vector<vreal> &knots
							, const std::vector<vreal> &weights
							, const std::vector<vreal> &controlp
							,const vreal &V0
							,const vreal &V1)
							:m(knots),m_W(weights),controlpoints(controlp)
{
	entitytype = 126;
	m_PROP1 = 0;//nonplanar
	m_PROP2 = 0;//open curve
	if (isRational == false)
		m_PROP3 = 1;//polynomial (not rational)
	else
		m_PROP3 = 0;//rational

	if (isPeriodic == false)
		m_PROP4 = 0;//non periodic
	else
		m_PROP4 = 1;//periodic

	m_K = controlPointCount - 1;
	m_M = degreeOfBasisFunctions;
	m_N = 1 + m_K - m_M;
	m_A = m_N + 2 * m_M;
	//for (int i=0;i<controlPointCount;i++)
	//{
	//	m_W.push_back(1.0);
	//}
	m_V0 = V0;
	m_V1 = V1;
	m_XNORM = 0.;
	m_YNORM = 0.;
	m_ZNORM = 0.;
}

BSplineCurve::~BSplineCurve()
{
	//delete [] m_T;
	//delete [] m_W;
	//delete [] controlpoints;
}

void BSplineCurve::GetEntityIn ( FileLines & mylines , int poffs)
{
	DoubleVector myvect;
	GetAllParam ( myvect , mylines );
	int vecsize = (int)myvect.size();
	if (vecsize <= 6)
		throw VException (( IDS_BSpGetExcNotEno),1);
	m_K = (int) (myvect[1]);
	m_M = (int) (myvect[2]);
	m_PROP1 = (int) myvect[3];
	m_PROP2 = (int) myvect[4];
	m_PROP3 = (int) myvect[5];
	m_PROP4 = (int) myvect[6];

	m_N = 1 + m_K - m_M;
	m_A = m_N + 2 * m_M;
	
	m_T.resize(m_A+1); //= new vreal [m_A+1];
	if (vecsize <= (10 + m_A + m_K + (m_K+1)*3))
		throw VException (( IDS_BSpGetExcNotEno1),1);
	for ( int i=0; i<m_A+1; i++ )
	{
		m_T[i] = static_cast<vreal> (myvect[7+i]);
	}
	
	m_W.resize(m_K+1); //= new vreal[m_K+1];
	
	for ( int i=0; i < m_K+1; i++ )
	{
		m_W[i] = static_cast<vreal> (myvect[8+m_A+i]);
	}

	controlpoints.resize((m_K+1)*3); //= new vreal [(m_K+1)*3];

	for ( int i=0; i < (m_K+1)*3; i++ )
	{
		controlpoints[i] = static_cast<vreal> (myvect[9+m_A+m_K+i]);
		// for deforming
		//if ((i % 3) == 2)
		//{
		//	controlpoints[i] += ((fabs(controlpoints[i-2])) * (-1.0 / 350.0));
		//}
	}

	int myoffset = 9 + m_A + m_K + (m_K+1)*3;
	m_V0 = (vreal)myvect[myoffset];
	m_V1 = (vreal)myvect[myoffset+1];

	// for safety
	//if ( fabs(m_T[m_M]-m_V0) > EPSILON)
	//	m_V0 = m_T[m_M];
	//if ( fabs(m_T[m_K+1]-m_V1) > EPSILON)
	//	m_V1 = m_T[m_K+1];
	int finali = myoffset + 2;
	if ( static_cast<int>(myvect.size()) >= (myoffset+4) )
	{
		m_XNORM = (vreal)myvect[myoffset+2];
		m_YNORM = (vreal)myvect[myoffset+3];
		m_ZNORM = (vreal)myvect[myoffset+4];
		finali = myoffset + 5;
	}
	else
	{
		m_XNORM = -1.;
		m_YNORM = -1.;
		m_ZNORM = -1.;
	}
	ReadModelId (finali,myvect);

#ifdef IGES_TEST_MODE
	Test_IgesSerialize mytester;
	mytester.Test_WriteEntityOut (*this,myvect);
#endif
}

void BSplineCurve::WriteEntityOut (std::vector<VStr> &strvec)const
{

	WriteParam (entitytype , strvec);
	WriteParam (m_K , strvec);
	WriteParam (m_M , strvec);
	WriteParam (m_PROP1 , strvec);
	WriteParam (m_PROP2 , strvec);
	WriteParam (m_PROP3 , strvec);
	WriteParam (m_PROP4 , strvec);
	
	for ( int i=0; i<m_A+1; i++ )
	{
		WriteParam (m_T[i],strvec);
	}
	
	//m_W = new vreal[m_K+1];

	for ( int i=0; i < m_K+1; i++ )
	{
		WriteParam (m_W[i] , strvec);
	}

	//controlpoints = new vreal [(m_K+1)*3];

	for ( int i=0; i < (m_K+1)*3; i++ )
	{
		WriteParam (controlpoints[i] , strvec);
	}

	WriteParam (m_V0 , strvec);
	WriteParam (m_V1 , strvec);
	
	//if (m_XNORM != -1.)
	{
		WriteParam (m_XNORM , strvec );
	}
	
	//if (m_YNORM != -1.)
	{		
		WriteParam (m_YNORM , strvec);
	}

	//if (m_ZNORM != -1.)
	{
		WriteParam (m_ZNORM , strvec);
	}

	WriteModelId(strvec);
}

// PROP1 (int) 0 = nonplanar 1 = planar
// PROP2 (int) 0 = opencurve 1 = closed
// PROP3 (int) 0 = rational 1 = polynomial
// PROP4 (int) 0 = non periodic 1 = periodic

bool BSplineCurve::IsClosed () const
{
	if ( m_PROP2 == 1 )
		return true;
	else
		return false;
}

//#############################################################################

bool BSplineCurve::IsPeriodic () const
{
	if ( m_PROP4 == 1 )
		return true;
	else
		return false;
}

//#############################################################################

bool BSplineCurve::IsPlanar () const
{
	if ( m_PROP1 == 1 )
		return true;
	else
		return false;
}

//#############################################################################

bool BSplineCurve::IsRational () const
{
	if ( m_PROP3 == 0 )
		return true;
	else
		return false;
}

//#############################################################################

int BSplineCurve::GetDegreeOfBasisFunctions () const
{
	return (m_M);
}

//#############################################################################


int BSplineCurve::GetControlPointNo () const
{
	return (m_K+1);
}

void BSplineCurve::GetChildren (std::vector<int> &childVec)const
{
	GetDirectoryChild (childVec);
}

void BSplineCurve::AcceptVisitor (IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual (*this);
}

//bool BSplineCurve::IsPrimitiveEntity()const
//{
//	return true;
//}

//double BSplineCurve::GetSortNo ()const
//{
//	return 8.0;
//}

IgesEntity * BSplineCurve::MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const
{
	if (m_subsetIndex != 0)
		return newSet.GetEntityPointer (m_subsetIndex);

	BSplineCurve *mycopy = new BSplineCurve(*this);
	CopyDE (*mycopy,mainSet,newSet);
	newSet.InsertEntity(mycopy);
	m_subsetIndex = mycopy->directorylineno;
	return mycopy;
}

bool BSplineCurve::IsCircularArc()const
{
	if (DE.m_formNo == 2)
		return true;
	else
		return false;
}
bool BSplineCurve::IsLine()const
{
	if (DE.m_formNo == 1)
		return true;
	else
		return false;
}
bool BSplineCurve::IsPolyline()const
{
	if (m_M == 1)
		return true;
	else
		return false;
}
bool BSplineCurve::IsEllipticalArc()const
{
	if (DE.m_formNo == 3)
		return true;
	else
		return false;
}
bool BSplineCurve::IsParabolicArc()const
{
	if (DE.m_formNo == 4)
		return true;
	else
		return false;
}
bool BSplineCurve::IsHyperbolicArc()const
{
	if (DE.m_formNo == 5)
		return true;
	else
		return false;
}

SequentialLineSet * BSplineCurve::MakeControlPointsSl()const
{
	int cno = GetControlPointNo();
	int csize = cno*3;
	SequentialLineSet *sl = new SequentialLineSet (cno);
	for (int ci=0;ci<csize;ci+=3)
	{
		VPoint curcopoint (controlpoints.at(ci),controlpoints.at(ci+1),controlpoints.at(ci+2));
		sl->InsertPoint (curcopoint);
	}
	return sl;
}

void BSplineCurve::Transform(const TransformationMatrix &tm)
{
	int pcount = GetControlPointNo();
	for (int i = 0; i<pcount; ++i)
	{
		int si = i * 3;
		VPoint curp(controlpoints.at(si), controlpoints.at(si + 1), controlpoints.at(si + 2));
		curp = tm.Transform(curp);
		controlpoints[si] = curp.x();
		controlpoints[si + 1] = curp.y();
		controlpoints[si + 2] = curp.z();
	}
	VPoint vec(m_XNORM, m_YNORM, m_ZNORM);
	vec = tm.TransformVec(vec);
	m_XNORM = vec.x();
	m_YNORM = vec.y();
	m_ZNORM = vec.z();
}