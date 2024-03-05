// BSplineSurface.cpp

#include "stdafx.h"
#include "BSplineSurface.h"
#include "Test_IgesSerialize.h"
#include "IgesEntityVisitor.h"
#include "IgesEntitySet.h"
#include "UntypedException.h"
#include <stdexcept>

BSplineSurface::BSplineSurface()
{
	entitytype = 128;
	//m_S = NULL;
	//m_T = NULL;
	//m_W = NULL;
	//controlpoints = NULL;
}

BSplineSurface::~BSplineSurface()
{
	//delete [] m_S;
	//delete [] m_T;
	//delete [] m_W;
	//delete [] controlpoints;
}
int BSplineSurface::CalculateA (const int N1 , const int M1)const
{
	int A=N1 + 2 * M1;
	return A;
}
int BSplineSurface::CalculateN1(const int K1 , const int M1)const
{
	int N1=1 + K1 - M1;
	return N1;
}
int BSplineSurface::CalculateN2(const int K2 , const int M2)const
{
	int N2 = 1 + K2 - M2;
	return N2;
}
int BSplineSurface::CalculateB (const int N2 , const int M2)const
{
	int B = N2 + 2 * M2;
	return B;
}
int BSplineSurface::CalculateC (const int K1 , const int K2)const
{
	int C = ( 1 + m_K1 ) * ( 1 + m_K2 );
	return C;
}
void BSplineSurface::GetEntityIn( FileLines & mylines , int poffs)
{
	try
	{
		DoubleVector myvect;
		GetAllParam ( myvect , mylines );
		int vecsize = (int)myvect.size();
		if (vecsize<=9)
			throw VException (( IDS_BSpGetExcNotEno2),1);
		m_K1 = (int)myvect[1];
		m_K2 = (int)myvect[2];
		m_M1 = (int)myvect[3];
		m_M2 = (int)myvect[4];

		m_PROP1 = (int) myvect[5];
		m_PROP2 = (int) myvect[6];
		m_PROP3 = (int) myvect[7];
		m_PROP4 = (int) myvect[8];
		m_PROP5 = (int) myvect[9];

		m_N1 = CalculateN1 (m_K1 , m_M1);
		m_N2 = CalculateN2 (m_K2 , m_M2);

		m_A = CalculateA (m_N1 , m_M1);
		m_B = CalculateB (m_N2,m_M2);
		m_C = CalculateC (m_K1,m_K2);
		
		m_S.resize(m_A+1); //= new vreal [m_A+1];// first knot sequence ( number of first knots:A+1)
		m_T.resize(m_B+1); //= new vreal [m_B+1];// second knot sequence ( number of second knots:B+1 )	

		int myvectsize = (int)myvect.size();
		if ( myvectsize <= (13 + m_A + 1 + m_B + 1 + m_C + (3 * m_C)))
			throw VException (( IDS_BSpGetExcNotEno3),1);	

		int offset = 10;

		for ( int i=0; i<m_A+1; i++ )
		{
			m_S[i] = static_cast<vreal> (myvect[i+offset]);
		}

		offset = 10 + m_A + 1;
		for ( int i = 0; i < m_B+1; i++ )
		{
			m_T[i] = static_cast<vreal> (myvect[i+offset]);
		}
		
		offset = 10 + m_A + 1 + m_B + 1;

		m_W.resize(m_C);// = new vreal[m_C]; // number of weights : C

		for ( int i=0; i < m_C; i++ )
		{
			m_W[i] = static_cast<vreal> (myvect[i+offset]);
		}

		offset = 10 + m_A + 1 + m_B + 1 + m_C;

		controlpoints.resize(m_C*3); //= new vreal [m_C*3]; // number of control points: C

		for ( int i=0; i < m_C*3; i++ )
		{
			controlpoints[i] = static_cast<vreal> (myvect[i+offset]);
			// for deforming
			//if ((i % 3) == 2)
			//{
			//	controlpoints[i] += ((fabs(controlpoints[i-2])) * (-1.0 / 350.0));
			//}
		}

		offset = 10 + m_A + 1 + m_B + 1 + m_C + (3 * m_C) ;
		
		// filling the parameters start and end values
		m_U0 = static_cast<vreal> (myvect[offset]);
		m_U1 = static_cast<vreal> (myvect[offset+1]);
		m_V0 = static_cast<vreal> (myvect[offset+2]);
		m_V1 = static_cast<vreal> (myvect[offset+3]);

		// for safety
		//if ( fabs(m_S[m_M1]-m_U0) > EPSILON)
		//	m_U0 = m_S[m_M1];
		//if ( fabs(m_S[m_K1+1]-m_U1) > EPSILON)
		//	m_U1 = m_S[m_K1+1];
		
		// for safety
		//if ( fabs(m_T[m_M2]-m_V0) > EPSILON)
		//	m_V0 = m_T[m_M2];
		//if ( fabs(m_T[m_K2+1]-m_V1) > EPSILON)
		//	m_V1 = m_T[m_K2+1];

	#ifdef IGES_TEST_MODE
		Test_IgesSerialize mytester;
		mytester.Test_WriteEntityOut (*this,myvect);
	#endif
	}
	catch (exception &exc)
	{
		throw exc;
	}
	catch (...)
	{
		throw UntypedException (( IDS_UnkExcInBSpGet),1);
	}
}

/*
5             PROP1      Integer   1 = Closed in first parametric variable direction
                                    0 = Not closed
6             PROP2      Integer   1 = Closed in second parametric variable direction
                                    0 = Not closed
7             PROP3      Integer   0 = Rational
                                    1 = Polynomial
8             PROP4      Integer   0 = Nonperiodic in first parametric variable direction
                                    1 = Periodic in first parametric variable direction
9             PROP5      Integer   0 = Nonperiodic in second parametric variable direction
                                    1 = Periodic in second parametric variable direction
*/

bool BSplineSurface::IsClosedInU () const
{
	if ( m_PROP1 == 1 )
		return true;
	else
		return false;
}

//#############################################################################

bool BSplineSurface::IsClosedInV () const
{
	if ( m_PROP2 == 1 )
		return true;
	else
		return false;
}

//#############################################################################

bool BSplineSurface::IsPeriodicInU () const
{
	if ( m_PROP4 == 1 ) 
		return true;
	else
		return false;
}

//#############################################################################

bool BSplineSurface::IsPeriodicInV () const
{
	if ( m_PROP5 == 1 )
		return true;
	else
		return false;
}

//#############################################################################

bool BSplineSurface::IsRational () const
{
	if ( m_PROP3 == 0 )
		return true;
	else
		return false;
}

//#############################################################################

int BSplineSurface::GetDegreeOfBasisFunctionsU () const
{
	return m_M1;
}

int BSplineSurface::GetDegreeOfBasisFunctionsV () const
{
	return m_M2;
}

int BSplineSurface::GetControlPointNoU () const
{
	return (m_K1+1);
}

int BSplineSurface::GetControlPointNoV () const
{
	return (m_K2+1);
}

void BSplineSurface::WriteEntityOut (std::vector<VStr> &strvec)const
{
		

	WriteParam (entitytype , strvec);
	WriteParam (m_K1 , strvec);
	WriteParam (m_K2 , strvec);
	WriteParam (m_M1 , strvec);
	WriteParam (m_M2 , strvec);
	WriteParam (m_PROP1 , strvec);
	WriteParam (m_PROP2 , strvec);
	WriteParam (m_PROP3 , strvec);
	WriteParam (m_PROP4 , strvec);
	WriteParam (m_PROP5 , strvec);
	
	for ( int i=0; i<m_A+1; i++ )
	{
		//m_S[i] = static_cast<vreal> (myvect[i+offset]);
		WriteParam (m_S[i] , strvec);
	}

	for ( int i = 0; i < m_B+1; i++ )
	{
		WriteParam (m_T[i] , strvec);
	}
	
	for ( int i=0; i < m_C; i++ )
	{
		WriteParam (m_W[i] , strvec);
	}

	for ( int i=0; i < m_C*3; i++ )
	{
		//DeformSurface (controlpoints , m_C);
		//void DeformSurface (const int coordinateno , const vreal &moveProportional);
		//{
			//coordinateno shows which coordinate to modify
			//coordinateno == 0 For X
			//coordinateno == 1 For Y
			//coordinateno == 2 For Z 

			//moveProportional shows how much to add to single control point coordinate
			//if controlpoint coordinate is 5 , -7 , 8 and Y is coordinate to modify and moveProportional = 10
			
		//}
		// for deforming
		//if ((i % 3) == 1)
		//{
		//	controlpoints[i] += ((fabs(controlpoints[i-1])) * (-1.0 / 350.0));
		//}

		WriteParam (controlpoints[i] , strvec);
	}
	
	WriteParam (m_U0 , strvec);
	WriteParam (m_U1 , strvec);
	WriteParam (m_V0 , strvec);
	WriteParam (m_V1 , strvec);

}

void BSplineSurface::GetChildren (std::vector<int> &childVec)const
{
	GetDirectoryChild (childVec);
}

void BSplineSurface::AcceptVisitor (IgesEntityVisitor &visitor)const
{
	visitor.AcceptVirtual (*this);
}

//bool BSplineSurface::IsPrimitiveEntity()const
//{
//	return true;
//}

//double BSplineSurface::GetSortNo ()const
//{
//	return 12.0;
//}

IgesEntity * BSplineSurface::MakeCopyAndAddToSet (const IgesEntitySet &mainSet , IgesEntitySet &newSet)const
{
	if (m_subsetIndex != 0)
		return newSet.GetEntityPointer (m_subsetIndex);

	BSplineSurface *mycopy = new BSplineSurface (*this);
	CopyDE (*mycopy,mainSet,newSet);
	newSet.InsertEntity(mycopy);
	m_subsetIndex = mycopy->directorylineno;
	return mycopy;
}




void BSplineSurface::ConstructAsCubicUniformPolynomial ( int upointno , int vpointno 
														, const std::vector<vreal> &uval
														, const std::vector<vreal> &knotsu
														, const std::vector<vreal> &vval
														, const std::vector<vreal> &knotsv
														, const std::vector<vreal> &controlPoints 
														, bool isClosedU
														, bool isClosedV
														)
{
	// K1 : upper index of first sum -- this upointno - 1
	this->m_K1 = upointno - 1;
	// K2: upper index of second sum -- this is vpointno -1
	this->m_K2 = vpointno - 1;
	// M1 : degree of first set of basis functions
	this->m_M1 = 3;
	// M2 : degree of second set of basis functions
	this->m_M2 = 3;
	// PROP1 : closed in first parametric direction --> 1 closed , 0 not closed
	m_PROP1 = isClosedU ? 1 : 0;
	// PROP2 : closed in second parametric direction --> 1 closed , 0 not closed
	m_PROP2 = isClosedV ? 1 : 0;
	// PROP3 :  0 rational , 1 polynomial
	m_PROP3 = 1;
	// PROP4 : 0 non periodic in u direction , 1 periodic in u direction
	m_PROP4 = 0;
	// PROP5 : 0 non periodic in v direction , 1 periodic in v direction
	m_PROP5 = 0;
	// N1 = 1 + K1 - M1
	this->m_N1 = 1 + m_K1 - m_M1;
	// N2 = 1 + K2 - M2
	this->m_N2 = 1 + m_K2 - m_M2;
	// A = N1 + 2 * M1
	m_A = m_N1 + 2 * m_M2;
	// B = N2 + 2 * M2
	m_B = m_N2 + 2 * m_M2;
	// C = (1 + K1) * (1 + K2)
	m_C = (1 + m_K1) * (1 + m_K2);
	// S first knot sequence 
	m_S = knotsu;
	// T second knot sequence 
	m_T = knotsv;
	// W : weights
	m_W = std::vector<vreal> ( m_C , 1.0);
	// control points
	this->controlpoints = controlPoints;
	// u and v range
	this->m_U0 = uval.front();
	this->m_U1 = uval.back();
	this->m_V0 = vval.front();
	this->m_V1 = vval.back();
}