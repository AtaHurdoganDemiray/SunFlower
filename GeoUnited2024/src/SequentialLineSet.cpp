// SequentialLineSet.cpp

#include "stdafx.h"
#include "SequentialLineSet.h"
#include "VLine.h"

#include "TransformationMatrix.h"
#include "SequentialLineSet2d.h"
#include "VTriangleE.h"
#include "VPlane.h"
#include "CoordinateSystem.h"
#include "LoopLineInt.h"

#include "LineSetPointMinDist.h"
#include "ArcFit.h"
#include "RotationMatrix.h"
#include "VLine2dE.h"
#include "SequentialLineSet2d.h"
#include "PosCubes2d.h"
#include "LineSetsMinDist.h"
#include "VInterval.h"
#include "PolarCalc2d.h"
#include "LTable.h"


SequentialLineSet::SequentialLineSet ( int initialcapacity )
: sequentialpoints(initialcapacity+1)
{
	
}

SequentialLineSet::SequentialLineSet ( const SequentialLineSet & Foo )
:sequentialpoints(Foo.sequentialpoints)
{

}
SequentialLineSet::SequentialLineSet(const SequentialLineSet2d &sl, const vreal z)
:sequentialpoints (sl.GetPointSize())
{
	int mysize = sl.GetPointSize();
	for (int i=0;i<mysize;++i)
	{
		const VPoint2d *p = sl.GetPoint(i);
		VPoint p3 (p->x(),p->y(),z);
		sequentialpoints.InsertPoint (p3);
	}
}
SequentialLineSet::SequentialLineSet (const PointSet &ps)
	:sequentialpoints (ps.Size())
{
	int psSize = ps.Size();
	for (int i=0;i<psSize;++i)
		InsertPoint (ps.At(i));
}
VLine SequentialLineSet::operator [] ( int index ) const
{
	int size = Size();
	
	if ( index >= size || index < 0 ) 
		throw ( VException ( ( IDS_SeqLinSetExcInd) , 0 ) );
		
	const VPoint * firstp = sequentialpoints[index];
	const VPoint * secondp = sequentialpoints[index+1];	
	VLine result ( *firstp,*secondp );
	return result;
}

int SequentialLineSet::Size () const
{
	if ( sequentialpoints.Size() > 0 )
		return ( sequentialpoints.Size()-1 );
	else
		return 0;
}

int SequentialLineSet::GetPointSize()const
{
	return sequentialpoints.Size();
}

bool SequentialLineSet::InsertPoint ( const VPoint & p )
{
	if (GetPointSize() == 0)
	{
		sequentialpoints.InsertPoint ( p.GetArray() );
		return true;
	}
	else if (p.Compare (*End() , EPSILON) == false)
	{
		sequentialpoints.InsertPoint ( p.GetArray() );
		return true;
	}
	return false;
}

VPoint* SequentialLineSet::GetPoint(int i)
{
	return sequentialpoints[i];
}

const VPoint* SequentialLineSet::GetPoint(int i)const
{
	return sequentialpoints[i];
}

VPoint SequentialLineSet::GetMin () const
{
	return (sequentialpoints.GetMin() );
}

VPoint SequentialLineSet::GetMax () const
{
	return (sequentialpoints.GetMax() );
}

void SequentialLineSet::Serialize (Bofstream &outfile)const
{
	sequentialpoints.Serialize(outfile);
}
	
void SequentialLineSet::Serialize (Bifstream &infile)
{
	sequentialpoints.Serialize(infile);
}

void SequentialLineSet::Transform(const TransformationMatrix &tm)
{
	sequentialpoints.Transform(tm);
}

PointSet & SequentialLineSet::GetPointSet()
{
	return sequentialpoints;
}

const PointSet & SequentialLineSet::GetPointSet()const
{
	return sequentialpoints;
}

bool SequentialLineSet::operator == (const SequentialLineSet &toCompare)const
{
	if (this==&toCompare)
		return true;

	if(sequentialpoints!=toCompare.sequentialpoints)
		return false;

	return true;
}

bool SequentialLineSet::operator != (const SequentialLineSet &toCompare)const
{
	return !(this->operator ==(toCompare));
}

vreal SequentialLineSet::GetLength()const
{
	//to be implemented...
	vreal totalLength = 0.0;
	int lineCount = Size();
	for (int i=0; i<lineCount; i++)
	{
		VLine currLine = this->operator [](i);
		vreal currLength = currLine.GetLength();
		totalLength += currLength;
	}
	return totalLength;
}	

VPoint SequentialLineSet::GetP (const vreal &t)const
{
//	RemakeAndFillGetPInfoIf();
//	vreal setLength = GetLength();
//	vreal totalL = 0.0;
//	int mysize = Size();
//	vreal p0 = 0.0;
//	vreal p1 = 0.0;
//	for (int i=0;i<mysize;i++)
//	{
//		VLine currL = operator[](i);
//		totalL += currL.GetLength();
//		p1 = totalL / setLength;
//		if (t>=p0-1.e-8 && t<=p1+1.e-8)
//		{
//			VPoint myp = currL.GetP1() + currL.GetV() * ((t - p0) / (p1 - p0));
//			return myp;
//		}
//		p0 = p1;
//	}
//	throw VException (( IDS_SeqGetTExc),1);
	int linei;
	vreal lenUpToLineStart;
	return SequentialLineSet::GetP(*this, this->GetLength(),t, linei, lenUpToLineStart);
}
VPoint SequentialLineSet::GetP (const vreal &t , const vreal &setLength , int &sli)const
{
//	RemakeAndFillGetPInfoIf();
//	vreal totalL = 0.0;
//	int mysize = Size();
//	vreal p0 = 0.0;
//	vreal p1 = 0.0;
//	for (int i=0;i<mysize;i++)
//	{
//		VLine currL = operator[](i);
//		totalL += currL.GetLength();
//		p1 = totalL / setLength;
//		if (t>=p0-1.e-8 && t<=p1+1.e-8)
//		{
//			sli = i;
//			VPoint myp = currL.GetP1() + currL.GetV() * ((t - p0) / (p1 - p0));
//			return myp;
//		}
//		p0 = p1;
//	}
//	throw VException (( IDS_SeqGetTExc),1);
	vreal lenUpToLineStart;
	return GetP(*this, setLength, t,sli, lenUpToLineStart);
}

std::map<const vpVec*, const SequentialLineSet::SLParametricInfo*> *SequentialLineSet::m_parametricInfoMap2 = new std::map<const vpVec*, const SequentialLineSet::SLParametricInfo*>();

VPoint SequentialLineSet::GetP (const std::vector<VPoint> &seq , double &p0 , double &startL , int &starti , const vreal &t , const vreal &setLengthInv , double* mypar1, VPoint* tn)
{
//	RemakeAndFillGetPInfoIf();
//	vreal totalL = startL;
//	int mysize = (int)seq.size()-1;
//	//vreal p0 = 0.0;
//	vreal p1 = p0;
//	for (int i=starti;i<mysize;i++)
//	{
//		VLine currL (seq.at(i),seq.at(i+1));
//		if (tn)
//		{
//			*tn = currL.GetV().Unit();
//		}
//		totalL += currL.GetLength();
//		p1 = totalL * setLengthInv;
//		if (mypar1)
//		{
//			*mypar1 = p1;
//		}
//		if (t>=p0-1.e-8 && t<=p1+1.e-8)
//		{
//			totalL -= currL.GetLength();
//			VPoint vecOnCurLine = currL.GetV() * ((t - p0) / (p1 - p0));
//			VPoint myp = currL.GetP1() + vecOnCurLine;
//			totalL += vecOnCurLine.GetLength();
//			return myp;
//		}
//		p0 = p1;
//		starti = i+1;
//		startL = totalL;
//	}
//	throw VException ( IDS_SeqSeqGetTExc,1);
	auto infoi = m_parametricInfoMap2->find(&seq);
	const SLParametricInfo* parinfo = NULL;
	if (infoi == m_parametricInfoMap2->end())
	{
		vreal seqLen = 1. / setLengthInv;
		parinfo = new SLParametricInfo(seq, seqLen);
		m_parametricInfoMap2->operator[](&seq) = parinfo;
	}
	else
	{
		parinfo = infoi->second;
	}
	vreal lenForT = t * parinfo->GetSeqLength();
	std::pair<vreal, VLine> solut = parinfo->GetLineForDistance(lenForT,starti);
	startL = solut.first;
	p0 = startL * setLengthInv;

	vreal lenonline = lenForT - startL;
	vreal linelen = solut.second.GetLength();
	vreal paronline = lenonline / linelen;
	VPoint curp = solut.second.GetP(paronline);
	if (mypar1 != 0)
	{
		*mypar1 = (startL + linelen) * setLengthInv;
	}
	if (tn != 0)
	{
		*tn = solut.second.GetV().Unit();
	}
	return curp;
}

const VPoint * SequentialLineSet::Start()const
{
	return 	GetPoint(0);
}
	
const VPoint * SequentialLineSet::End()const
{
	return GetPoint (GetPointSize()-1);
}
VPoint * SequentialLineSet::Start()
{
	return 	GetPoint(0);
}
	
VPoint * SequentialLineSet::End()
{
	return GetPoint (GetPointSize()-1);
}

void SequentialLineSet::Reverse()
{
	int mysize = GetPointSize();
	SequentialLineSet tmp (mysize);
	for (int i=mysize-1; i>=0; i--)
	{
		const VPoint *currp = GetPoint (i);
		tmp.InsertPoint (*currp);
	}
	*this = tmp;
}
	
void SequentialLineSet::AddBack (const SequentialLineSet &toAdd)
{
	int mysize = toAdd.GetPointSize();
	for(int i=0;i<mysize;i++)
	{
		const VPoint *currp = toAdd.GetPoint(i);
		InsertPoint (*currp);
	}
}

void SequentialLineSet::Reserve(int newcapacity)
{
	sequentialpoints.Reserve (newcapacity);
}

bool SequentialLineSet::GetPlaneIfPlanar(VPlane &p,double tol,bool &isclosed , bool &isccw)const
{
	// check if closed 
	if (this->Size() == 0)
		throw VException (( IDS_SeqGetExcSetIsE),1);
	if (Start()->Compare(*End() , tol))
		isclosed = true;
	else
		isclosed = false;
	
	int psize = GetPointSize();
	if (psize < 2)
		throw VException (( IDS_SeqGetExcTheIsN),1);
	const VPoint * p1 = GetPoint(0);
	const VPoint * p2 = GetPoint(1);

	if (psize == 2)
	{
		VLine l (*p1,*p2);
		VVector dirx = l.GetV().Unit();
		VVector v1 = dirx ^ UY;
		VVector v2 = dirx ^ UX;
		if (v1.GetLength2() > v2.GetLength2())
			p.normal = v1.Unit();
		else
			p.normal = v2.Unit();
		p.pointonplane = l.GetP1();
		return true;
	}

	int counter = 2;
	bool planeset(false);
	while (counter < psize && planeset == false)
	{
		const VPoint *p3 = GetPoint(counter);
		if (VPoint::IsColinear (*p1,*p2,*p3,tol) == false)
		{
			VTriangleE t(*p1,*p2,*p3);
			p.normal = t.GetNormal();
			p.pointonplane = *p1;
			planeset = true;
		}
		++counter;
	}

	if (planeset == false)
		return false;

	bool ck = IsPointsClose(p,tol);

	if (ck && isclosed)
	{
		// Check if ccw or not 
		CoordinateSystem cs = p.GetCoordSys();
		if (this->Size() == 0)
			throw VException (( IDS_SeqGetExcSetIsE),1);
		VLine l = this->operator[](0);
		VPoint lp1 = cs.AbsoluteCoordToRelativeCoord(l.GetP1());
		VPoint lp2 = cs.AbsoluteCoordToRelativeCoord(l.GetP2());
		VPoint lpm = cs.AbsoluteCoordToRelativeCoord(l.GetP(0.5));
		VLine2d l2d (VPoint2d (lp1.x(),lp1.y()) , VPoint2d (lp2.x() , lp2.y()) );
		VPoint2d tmp = VPoint2d (lpm.x() , lpm.y()) + l2d.GetNormal() * 0.04;
		VPoint ppp = cs.RelativeCoordToAbsoluteCoord(VPoint (tmp.x() , tmp.y() , 0.0));
		VPoint outp = this->GetMax();
		outp += VPoint (100.0,100.0,100.0);
		outp = p.ProjectPoint(outp);
		isccw = LoopLineInt::IsPointInLoop(ppp,this,outp , cs);
	}
	return ck;
}

bool SequentialLineSet::IsPointsClose (const VPlane &p , double tol)const
{
	double tol2 = tol * tol;
	int psize = GetPointSize();
	// now check if all points on sl is close to plane within tolerance
	for (int i=0;i<psize;++i)
	{
		const VPoint * cp = GetPoint(i);
		VPoint pp = p.ProjectPoint(*cp);
		if ((pp- *cp).GetLength2() > tol2 )
			return false;
	}
	return true;
}


double SequentialLineSet::GetT(const VPoint &pointOnSl, int ni , double slLength)const
{
	int mysize = Size();
	if (ni == mysize && pointOnSl.Compare(*this->End(),1.e-6))
		return 1.;
	if (ni < 0 || ni >= mysize)
		throw VException (( IDS_SeqGetExcNiIsNo),1);

	// calculate length from begining to pointOnSl
	vreal totalL = 0.0;
	for (int i = 0; i<ni; ++i)
	{
		VLine l = this->operator [] (i);
		totalL += l.GetLength();
	}
	VLine ll = this->operator [] (ni);
	double tt = ll.GetT (pointOnSl);
	totalL += (ll.GetLength() * tt);
	double myres = totalL / slLength;
	return myres;
}

int SequentialLineSet::GetNi (const VPoint &pointOnSl , double compTol)const
{
	int mysize = GetPointSize();
	for (int i=0;i<mysize;++i)
	{
		const VPoint *p = GetPoint (i);
		if (p->Compare (pointOnSl , compTol))
		{
			if (i == mysize -1)
				i -= 1;		// this i is equal to point no , but the client of this functions expect line number!
			return i;
		}
	}
	return -1;
}

VPoint SequentialLineSet::GetMaxPointInCs (const CoordinateSystem &cs)const
{
	VPoint mx (MIN_NUMBER_LIMIT);
	int mysize = this->GetPointSize();
	for (int i=0;i<mysize;++i)
	{
		const VPoint *p = GetPoint (i);
		mx.ReplaceMax (cs.AbsoluteCoordToRelativeCoord(*p));
	}
	return mx;
}


VPoint SequentialLineSet::GetMinPointInCs (const CoordinateSystem &cs)const
{
	VPoint mn (MAX_NUMBER_LIMIT);
	int mysize = GetPointSize();
	for (int i=0;i<mysize;++i)
	{
		const VPoint *p = GetPoint (i);
		mn.ReplaceMin (cs.AbsoluteCoordToRelativeCoord(*p));
	} 
	return mn;
}

void SequentialLineSet::RemoveCoincidentWithinTol (double tol)
{
	int mysize = GetPointSize();
	if (mysize < 2)
	{
		LogFileWriter::WriteUnknownExceptionLog (( IDS_SeqRemLesThaPoi));
		return;
	}
	SequentialLineSet tmp (mysize);
	const VPoint *p0 = GetPoint (0);
	tmp.InsertPoint (*p0);
	for (int i=1;i<mysize;++i)
	{
		const VPoint *p1 = GetPoint (i);
		if (p1->Compare (*p0,tol) == false)
		{
			tmp.InsertPoint(*p1);
			p0 = p1;
		}
	}
	(*this) = tmp;
}

VLine SequentialLineSet::EndLine ()const
{
	return this->operator[] (this->Size()-1);
}
bool SequentialLineSet::GetSubSl(vreal t0, vreal t1, SequentialLineSet &sl) const
{
	if (t0 < 0.0 || t0 > 1.0 || t1 < 0.0 || t1 > 1.0 || t1 < t0)
		return false;

	vreal len = GetLength();
	int begin = 0;
	int end = 0;
	sl.InsertPoint(GetP(t0, len, begin));
	VPoint endP = GetP(t1, len, end);


	int pSize = GetPointSize();
	if (begin < 0 || begin >= pSize || end < 0 || end > pSize)
		return false;

	for (int i = begin + 1; i <= end; i++)
		sl.InsertPoint(*GetPoint(i));

	sl.InsertPoint(endP);
	return true;
}
void SequentialLineSet::GetNearestPoint(const VPoint &p1, VPoint &pointOnSet, int *ni) const
{
	SequentialLineSetPointMinDist dist(*this);
	dist.MinDist(p1, pointOnSet, ni);
}
void SequentialLineSet::DigitiseByLengthStep (SequentialLineSet &sl, vreal step, vreal t0, vreal t1) const
{
	double lenUpToNow = 0;
	int iUpToNow = 0;
	double par0 = 0.;
	double invSeqLen = 1. / GetLength();
	double deltat = step * invSeqLen;
	const std::vector <VPoint>& vpVec = GetPointSet().GetVec();

	for (double parT = t0; parT <= t1; parT += deltat)
	{
		VPoint curp = GetP(vpVec, par0, lenUpToNow, iUpToNow, parT, invSeqLen);
		sl.InsertPoint(curp);
	}
}
void SequentialLineSet::DigitiseByLengthStep(vreal thisSlLength, vreal step, vreal t0, vreal t1 , std::vector<std::pair<vreal,VPoint>> &orderedLenPointPairs) const
{
	double lenUpToNow = 0;
	int iUpToNow = 0;
	double par0 = 0.;
	double invSeqLen = 1. / thisSlLength;
	double deltat = step * invSeqLen;
	const std::vector <VPoint>& vpVec = GetPointSet().GetVec();
	//std::vector<std::pair<vreal, VPoint>> orderedLenPointPairs2;
	//int linei2;
	//vreal hitlineStartLen;
	//vreal thislen = this->GetLength();
	for (double parT = t0; parT <= t1 + 1.e-6; parT += deltat)
	{
		VPoint curp = GetP(parT);//GetP(vpVec, par0, lenUpToNow, iUpToNow, parT, invSeqLen);
		orderedLenPointPairs.push_back(std::pair<vreal, VPoint>(parT*thisSlLength, curp));
		//VPoint curp2 = this->GetP(*this, thislen, parT, linei2, hitlineStartLen);
		//orderedLenPointPairs2.push_back(std::pair<vreal,VPoint>(hitlineStartLen,curp2));
	}
}
void SequentialLineSet::DigitiseByLengthStepReverse (SequentialLineSet &sl, vreal step, vreal t1, vreal t0) const
{
	SequentialLineSet slReverse(sl.Size());
	DigitiseByLengthStep(slReverse, step, t0, t1);
	slReverse.Reverse();
	sl.AddBack(slReverse);
}

//bool SequentialLineSet::GetSubSlRich(vreal t0, vreal t1, SequentialLineSet &sl, vreal tol) const
//{
//	if (t0 < 0.0 || t0 > 1.0 || t1 < 0.0 || t1 > 1.0 || t1 < t0 || tol <= 0)
//		return false;
//	
//	vreal len = GetLength();
//	int begin = 0;
//	int end = 0;
//	sl.InsertPoint(GetP(t0, len, begin));
//	VPoint endP = GetP(t1, len, end);
//
//
//	int pSize = GetPointSize();
//	if (begin < 0 || begin >= pSize || end < 0 || end > pSize)
//		return false;
//
//	for (int i = begin + 1; i <= end; i++)
//	{
//		const VPoint *pPrev = GetPoint(i - 1);
//		const VPoint *p = GetPoint(i);
//
//		VLine ln(*pPrev, *p);
//		vreal len = ln.GetLength();
//		vreal par = tol / len;
//		vreal tempPar = 0;
//		while (len > tol)
//		{
//			VPoint p1 = ln.GetP(tempPar);
//			sl.InsertPoint(p1);
//			len = VLine(p1, *p).GetLength();
//			tempPar += par;
//		}
//		sl.InsertPoint(*p);
//	}
//	sl.InsertPoint(endP);
//	return true;
//}
bool SequentialLineSet::GetSubSlRich(vreal t0, vreal t1, SequentialLineSet &sl, vreal tol) const
{
	if (t0 < 0.0 || t0 > 1.0 || t1 < 0.0 || t1 > 1.0 || t1 <= t0 || tol <= 0)
		throw VException(IDS_SeqGetExcTOrTOu, 1);

	vreal thislen = GetLength();
	int begin = 0;
	int end = 0;
	//sl.InsertPoint(GetP(t0, thislen, begin));
	VPoint endP = GetP(t1, thislen, end);


	int pSize = GetPointSize();
	++end;
	if (begin < 0 || begin >= pSize || end < 0 || end >= pSize)
		throw VException(IDS_SeqGetExcBegOrE, 1);
	VPoint prevpoint = GetP(t0, thislen, begin);
	sl.InsertPoint(prevpoint);
	for (int i = begin + 1; i <= end; i++)
	{
		//const VPoint *pPrev = GetPoint(i - 1);
		VPoint p = *GetPoint(i);
		if (i == end)
			p = endP;

		//VLine ln(*pPrev, p);
		VLine ln(prevpoint, p);
		vreal len = ln.GetLength();
		vreal par = tol / len;
		vreal tempPar = 0;
		while (len > tol)
		{
			VPoint p1 = ln.GetP(tempPar);
			sl.InsertPoint(p1);
			len = VLine(p1, p).GetLength();
			tempPar += par;
		}
		sl.InsertPoint(p);
		prevpoint = p;
	}

	//sl.InsertPoint(endP);
	return true;
}

double SequentialLineSet::GetT (const VPoint &pointOnSl , double slLength) const
{
	int mysize = Size();

	// calculate length from begining to pointOnSl
	vreal totalL = 0.0;
	vreal tol = VGlobalValues::TENMICRONS_TO_VUNIT;
	vreal mypar;
	for (int i = 0; i< mysize; ++i)
	{
		VLine l = this->operator [] (i);
		if (l.IsOnLine(pointOnSl,mypar,tol))
		{
			double tt = l.GetT (pointOnSl);
			totalL += (l.GetLength() * tt);
			return totalL / slLength;
		}
		totalL += l.GetLength();
	}

	throw VException( IDS_SeqGet, 1);
}
bool SequentialLineSet::GetLineOrArcVec(vreal t0, vreal t1, std::vector<LineOrArc> &result, vreal stepTol, vreal mytol) const
{
	if (t0 < 0.0 || t0 > 1.0 || t1 < 0.0 || t1 > 1.0 || t1 < t0 || stepTol <= 0)
		return false;

	vreal thislen = GetLength();
	int begin = 0;
	int end = 0;
	VPoint endP = GetP(t1, thislen, end);


	int pSize = GetPointSize();
	if (begin < 0 || begin >= pSize || end < 0 || end > pSize)
		return false;

	++end;
	SequentialLineSet sl(1000);
	for (int i = begin + 1; i <= end; i++)
	{
		const VPoint* pPrev = GetPoint(i - 1);
		VPoint p = *GetPoint(i);
		if (i == end)
			p = endP;

		VLine ln(*pPrev, p);
		vreal len = ln.GetLength();
		vreal par = stepTol / len;
		vreal tempPar = 0;
		while (len > stepTol && par < 1.0)
		{
			VPoint p1 = ln.GetP(tempPar);
			sl.InsertPoint(p1);
			len = VLine(p1, p).GetLength();
			tempPar += par;
		}
		sl.InsertPoint(p);
		if (len > mytol * 10)
		{
			ArcFit arcfit;
			arcfit.Fit(sl.GetPointSet().GetVec(), mytol, result);
			sl = SequentialLineSet(1000);
		}
	}

	//sl.InsertPoint(endP);
	return true;
}

void SequentialLineSet::PrepareForArcFit (std::vector<SequentialLineSet> &slVec , vreal sharpAngleThreshold , vreal minDistanceStep)const
{
	int pSize = GetPointSize();
	if (pSize < 2)
		return;
	vreal cosAngleLimit = cos (sharpAngleThreshold * DEGREE_TO_RAD);
	const VPoint *prevPoint = GetPoint(0);
	slVec.push_back (SequentialLineSet(10));
	slVec.back().InsertPoint (*prevPoint);
	for (int i=1;i<pSize;++i)
	{
		const VPoint *curPoint = GetPoint(i);
		bool isSharpCorner = false;
		if (slVec.back().GetPointSize() > 1)
		{
			VPoint *prevprevPoint = slVec.back().GetPoint(slVec.back().GetPointSize()-2);
			isSharpCorner = VPoint::IsSharpCorner (*prevprevPoint,*prevPoint,*curPoint,cosAngleLimit);
		}
		if (isSharpCorner)
		{
			slVec.push_back (SequentialLineSet(10));
			slVec.back().InsertPoint(*prevPoint);
		}
		
		VLine ln (*prevPoint,*curPoint);
		vreal len = ln.GetLength();
		vreal par = minDistanceStep / len;
		vreal tempPar = par;
		while (len > minDistanceStep)
		{
			VPoint p1 = ln.GetP(tempPar);
			slVec.back().InsertPoint(p1);
			len = VLine(p1, *curPoint).GetLength();
			tempPar += par;
		}
		slVec.back().InsertPoint(*curPoint);
	
		prevPoint = curPoint;
	}
}
void SequentialLineSet::PrepareForArcFitNoSharpCornerCheck (std::vector<SequentialLineSet> &slVec , vreal minDistanceStep)const
{
	int pSize = GetPointSize();
	if (pSize < 2)
		return;
	const VPoint *prevPoint = GetPoint(0);
	slVec.push_back (SequentialLineSet(10));
	slVec.back().InsertPoint (*prevPoint);
	for (int i=1;i<pSize;++i)
	{
		const VPoint *curPoint = GetPoint(i);
		bool isSharpCorner = false;
		if (slVec.back().GetPointSize() > 1)
		{
			VPoint *prevprevPoint = slVec.back().GetPoint(slVec.back().GetPointSize()-2);
			//isSharpCorner = VPoint::IsSharpCorner (*prevprevPoint,*prevPoint,*curPoint,cosAngleLimit);
		}
		//if (isSharpCorner)
		//{
		//	slVec.push_back (SequentialLineSet(10));
		//	slVec.back().InsertPoint(*prevPoint);
		//}
		
		VLine ln (*prevPoint,*curPoint);
		vreal len = ln.GetLength();
		vreal par = minDistanceStep / len;
		vreal tempPar = par;
		while (len > minDistanceStep)
		{
			VPoint p1 = ln.GetP(tempPar);
			slVec.back().InsertPoint(p1);
			len = VLine(p1, *curPoint).GetLength();
			tempPar += par;
		}
		slVec.back().InsertPoint(*curPoint);
	
		prevPoint = curPoint;
	}
}

std::map<vreal,vreal> SequentialLineSet::Intersection (const VLine &l)const
{
	std::map<vreal,vreal> res;
	int mysize = Size();
	VPoint ip1,ip2;
	vreal i1,i2;
	for (int i=0;i<mysize;++i)
	{
		VLine curLine = this->operator[](i);
		curLine.InfiniteLinesInt (l , ip1 , i1 , ip2 , i2);
		if (i1<=1.0 && i1>=0.0 && i2 >= 0.0)
		{
			res[i2] = i2;
		}
	}
	return res;
}

bool SequentialLineSet::FitLineOrArcs (vreal sharpAngleThresholdInDegrees , vreal minDistanceStep,std::vector<LineOrArc> &res , const CoordinateSystem &ws , vreal tol , int useMinDistanceStep)
{
	try
	{
		//vreal sharpAngleThreshold = 10.0;
		//vreal minDistanceStep = tol * 20.0;
		std::vector<SequentialLineSet> slVec;
		SequentialLineSet::PrepareForArcFit(slVec,sharpAngleThresholdInDegrees,minDistanceStep);
		size_t mySlSize = slVec.size();
		for (size_t k=0;k<mySlSize;++k)
		{
			std::vector<VPoint> pres;
			SequentialLineSet::ProjectPointsToPlane(slVec.at(k),ws.GetDirz(),ws.GetOrigin(),pres,tol,false);
			ArcFit afit;
			afit.FitTr(pres,tol,res,ws);
		}
		return true;
	}
	catch (VException &e)
	{
		LogFileWriter::WriteExceptionLog (e,"SequentialLineSet::FitLineOrArcs");
		return false;
	}
	catch (std::exception &e)
	{
		LogFileWriter::WriteExceptionLog (e,"SequentialLineSet::FitLineOrArcs");
		return false;
	}
	catch (...)
	{
		LogFileWriter::WriteUnknownExceptionLog ("Unknown exception ","SequentialLineSet::FitLineOrArcs");
		return false;
	}
}

void SequentialLineSet::ProjectPointsToPlane (const SequentialLineSet &sl , const VPoint &pnormal, const VPoint &pp , std::vector<VPoint> &res,vreal tol , bool refinepoints)
{
	const std::vector<VPoint> &src = sl.GetPointSet().GetVec();
	std::vector<VPoint>refvec;
	const std::vector<VPoint> *tosend;

	if (refinepoints)
	{
		RefinePoints(src,refvec,tol);
		tosend = &refvec;
	}
	else
	{
		tosend = &src;
	}

	size_t mysize = tosend->size();
	for (size_t i=0; i<mysize; ++i)
	{
		const VPoint &sp = (*tosend)[i];
		//  (sp + n t) - pp ) % n = 0  ->   sp % n + t - pp % n = 0 -> t = pp - sp ) % n
		double  t = (pp-sp) % pnormal;
		VPoint prjp = sp + (pnormal * t);
		res.push_back(prjp);
	}
}
void SequentialLineSet::RefinePoints (const std::vector<VPoint> &src , std::vector<VPoint> &res , double trsh)
{
	if (src.size() < 2)
		return;
	VPoint pp = src.front();
	res.push_back(pp);

	size_t mysize = src.size();
	for (size_t i=1;i<mysize;++i)
	{
		VPoint cp = src[i];
		VPoint v = cp - pp;
		double len = v.GetLength();
		if ( len > trsh)
		{
			double parstep = trsh / len;
			VLine l (pp , cp);
			for (double par = parstep; par < 1.; par+=parstep)
			{
				res.push_back(l.GetP(par));
			}
			res.push_back(cp);
		}
		else
		{
			res.push_back (cp);
		}
		pp = cp;
	}
}

bool SequentialLineSet::IsClosed(vreal tol)const
{
	if (GetPointSize() > 0)
	{
		return Start()->Compare (*End() , tol);
	}
	else
	{
		LogFileWriter::WriteUnknownExceptionLog("SequentialLineSet::IsClosedException no points");
		return false;
	}
}

void SequentialLineSet::GetMinMaxAlongCsX (const CoordinateSystem &cs , std::map<vreal,int> &mni , std::map<vreal,int> &mxi,vreal coinTol)const
{
	VPoint mnp (MAX_NUMBER_LIMIT);
	VPoint mxp (MIN_NUMBER_LIMIT);
	vreal halfCoinTol = coinTol * 0.5;
	int psize = GetPointSize();
	for (int i=0;i<psize;++i)
	{
		const VPoint *curp = GetPoint(i);
		VPoint tp = cs.AbsoluteCoordToRelativeCoord (*curp);
		if (tp.x() < (mnp.x()+halfCoinTol))
		{
			if (mnp.x() - tp.x() > coinTol)
			{
				mni.clear();
			}
			mnp = tp;
			mni[tp.y()] = i;
		}
		if ((tp.x()+halfCoinTol) > mxp.x())
		{
			if (tp.x() - mxp.x() > coinTol)
			{
				mxi.clear();
			}
			mxp = tp;
			mxi[tp.y()] = i;
		}
	}
}

void SequentialLineSet::GetConvexHull (vreal angleStep , std::map<vreal,int> &angleIndexMap , vreal coinTol)const
{
	for (vreal angle = 0.0; angle<180.0;angle += angleStep)
	{
		CoordinateSystem cs;
		RotationMatrix R (VPoint(0.0),UZ,angle);
		cs.Transform (R);
		std::map<vreal,int> mni,mxi;
		GetMinMaxAlongCsX (cs,mni,mxi,coinTol);
		std::map<vreal,int>::const_iterator i = mni.begin();
		std::map<vreal,int>::const_iterator e = mni.end();
		for (i;i!=e;++i)
		{
			angleIndexMap[angle-180.0] = i->second;
		}
		std::map<vreal,int>::const_iterator ii = mxi.begin();
		std::map<vreal,int>::const_iterator ee = mxi.end();
		for (ii;ii!=ee;++ii)
		{
			angleIndexMap[angle] = ii->second;
		}
	}
}

void SequentialLineSet::GetPointsBetween (int i0,int i1,SequentialLineSet &subSl,vreal tol)const
{
	int j0=i0;
	int j1=i1;
	if (this->IsClosed(tol))
	{
		if (j0>j1)
		{
			int tmp = j0;
			j0 = j1;
			j1 = tmp;
		}
		int psize = this->GetPointSize();
		int d1 = j1 - j0;
		int d2 = psize - j1;
		d2 += j0;
		if (d2 < d1)
		{
			for (int j=j1;j<psize;++j)
				subSl.InsertPoint (*this->GetPoint(j));
			for (int w=0;w<j0;++w)
				subSl.InsertPoint(*this->GetPoint(w));
		}
		else
		{
			for (int k=j0;k<=j1;++k)
				subSl.InsertPoint (*this->GetPoint(k));
		}
	}
	else
	{
		if (j0>j1)
		{
			int tmp = j0;
			j0 = j1;
			j1 = tmp;
		}
		for (int k=j0;k<=j1;++k)
			subSl.InsertPoint(*this->GetPoint(k));
	}
}

void SequentialLineSet::GetPointsBetween2 (int i0 , int i1 , SequentialLineSet &subSl , vreal closedTol)const
{
	int j0 (i0);
	int j1 (i1);
	int psize = GetPointSize();

	if (j1 < j0)
	{
		int j11 = j1 + psize;
		int pcount1 = j0 - j1;
		int pcount2 = j11 - j0;
		//if (IsClosed(closedTol))
		//{
		//	j1 += psize;
		//}
		//else
		//{
		//	int tmp (j0);
		//	j0 = j1;
		//	j1 = tmp;
		//}
		if (pcount1 < pcount2)
		{
			for (int i = j0; i >= j1; --i)
			{
				subSl.InsertPoint(*GetPoint(i));
			}
		}
		else
		{
			for (int i = j0; i <= j11; ++i)
			{
				subSl.InsertPoint(*GetPoint(i%psize));
			}
		}
	}
	else
	{
		int j00 = j0 + psize;
		int pcount1 = j1 - j0;
		int pcount2 = j00 - j1;
		if (pcount1 < pcount2)
		{
			for (int i = j0; i <= j1; ++i)
			{
				subSl.InsertPoint(*GetPoint(i));
			}
		}
		else
		{
			for (int i = j00; i >= j1; --i)
			{
				subSl.InsertPoint(*GetPoint(i%psize));
			}
		}
	}

}

bool SequentialLineSet::IsCCWOnXY ()const
{
	//SequentialLineSet2d sltmp (*this);
	//SequentialLineSet2d *sl = &sltmp;
	//PosCubes2d tmpCubes (sltmp);
	//PosCubes2d *cubes = &tmpCubes;
	//VLine2dE l1 (sl->operator [] ((int)(sl->Size()*0.5)));
	//VLine2dE l2 (sl->operator [] (0));
	//VLine2dE l3 (sl->operator [] (sl->Size()-1));
	//vreal d1 = l1.GetV().GetLength();
	//vreal d2 = l2.GetV().GetLength();
	//vreal d3 = l3.GetV().GetLength();
	//VLine2dE l;
	//if (d1 >= d2 && d1 >= d3)
	//	l = l1;
	//if (d2 >= d1 && d2 >= d3)
	//	l = l2;
	//if (d3 >= d1 && d3 >= d2)
	//	l = l3;
	//VPoint2d outerP = sltmp.GetMin();
	//outerP -= VPoint2d(1.02, 1.03);
	//if ( LoopLineInt::IsPointInLoop ( l.GetP(0.5)+l.N()*0.0001 , sl , cubes))
	//{
	//	// line normal is pointing out of loop so CCW
	//	return true;
	//}	
	//else
	//	return false;

	VLine l1(this->operator [] ((int)(this->Size()*0.5)));
	VLine l2(this->operator [] (0));
	VLine l3(this->operator [] (this->Size() - 1));
	vreal d1 = l1.GetV().GetLength();
	vreal d2 = l2.GetV().GetLength();
	vreal d3 = l3.GetV().GetLength();
	VLine l;
	if (d1 >= d2 && d1 >= d3)
		l = l1;
	if (d2 >= d1 && d2 >= d3)
		l = l2;
	if (d3 >= d1 && d3 >= d2)
		l = l3;
	VPoint N = l.GetNormalOnXYPlane();
	VPoint outp = GetMin() - VPoint(1.02, 1.03, 0.0);

	if (LoopLineInt::IsPointInLoop(l.GetP(0.5) + N * 0.0001, this, outp))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// start to search int from starti if finds intersection and adds to ipVec
// there may be more than one intersection , return value is the nearest point index to plane point
// if there is no intersection return value is -1
int SequentialLineSet::GetInt (const VPlane &pln , std::map<int,vreal> &indexParMap , int starti)const
{
	vreal minDist = MAX_NUMBER_LIMIT;
	int resi = -1;
	int lsize = Size();
	for (int i=starti; i<lsize;++i)
	{
		VLine curLine = this->operator[](i);
		vreal t;
		if (pln.Int(curLine,t) == VPlane::REGULAR)
		{
			if ( t < 1.0 + EPSILON && t > -EPSILON)
			{
				indexParMap[i] = t;
				VPoint ip = curLine.GetP(t);
				vreal dist = (ip-pln.pointonplane).GetLength();
				if (dist < minDist)
				{
					resi = i;
				}
			}
		}
	}
	return resi;
}

void SequentialLineSet::ReorderIfClosed (const VPoint &newStartp , int pi , vreal tol)
{
	if (this->IsClosed(tol) == false)
		return;
	int psize = GetPointSize();
	VLine l ( *GetPoint(pi) , *GetPoint ((pi+1) % psize));
	vreal t = l.GetT(newStartp);
	if (t < -EPSILON || t > 1.0 + EPSILON)
		throw VException ("pi is not correct! SequentialLineSet::ReorderIfClosed",1);
	SequentialLineSet newSet (psize+1);
	newSet.InsertPoint (newStartp);
	for (int i=pi+1;i<psize;++i)
	{
		newSet.InsertPoint (*GetPoint(i));
	}
	for (int i=0;i<=pi;++i)
	{
		newSet.InsertPoint(*GetPoint(i));
	}
	newSet.InsertPoint(newStartp);
	*this = newSet;
}

bool SequentialLineSet::IsSharpCorner (int pi,vreal cosSharpAngleThreshold)const
{
	if (pi < 1 || pi > GetPointSize()-1)
		return false;
	const VPoint * prevp = GetPoint (pi - 1);
	const VPoint *p = GetPoint(pi);
	const VPoint *nextp = GetPoint (pi+1);
	VPoint v0 = *p - *prevp;
	VPoint v1 = *nextp - *p;
	vreal ck = v1 % v0;
	if (ck < cosSharpAngleThreshold)
		return true;
	else
		return false;
}

void SequentialLineSet::PlanarPairWalkIfClosed ( SequentialLineSet *sl 
												, vreal maxLenChangeRatio 
												, vreal cosSharpAngleThreshold 
												, std::list<std::pair<VPoint2d , VLine>> &myParxSlParyLineMap 
												, vreal tol)
{
	if (this->IsClosed(tol) == false || sl->IsClosed(tol) == false)
		return;
	
	bool thisccw = this->IsCCWOnXY();
	if (thisccw == false)
		this->Reverse();
	bool slccw = sl->IsCCWOnXY();
	if (slccw == false)
		sl->Reverse();

	//LineSetsMinDist dcalc;
	vreal mind1;
	VLine minl1;
	int sli1,sli2;
	LineSetsMinDist::Calc (*this , *sl , mind1 , minl1,&sli1,&sli2);

	this->ReorderIfClosed (minl1.GetP1(),sli1,tol);
	sl->ReorderIfClosed (minl1.GetP2(),sli2,tol);


}

void SequentialLineSet::CleanZOssilationOnXZ (SequentialLineSet &sl , const CoordinateSystem &cs , vreal slopeThreshold)
{

}
void SequentialLineSet::CleanZOssilationOnYZ (SequentialLineSet &sl , const CoordinateSystem &cs , vreal slopeThreshold)
{

}
void SequentialLineSet::BreakBySlopeChangeOnXZ (const SequentialLineSet &sl , const CoordinateSystem &cs , vreal slopeChangeThreshold , vreal sharpCornerThreshold , std::vector<SequentialLineSet *> &slVec)// slopeChange = fabs(dx0/dz0-dxcur/dzcur)  
{
	int lsize = sl.Size();
	if (lsize == 0)
		return;
	//bool steepRegion (false);
	//bool steepRegionPrev(false);
	VLine initiall = sl.operator[](0);
	VLine itl (cs.AbsoluteCoordToRelativeCoord(initiall.GetP1()),cs.AbsoluteCoordToRelativeCoord(initiall.GetP2()));
	VPoint idl = itl.GetV();
	idl.NormThis();
	vreal initialAngle = atan2(idl.z(),idl.x());
	vreal prevAngle = initialAngle;
	SequentialLineSet *cursl = new SequentialLineSet (10);
	cursl->InsertPoint (initiall.GetP1());
	cursl->InsertPoint(initiall.GetP2());
	VPoint pdl = idl;
	for (int li=1;li<lsize;++li)
	{
		VLine curl = sl.operator[](li);
		VLine tl (cs.AbsoluteCoordToRelativeCoord(curl.GetP1()),cs.AbsoluteCoordToRelativeCoord(curl.GetP2()));
		VPoint dl = tl.GetV();
		dl.NormThis();
		vreal dp1 = idl % dl;
		vreal curAngle = acos(dp1);//atan2(dl.z(),dl.x());
		vreal dp2 = pdl % dl;
		vreal curAngle2 = acos(dp2);
		//if (fabs(curAngle-initialAngle) > slopeChangeThreshold || fabs(curAngle-prevAngle) > sharpCornerThreshold )
		if (fabs(curAngle) > slopeChangeThreshold ||  fabs (curAngle2) > sharpCornerThreshold)
		{
			//initialAngle = curAngle;
			//prevAngle = curAngle;
			idl = dl;
			pdl = dl;
			slVec.push_back (cursl);
			cursl = new SequentialLineSet (10);
			cursl->InsertPoint(curl.GetP1());
			cursl->InsertPoint(curl.GetP2());
		}
		else
		{
			cursl->InsertPoint (curl.GetP2());
			//prevAngle = curAngle;
			pdl = dl;
		}
	}
	slVec.push_back (cursl);
}
void SequentialLineSet::BreakBySlopeChangeOnYZ (const SequentialLineSet &sl , const CoordinateSystem &cs , vreal slopeChangeThreshold , vreal sharpCornerThreshold , std::vector<SequentialLineSet *> &slVec)// slopeChange = fabs(dx0/dz0-dxcur/dzcur)  
{
	int lsize = sl.Size();
	if (lsize == 0)
		return;
	bool steepRegion (false);
	bool steepRegionPrev(false);
	VLine initiall = sl.operator[](0);
	VLine itl (cs.AbsoluteCoordToRelativeCoord(initiall.GetP1()),cs.AbsoluteCoordToRelativeCoord(initiall.GetP2()));
	VPoint idl = itl.GetV();
	vreal initialAngle = atan2(idl.z(),idl.y());
	vreal prevAngle = initialAngle;
	SequentialLineSet *cursl = new SequentialLineSet (10);
	cursl->InsertPoint (initiall.GetP1());
	cursl->InsertPoint(initiall.GetP2());
	for (int li=1;li<lsize;++li)
	{
		VLine curl = sl.operator[](li);
		VLine tl (cs.AbsoluteCoordToRelativeCoord(curl.GetP1()),cs.AbsoluteCoordToRelativeCoord(curl.GetP2()));
		VPoint dl = tl.GetV();
		vreal curAngle = atan2(dl.z(),dl.y());
		if (fabs(curAngle-initialAngle) > slopeChangeThreshold || fabs(curAngle-prevAngle) > sharpCornerThreshold )
		{
			initialAngle = curAngle;
			prevAngle = curAngle;
			slVec.push_back (cursl);
			cursl = new SequentialLineSet (10);
			cursl->InsertPoint(curl.GetP1());
			cursl->InsertPoint(curl.GetP2());
		}
		else
		{
			cursl->InsertPoint (curl.GetP2());
			prevAngle = curAngle;
		}
	}
	slVec.push_back (cursl);
}

void SequentialLineSet::GroupBySlopeIntervalsOnCsXY (const CoordinateSystem &cs , std::list<std::pair<VInterval,std::vector<int>>> &slopeIntervals)const
{
	int lsize = Size();
	VPoint zeroPoint(0.0);
	for (int li=0;li<lsize;++li)
	{
		VLine curl = this->operator[](li);
		VLine tl (cs.AbsoluteCoordToRelativeCoord(curl.GetP1()),cs.AbsoluteCoordToRelativeCoord(curl.GetP2()));
		VPoint dl = tl.GetV();
		if (dl.Compare(zeroPoint,EPSILON))
			continue;
		vreal curslope = PolarCalc2d::GetPolarAngle(dl.x(),dl.y()); 
		std::list<std::pair<VInterval,std::vector<int>>>::iterator si = slopeIntervals.begin();
		std::list<std::pair<VInterval,std::vector<int>>>::iterator se = slopeIntervals.end();
		for (si;si!=se;++si)
		{
			if (si->first.IsContaining(curslope) || si->first.IsContaining(curslope-360.0))
			{
				si->second.push_back (li);
				break;
			}
		}
	}
}

void SequentialLineSet::IndexVecToParIntervals (const std::vector<int> &ivec , std::vector<VInterval> &parIntVec)const
{
	std::vector<VInterval> iiVec;
	VInterval::IndexVecToIntervals(ivec,iiVec);
	int isize = (int)iiVec.size();
	vreal slLength = this->GetLength();
	for (int ii=0;ii<isize;++ii)
	{
		VInterval &curi = iiVec.at(ii);
		int i0 = (int)curi.GetT0();
		vreal par0 = GetT( *GetPoint(i0) , i0 , slLength); 
		int i1 = (int)curi.GetT1();
		++i1;// expand the end by one , line index to point index
		vreal par1 = GetT( *GetPoint(i1) , i1 , slLength);
		parIntVec.push_back (VInterval(par0,par1));
	}
}

SequentialLineSet * SequentialLineSet::MakeSlAndModifyZ(const LTable &lt, const CoordinateSystem &cs)const
{
	int psize = GetPointSize();
	if (psize == 0)
		throw VException("SequentialLineSet::MakeSlAndModifyZ exception , this set is empty!",1);
	SequentialLineSet * newSl = new SequentialLineSet(psize);
	const VPoint *prevp = GetPoint(0);
	vreal dz;
	bool ck = lt.LookUp(0.0,dz);
	if (!ck)
		throw VException("SequentialLineSet::MakSlAndModifyZ exception , look up can not find parameter",1);
	VPoint relp0 = cs.AbsoluteCoordToRelativeCoord(*prevp);
	relp0 += UZ * dz;
	vreal totalLen = this->GetLength();
	vreal invLen = 1. / totalLen;
	vreal lenSoFar = 0.0;
	newSl->InsertPoint(cs.RelativeCoordToAbsoluteCoord(relp0));
	for (int pi = 1; pi < psize; ++pi)
	{
		const VPoint *p = GetPoint(pi);
		vreal curLen = (*p - *prevp).GetLength();
		lenSoFar += curLen;
		vreal curPar = lenSoFar * invLen;
		bool ck2 = lt.LookUp(curPar, dz);
		if (!ck2)
			throw VException("SequentialLineSet::MakSlAndModifyZ exception , look up can not find parameter 2", 1);
		VPoint relp = cs.AbsoluteCoordToRelativeCoord(*p);
		relp += UZ * dz;
		newSl->InsertPoint(cs.RelativeCoordToAbsoluteCoord(relp));
		prevp = p;
	}
	return newSl;
}

void SequentialLineSet::MakeSubSl(const std::map<int, int> &points, std::list<SequentialLineSet *> &subSlList)const
{
	std::map<int, int>::const_iterator si = points.begin();
	std::map<int, int>::const_iterator se = points.end();
	int prevli = -2;
	SequentialLineSet *sl = 0;
	for (si; si != se; ++si)
	{
		VLine curl = this->operator[](si->first);
		if ((si->first - prevli) > 1)
		{
			if (sl != 0)
				subSlList.push_back(sl);
			sl = new SequentialLineSet(20);
			sl->InsertPoint(curl.GetP1());
		}
		sl->InsertPoint(curl.GetP2());
		prevli = si->first;
	}
	if (sl)
		subSlList.push_back(sl);
}

void SequentialLineSet::XFillBoundary(const CoordinateSystem &cs, std::map<vreal, std::map<vreal,vreal>> &boundary , vreal tol)const
{
	// for all lines
	int lsize = Size();
	for (int li = 0; li<lsize; ++li)
	{
		VLine curl = this->operator[](li);
		VLine tline(cs.AbsoluteCoordToRelativeCoord(curl.GetP1())
			, cs.AbsoluteCoordToRelativeCoord(curl.GetP2()));
		VPoint lmin = tline.GetMin();
		VPoint lmax = tline.GetMax();
		std::map<vreal, std::map<vreal, vreal>>::iterator xi = boundary.lower_bound(lmin.x());//boundary.begin();
		std::map<vreal, std::map<vreal,vreal>>::iterator xe = boundary.end();
		for (xi; xi != xe; ++xi)
		{
			if (xi->first >= lmin.x() - tol && xi->first <= lmax.x() + tol)
			{
				VPoint origin(lmin);
				origin.x(xi->first);
				VPlane pln(origin, UX);
				vreal intt;
				VPlane::IntType ck = pln.Int(tline, intt);
				if (ck == VPlane::REGULAR)
				{
					if (intt > -EPSILON && intt < 1 + EPSILON)
					{
						VPoint ipoint = tline.GetP(intt);
						std::map<vreal, vreal> &curmap = xi->second;//boundary[xi->first];
						curmap[ipoint.y()] = ipoint.y();
					}
				}
			}
			else
				break;
		}
	}
}

bool SequentialLineSet::IsTurningLine(int index/*, const SequentialLineSet &sls*/)
{
	
	if (index >= this->Size())
	{
		throw new VException("Index out of range, SequentialLineSet::IsTurningLine",1);
	}
	if (index == 0 || index == 1)
		return false;

	int tmpLastIndex = index + 1;
	if (index == this->Size() - 1)
		tmpLastIndex = 0;

	VLine prevL = (*this)[index-1];
	VLine currL = (*this)[index];
	VLine nextL = (*this)[tmpLastIndex];
 
	vreal prevLength = prevL.GetLength();
	vreal currLength = currL.GetLength();
	vreal nextLength = nextL.GetLength();
	if (fabs(prevLength*currLength*nextLength) < EPSILON)
		return false;

	VPoint v1 = prevL.GetV() / prevLength;
	VPoint v2 = currL.GetV() / currLength;
	VPoint v3 = nextL.GetV() / nextLength;
	 
	VPoint prod1 = v1 ^ v2;
	VPoint prod2 = v2 ^ v3;
	 
	vreal prod = prod1.z();//* prod2.z();
	if (prod < 0)
		return true;
	if (fabs(prod) < EPSILON)
	{
		throw new VException("Lines not correct format , prod = 0 SequentialLineSet::IsTurningLine", 1);
	}

	return false; 
}

void SequentialLineSet::YFillBoundary(const CoordinateSystem &cs, std::map<vreal, std::multimap<vreal, vreal>> &boundary, vreal tol)const
{
	// for all lines
	int lsize = Size();
	for (int li = 0; li<lsize; ++li)
	{
		VLine curl = this->operator[](li);
		VLine tline(cs.AbsoluteCoordToRelativeCoord(curl.GetP1())
			, cs.AbsoluteCoordToRelativeCoord(curl.GetP2()));
		VPoint lmin = tline.GetMin();
		VPoint lmax = tline.GetMax();
		std::map<vreal, std::multimap<vreal, vreal>>::iterator yi = boundary.lower_bound(lmin.y());//boundary.begin();
		std::map<vreal, std::multimap<vreal, vreal>>::iterator ye = boundary.end();
		for (yi; yi != ye; ++yi)
		{
			if (yi->first >= lmin.y() - tol && yi->first <= lmax.y() + tol)
			{
				VPoint origin(lmin);
				origin.y(yi->first);
				VPlane pln(origin, UY);
				vreal intt;
				VPlane::IntType ck = pln.Int(tline, intt);
				if (ck == VPlane::REGULAR)
				{
					if (intt > -EPSILON && intt < 1 + EPSILON)
					{
						VPoint ipoint = tline.GetP(intt);
						std::multimap<vreal, vreal> &curmap = yi->second;//boundary[yi->first];
						curmap.insert(std::pair<vreal, vreal>(ipoint.x(), ipoint.x()));
					}
				}
			}
			else
				break;
		}
	}
}
void SequentialLineSet::YFillBoundary(const CoordinateSystem &cs, std::map<vreal, std::map<vreal, vreal>> &boundary, vreal tol)const
{
	// for all lines
	int lsize = Size();
	for (int li = 0; li<lsize; ++li)
	{
		VLine curl = this->operator[](li);
		VLine tline(cs.AbsoluteCoordToRelativeCoord(curl.GetP1())
			, cs.AbsoluteCoordToRelativeCoord(curl.GetP2()));
		VPoint lmin = tline.GetMin();
		VPoint lmax = tline.GetMax();
		std::map<vreal, std::map<vreal, vreal>>::iterator yi = boundary.lower_bound(lmin.y());//boundary.begin();
		std::map<vreal, std::map<vreal, vreal>>::iterator ye = boundary.end();
		for (yi; yi != ye; ++yi)
		{
			if (yi->first >= lmin.y() - tol && yi->first <= lmax.y() + tol)
			{
				VPoint origin(lmin);
				origin.y(yi->first);
				VPlane pln(origin, UY);
				vreal intt;
				VPlane::IntType ck = pln.Int(tline, intt);
				if (ck == VPlane::REGULAR)
				{
					if (intt > -EPSILON && intt < 1 + EPSILON)
					{
						VPoint ipoint = tline.GetP(intt);
						std::map<vreal, vreal> &curmap = yi->second;//boundary[yi->first];
						curmap.insert(std::pair<vreal, vreal>(ipoint.x(), ipoint.x()));
					}
				}
			}
			else
				break;
		}
	}
}
void SequentialLineSet::XFillBoundary3D(const CoordinateSystem &cs, std::map<vreal, std::multimap<vreal, VPoint>> &xCoordYCoordPointMap, vreal tol)const
{
	// for all lines
	int lsize = Size();
	for (int li = 0; li<lsize; ++li)
	{
		VLine curl = this->operator[](li);
		VLine tline(cs.AbsoluteCoordToRelativeCoord(curl.GetP1())
			, cs.AbsoluteCoordToRelativeCoord(curl.GetP2()));
		VPoint lmin = tline.GetMin();
		VPoint lmax = tline.GetMax();
		std::map<vreal, std::multimap<vreal, VPoint>>::iterator xi = xCoordYCoordPointMap.lower_bound(lmin.x());//xCoordYCoordPointMap.begin();
		std::map<vreal, std::multimap<vreal, VPoint>>::iterator xe = xCoordYCoordPointMap.end();
		for (xi; xi != xe; ++xi)
		{
			if (xi->first >= lmin.x() - tol && xi->first <= lmax.x() + tol)
			{
				VPoint origin(lmin);
				origin.x(xi->first);
				VPlane pln(origin, UX);
				vreal intt;
				VPlane::IntType ck = pln.Int(tline, intt);
				if (ck == VPlane::REGULAR)
				{
					if (intt > -EPSILON && intt < 1 + EPSILON)
					{
						VPoint ipoint = tline.GetP(intt);
						std::multimap<vreal, VPoint> &curmap = xi->second;//xCoordYCoordPointMap[xi->first];
						curmap.insert(std::pair<vreal, VPoint>(ipoint.y(), ipoint));
					}
				}
			}
			else
				break;
		}
	}
}
void SequentialLineSet::YFillBoundary3D(const CoordinateSystem &cs, std::map<vreal, std::multimap<vreal, VPoint>> &yCoordXCoordPointMap, vreal tol)const
{
	// for all lines
	int lsize = Size();
	for (int li = 0; li<lsize; ++li)
	{
		VLine curl = this->operator[](li);
		VLine tline(cs.AbsoluteCoordToRelativeCoord(curl.GetP1())
			, cs.AbsoluteCoordToRelativeCoord(curl.GetP2()));
		VPoint lmin = tline.GetMin();
		VPoint lmax = tline.GetMax();
		std::map<vreal, std::multimap<vreal, VPoint>>::iterator yi = yCoordXCoordPointMap.lower_bound(lmin.y());//yCoordXCoordPointMap.begin();
		std::map<vreal, std::multimap<vreal, VPoint>>::iterator ye = yCoordXCoordPointMap.end();
		for (yi; yi != ye; ++yi)
		{
			if (yi->first >= lmin.y() - tol && yi->first <= lmax.y() + tol)
			{
				VPoint origin(lmin);
				origin.y(yi->first);
				VPlane pln(origin, UY);
				vreal intt;
				VPlane::IntType ck = pln.Int(tline, intt);
				if (ck == VPlane::REGULAR)
				{
					if (intt > -EPSILON && intt < 1 + EPSILON)
					{
						VPoint ipoint = tline.GetP(intt);
						std::multimap<vreal, VPoint> &curmap = yi->second;//yCoordXCoordPointMap[yi->first];
						curmap.insert(std::pair<vreal, VPoint>(ipoint.x(), ipoint));
					}
				}
			}
			else
				break;
		}
	}
}

vreal SequentialLineSet::GetGreenAreaOnXY()const
{
	int mysize = GetPointSize();
	vreal GreenAreaOnXY = 0.0;
	for (int i = 0; i < mysize; ++i)
	{
		int j = (i+1) % mysize;
		const VPoint *ip = GetPoint(i);
		const VPoint *jp = GetPoint(j);
		GreenAreaOnXY += ip->x() * jp->y();
		GreenAreaOnXY -= ip->y() * jp->x();
	}
	return fabs(GreenAreaOnXY*0.5);
}

void SequentialLineSet::BreakAtSharpCorners(vreal sharpAngleThresholdInDegrees , std::vector<SequentialLineSet *> &splitVec , vreal coincidentTol)const
{
	SequentialLineSet *sl = new SequentialLineSet(this->GetPointSize());
	int lsize = Size();
	
	if (lsize == 0)
		return;
	int firstCounter = 0;
	sl->InsertPoint(*GetPoint(firstCounter));
	VLine firstLine(*GetPoint(firstCounter),*GetPoint(firstCounter+1));
	while(firstLine.GetV().Compare(VPoint(0.0), coincidentTol) == true)
	{
		++firstCounter;
		if (firstCounter > lsize - 2)
			throw VException("SequentialLineSet::BreakAtSharpCorners exception , Sl length is smaller than coincident tolerance",1);
		firstLine.SetP2(*GetPoint(firstCounter+1));
	}
	sl->InsertPoint(firstLine.GetP2());

	if (lsize == 1)
	{
		if (sl->Size() > 0)
		{
			splitVec.push_back(sl);
		}
		else
		{
			LogFileWriter::WriteUnknownExceptionLog("SequentialLineSet::BreakAtSharpCorners exception", " Sl length is smaller than coincident tolerance");
		}
		return;
	}

	VLine prevLine = sl->operator[](firstCounter);
	VVector prevVec = prevLine.GetV().Unit();

	for (int li = firstCounter+1; li < lsize; ++li)
	{
		VLine curLine = this->operator[](li);
		VVector curVec = curLine.GetV();
		if (curVec.Compare(VPoint(0.0), coincidentTol) == true)
			continue;
		curVec.NormThis();// .Unit();
		vreal myAngDeg = acos (curVec % prevVec) * RAD_TO_DEGREE;
		//vreal angToCk = 180.0 - myAngDeg;
		if (myAngDeg > sharpAngleThresholdInDegrees)
		{
			if (sl->Size() > 0)
			{
				splitVec.push_back(sl);
				sl = new SequentialLineSet(200);
			}
			else
			{
				LogFileWriter::WriteUnknownExceptionLog("SequentialLineSet::BreakAtSharpCorners ","after break prev sl is empty");
			}
			sl->InsertPoint(curLine.GetP1());
			sl->InsertPoint(curLine.GetP2());
		}
		else
		{
			sl->InsertPoint(curLine.GetP2());
		}
		prevLine = curLine;
		prevVec = curVec;
	}
	if (sl->Size() > 0)
	{
		splitVec.push_back(sl);
	}
}

void SequentialLineSet::SetZ(vreal z)
{
	int mysize = Size();
	for (int i = 0; i < mysize; ++i)
	{
		VPoint *curp = this->GetPoint(i);
		curp->z(z);
	}
}

void SequentialLineSet::CollectSharpCorners(vreal sharpAngleThresholdInDegrees, std::vector<VPoint>& sharpCorners, vreal coincidentTol)const
{
	int firstCounter = 0;
	VLine prevLine = this->operator[](firstCounter);
	VVector prevVec = prevLine.GetV().Unit();
	int lsize = this->Size();
	for (int li = firstCounter + 1; li < lsize; ++li)
	{
		VLine curLine = this->operator[](li);
		VVector curVec = curLine.GetV();
		if (curVec.Compare(VPoint(0.0), coincidentTol) == true)
			continue;
		curVec.NormThis();// .Unit();
		vreal myAngDeg = acos(curVec % prevVec) * RAD_TO_DEGREE;
		//vreal angToCk = 180.0 - myAngDeg;
		if (myAngDeg > sharpAngleThresholdInDegrees)
		{
			sharpCorners.push_back(curLine.GetP1());
		}
		prevLine = curLine;
		prevVec = curVec;
	}
	VLine firstLine = this->operator[](firstCounter);
	VVector firstVec = prevLine.GetV().Unit();
	vreal myAngDeg = acos(firstVec % prevVec) * RAD_TO_DEGREE;
	//vreal angToCk = 180.0 - myAngDeg;
	if (myAngDeg > sharpAngleThresholdInDegrees)
	{
		sharpCorners.push_back(firstLine.GetP1());
	}
}

	// divides overall distance of the line sequence to arraySegmentCount and for i x 
	SequentialLineSet::SLParametricInfo::SLParametricInfo(const std::vector<VPoint>& seq, vreal seqLength , int arraySegmentCount)
		:m_seq(seq),m_arraySegmentCount(arraySegmentCount),m_seqLength(seqLength)
	{
		//const std::vector<VPoint>& m_seq;
		//int m_arraySegmentCount;
		//vreal m_segmentDistanceStep; // segmentDistanceStep = m_seq.Length / arraySegmentCount
		m_seqLength = seqLength;
		m_segmentDistanceStep = m_seqLength / m_arraySegmentCount;

		// For each line if a point on line distance to sequence zero is between segment start and end distances to sequence zero THEN line index is put inside  i  th index vector 
		//std::list<std::pair<int, vreal>>* m_distanceFrom0LineIndexArray;// pair holds line index and line start distance from seq begin
		m_distanceFrom0LineIndexArray = new std::list<std::pair<int, vreal>>[m_arraySegmentCount];
		int lineCount = 0;
		VPoint prevP = m_seq.at(0);
		vreal distanceSoFar = 0.0;
		vreal prevDistanceSoFar = distanceSoFar;
		int prevDistanceSoFarSegmenti = 0;
		for (int i = 1; i < m_seq.size(); ++i)
		{
			VPoint curP = m_seq.at(i);
			vreal curLineLen = (curP - prevP).GetLength();
			distanceSoFar += curLineLen;
			int distanceSoFarSegmenti = GetSegmentiForDistanceToSeqZero(distanceSoFar);
			for (int j = prevDistanceSoFarSegmenti; j <= distanceSoFarSegmenti; ++j)
			{
				m_distanceFrom0LineIndexArray[j].push_back(std::pair<int, vreal>(i-1,prevDistanceSoFar));
			}
			prevP = curP;
			prevDistanceSoFarSegmenti = distanceSoFarSegmenti;
			prevDistanceSoFar = distanceSoFar;
		}
	}
	SequentialLineSet::SLParametricInfo::~SLParametricInfo()
	{
		delete[] m_distanceFrom0LineIndexArray;
	}
	vreal SequentialLineSet::SLParametricInfo::GetSeqLength()const
	{
		return m_seqLength;
	}
	vreal SequentialLineSet::SLParametricInfo::GetSegmentStartDistanceToSeqZero(int segmenti)const
	{
		// segmentStartDistanceToSequenceZero = i * segmentDistanceStep
		vreal segmentStartDistanceToSequenceZero = segmenti * m_segmentDistanceStep;
		return segmentStartDistanceToSequenceZero;
	}
	vreal SequentialLineSet::SLParametricInfo::GetSegmentEndDistanceToSeqZero(int segmenti)const // segmentEndDistanceToSequenceZero = (i + 1) * segmentDistanceStep
	{
		vreal segmentEndDistanceToSequenceZero = (segmenti+1) * m_segmentDistanceStep;
		return segmentEndDistanceToSequenceZero;
	}
	int SequentialLineSet::SLParametricInfo::GetSegmentiForDistanceToSeqZero(vreal distanceFromSeqBegin)const
	{
		// i = (int) (distanceFromSeqBegin / segmentDistanceStep) , i must be in interval [0 , arraySegmentCount - 1]
		int i = (int)(distanceFromSeqBegin / m_segmentDistanceStep);
		if (i < 0)
			i = 0;
		if (i > (m_arraySegmentCount - 1))
			i = m_arraySegmentCount - 1;
		return i;
	}
	std::pair<vreal,VLine> SequentialLineSet::SLParametricInfo::GetLineForDistance(vreal distanceFromSeqBegin,int &curli)const
	{
		std::list<std::pair<vreal,VLine>> lCollection;
		int i = GetSegmentiForDistanceToSeqZero(distanceFromSeqBegin);
		std::list<std::pair<int, vreal>>& curlist = m_distanceFrom0LineIndexArray[i];
		for (auto li = curlist.begin(); li != curlist.end(); ++li)
		{
			vreal curLineLStart = li->second;
			curli = li->first;
			VPoint p0 = m_seq.at(curli);
			VPoint p1 = m_seq.at(curli + 1);
			VLine curL(p0, p1);
			if (distanceFromSeqBegin>= curLineLStart && distanceFromSeqBegin <= curLineLStart + curL.GetLength())
			return std::pair<vreal,VLine>(curLineLStart,curL);
		}
		curli = curlist.rbegin()->first;
		return std::pair<vreal,VLine>(curlist.rbegin()->second,VLine(m_seq.at(curli),m_seq.at(curli+1)));
	}
	std::map<const SequentialLineSet*, const SequentialLineSet::SLParametricInfo*> * SequentialLineSet::m_parametricInfoMap = new std::map<const SequentialLineSet*, const SequentialLineSet::SLParametricInfo*>();
	
	VPoint SequentialLineSet::GetP(const SequentialLineSet& sl, const vreal slLength , const vreal t, int& linei, vreal& lenUpToHitLineStart)
	{
		auto info = m_parametricInfoMap->find(&sl);
		const SLParametricInfo* parinfo;
		if (info == m_parametricInfoMap->end())
		{
			SequentialLineSet::SLParametricInfo* newSlParInfo = new SLParametricInfo(sl.GetPointSet().GetVec(), slLength);
			m_parametricInfoMap->operator[](&sl) = newSlParInfo;
			parinfo = newSlParInfo;
		}
		else
		{
			parinfo = info->second;
		}
		vreal lenuptopoint = t * slLength;
		std::pair<vreal,VLine> curdistlinepair = parinfo->GetLineForDistance(lenuptopoint,linei);
		vreal lenonline = lenuptopoint - curdistlinepair.first;
		vreal paronline = lenonline / curdistlinepair.second.GetLength();
		VPoint curp = curdistlinepair.second.GetP(paronline);
		lenUpToHitLineStart = curdistlinepair.first;// +curdistlinepair.second.GetV().GetLength();
		return curp;
	}

	void SequentialLineSet::DigitiseByLengthStepNew(SequentialLineSet& toFill, vreal lenStep)
	{
		int lineCount = Size();
		if (lineCount == 0)
		{
			LogFileWriter::WriteUnknownExceptionLog("SequentialLineSet::DigitiseByLengthStepNew exception.Size is 0.");
			return;
		}
		toFill.InsertPoint(*this->GetPoint(0));
		for (int i = 0; i < lineCount; ++i)
		{
			VLine curLine = this->operator[](i);
			vreal lineLen = curLine.GetLength();
			int pCount = (int)(lineLen / lenStep);
			vreal curStep = lineLen / (pCount + 1);
			VPoint v = curLine.GetV().Unit();
		
			for (int i = 0; i < pCount; ++i)
			{
				VPoint p = curLine.GetP1() + v * (curStep * i);
				toFill.InsertPoint(p);
			}
			toFill.InsertPoint(curLine.GetP2());
		}
	}