// SequentialLineSet2d.cpp

#include "stdafx.h"
#include "SequentialLineSet2d.h"
#include "VLine2d.h"

#include "TransformationMatrix.h"
#include "SequentialLineSet.h"


SequentialLineSet2d::SequentialLineSet2d ( int initialcapacity )
: sequentialpoints(initialcapacity+1)
{
	
}

SequentialLineSet2d::SequentialLineSet2d ( const SequentialLineSet2d & Foo )
:sequentialpoints ( Foo.sequentialpoints )
{

}

SequentialLineSet2d::SequentialLineSet2d (const SequentialLineSet &sl)
:sequentialpoints (sl.GetPointSize())
{
	int mysize = sl.GetPointSize();
	for (int i=0;i<mysize;++i)
	{
		const VPoint *p = sl.GetPoint(i);
		VPoint2d p2 (p->x(),p->y());
		sequentialpoints.InsertPoint (p2);
	}
}
VLine2d SequentialLineSet2d::operator [] ( int index ) const
{
	int size = Size();
	
	if ( index >= size || index < 0 ) 
		throw ( VException ( ( IDS_SeqLinSetExc) , 0 ) );
		
	const VPoint2d * firstp = sequentialpoints[index];
	const VPoint2d * secondp = sequentialpoints[index+1];
	
	VLine2d result ( *firstp,*secondp );
	return result;
}

int SequentialLineSet2d::Size () const
{
	if ( sequentialpoints.Size() > 0 )
		return ( sequentialpoints.Size()-1 );
	else
		return 0;
}

int SequentialLineSet2d::GetPointSize ()const
{
	return sequentialpoints.Size();
}

void SequentialLineSet2d::InsertPoint ( const VPoint2d & p )
{
	if (GetPointSize() == 0)
		sequentialpoints.InsertPoint ( p.GetArray() );
	else if (p.Compare (*End() , EPSILON) == false)
		sequentialpoints.InsertPoint ( p.GetArray() );
}

VPoint2d SequentialLineSet2d::GetMin () const
{
	return (sequentialpoints.GetMin());
}

VPoint2d SequentialLineSet2d::GetMax () const
{
	return (sequentialpoints.GetMax());
}

const VPoint2d * SequentialLineSet2d::GetPoint ( int index ) const
{
	return ( sequentialpoints[index] );
}

VPoint2d * SequentialLineSet2d::GetPoint ( int index )
{
	return ( sequentialpoints[index] );
}

void SequentialLineSet2d::Serialize (Bofstream &outfile)const
{
	sequentialpoints.Serialize(outfile);
}
	
void SequentialLineSet2d::Serialize (Bifstream &infile)
{
	sequentialpoints.Serialize(infile);
}

PointSet2d & SequentialLineSet2d::GetPointSet()
{
	return sequentialpoints;
}

const PointSet2d & SequentialLineSet2d::GetPointSet()const
{
	return sequentialpoints;
}

bool SequentialLineSet2d::operator == (const SequentialLineSet2d &toCompare)const
{
	if (this==&toCompare)
		return true;

	if(sequentialpoints!=toCompare.sequentialpoints)
		return false;

	return true;
}

bool SequentialLineSet2d::operator != (const SequentialLineSet2d &toCompare)const
{
	return !(this->operator ==(toCompare));
}

vreal SequentialLineSet2d::GetLength()const
{
	//to be implemented...
	vreal totalLength = 0.0;
	int lineCount = Size();
	for (int i=0; i<lineCount; i++)
	{
		VLine2d currLine = this->operator [](i);
		vreal currLength = currLine.GetLength();
		totalLength += currLength;
	}
	return totalLength;
}	

VPoint2d SequentialLineSet2d::GetP (const vreal &t)const
{
	// to be implemented
	vreal setLength = GetLength();
	vreal totalL = 0.0;
	int mysize = Size();
	vreal p0 = 0.0;
	vreal p1 = 0.0;
	for (int i=0;i<mysize;i++)
	{
		VLine2d currL = operator[](i);
		totalL += currL.GetLength();
		p1 = totalL / setLength;
		if (t>=p0-1.e-8 && t<=p1+1.e-8)
		{
			VPoint2d myp = currL.GetP1() + currL.GetV() * ((t - p0) / (p1 - p0));
			return myp;
		}
		p0 = p1;
	}
	throw VException (( IDS_SeqGetTExc),1);
}

VPoint2d SequentialLineSet2d::GetP (const vreal &t,const vreal &setLength)const
{
	vreal totalL = 0.0;
	int mysize = Size();
	vreal p0 = 0.0;
	vreal p1 = 0.0;
	for (int i=0;i<mysize;i++)
	{
		VLine2d currL = operator[](i);
		totalL += currL.GetLength();
		p1 = totalL / setLength;
		if (t>=p0-1.e-8 && t<=p1+1.e-8)
		{
			VPoint2d myp = currL.GetP1() + currL.GetV() * ((t - p0) / (p1 - p0));
			return myp;
		}
		p0 = p1;
	}
	throw VException (( IDS_SeqGetTExc),1);
}

const VPoint2d * SequentialLineSet2d::Start()const
{
	return 	GetPoint(0);
}
	
const VPoint2d * SequentialLineSet2d::End()const
{
	return GetPoint (GetPointSize()-1);
}

VPoint2d * SequentialLineSet2d::Start()
{
	return 	GetPoint(0);
}
	
VPoint2d * SequentialLineSet2d::End()
{
	return GetPoint (GetPointSize()-1);
}

void SequentialLineSet2d::Reverse()
{
	int mysize = GetPointSize();
	SequentialLineSet2d tmp (mysize);
	for (int i=mysize-1; i>=0; i--)
	{
		const VPoint2d *currp = GetPoint (i);
		tmp.InsertPoint (*currp);
	}
	*this = tmp;
}
	
void SequentialLineSet2d::AddBack (const SequentialLineSet2d &toAdd)
{
	int mysize = toAdd.GetPointSize();
	for(int i=0;i<mysize;i++)
	{
		const VPoint2d *currp = toAdd.GetPoint(i);
		InsertPoint (*currp);
	}
}
void SequentialLineSet2d::Reserve(int newcapacity)
{
	sequentialpoints.Reserve (newcapacity);
}

void SequentialLineSet2d::TransformToSquare (const vreal u0 , const vreal u1
											 ,const vreal v0 , const vreal v1)
{
	// This function is used for transforming a trimming loop 
	// in unit square u[0,1]v[0,1] to a square u[u0,u1] v[v0,v1]
	// u0,u1 and v0,v1 are owner surface evaluation parameter range
	vreal du = u1 - u0;
	vreal dv = v1 - v0;

	int mysize = GetPointSize();
	for (int i=0;i<mysize;++i)
	{
		VPoint2d *p = GetPoint (i);
		p->x ( u0 + p->x() * du );
		p->y ( v0 + p->y() * dv );
	}
}

int SequentialLineSet2d::GetMinRef() const
{
	VPoint2d mn = GetMin();
	vreal d2 (MAX_NUMBER_LIMIT);
	int res =-1;
	int mysize=GetPointSize();
	for (int i=0;i<mysize;++i)
	{
		const VPoint2d *cp = GetPoint (i);
		vreal mydist2 = (mn - *cp).GetLength2();
		if (mydist2 < d2)
		{
			res = i;
			d2 = mydist2;
			if (fabs (d2) < 1.e-20)
				return res;
		}
	}
	return res;
}

void SequentialLineSet2d::MakeTenDigits()
{
	int mysize = GetPointSize();
	for (int i=0;i<mysize;++i)
	{
		VPoint2d *p=GetPoint (i);
		p->MakeTenDigits();
	}
}

void SequentialLineSet2d::Transform (const TransformationMatrix &tm)
{
	sequentialpoints.Transform (tm);
}

void SequentialLineSet2d::FixDiscontinuetyInXForTrimLoop01(vreal tol)
{
	int psize = GetPointSize();
	for (int pi = 0; pi < psize; ++pi)
	{
		VPoint2d * p = GetPoint(pi);
		if (fabs(p->x()) < tol || fabs(p->x() - 1.0) < tol)
		{
			int pii = GetFirstPrevXNot01(pi, tol);
			if (pii != -1)
			{
				VPoint2d *pp = GetPoint(pii);
				vreal xp = pp->x();
				if (fabs(xp - 1.0) < fabs(xp))
					p->x(1.0);
				else
					p->x(0.0);
			}
			else
			{
				int pnn = GetFirstNextXNot01(pi, tol);
				if (pnn == -1)
					throw VException("SequentialLineSet2d::FixDiscontinuetyInXForTrimLoop01 exception no prev no next!",1);
				VPoint2d *nn = GetPoint(pnn);
				vreal xn = nn->x();
				if (fabs(xn - 1.0) < fabs(xn))
					p->x(1.0);
				else
					p->x(0.0);
			}
		}
	}
}

void SequentialLineSet2d::FixDiscontinuetyInYForTrimLoop01(vreal tol)
{
	int psize = GetPointSize();
	for (int pi = 0; pi < psize; ++pi)
	{
		VPoint2d * p = GetPoint(pi);
		if (fabs(p->y()) < tol || fabs(p->y() - 1.0) < tol)
		{
			int pii = GetFirstPrevYNot01(pi, tol);
			if (pii != -1)
			{
				VPoint2d *pp = GetPoint(pii);
				vreal yp = pp->y();
				if (fabs(yp - 1.0) < fabs(yp))
					p->y(1.0);
				else
					p->y(0.0);
			}
			else
			{
				int pnn = GetFirstNextYNot01(pi, tol);
				if (pnn == -1)
					throw VException("SequentialLineSet2d::FixDiscontinuetyInXForTrimLoop01 exception no prev no next!", 1);
				VPoint2d *nn = GetPoint(pnn);
				vreal yn = nn->y();
				if (fabs(yn - 1.0) < fabs(yn))
					p->y(1.0);
				else
					p->y(0.0);
			}
		}
	}
}

int SequentialLineSet2d::GetFirstPrevXNot01(int pi, vreal tol)
{
	for (int ii = pi - 1; ii >= 0; --ii)
	{
		VPoint2d *p = GetPoint(ii);
		if (fabs(p->x()) > tol && fabs(p->x() - 1.0) > tol)
			return ii;
	}
	return -1;
}
int SequentialLineSet2d::GetFirstNextXNot01(int pi, vreal tol)
{
	int mysize = GetPointSize();
	for (int ii = pi + 1; ii < mysize; ++ii)
	{
		VPoint2d *p = GetPoint(ii);
		if (fabs(p->x()) > tol && fabs(p->x() - 1.0) > tol)
		{
			return ii;
		}
	}
	return -1;
}
int SequentialLineSet2d::GetFirstPrevYNot01(int pi, vreal tol)
{
	for (int ii = pi - 1; ii >= 0; --ii)
	{
		VPoint2d *p = GetPoint(ii);
		if (fabs(p->y()) > tol && fabs(p->y() - 1.0) > tol)
			return ii;
	}
	return -1;
}
int SequentialLineSet2d::GetFirstNextYNot01(int pi, vreal tol)
{
	int mysize = GetPointSize();
	for (int ii = pi + 1; ii < mysize; ++ii)
	{
		VPoint2d *p = GetPoint(ii);
		if (fabs(p->y()) > tol && fabs(p->y() - 1.0) > tol)
		{
			return ii;
		}
	}
	return -1;
}