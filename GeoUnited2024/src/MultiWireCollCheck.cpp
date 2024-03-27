// MultiWireCollCheck.cpp

#include "stdafx.h"
#include "MultiWireCollCheck.h"
#include "VLine.h"
#include "VArc.h"
#include "SequentialLineSet.h"
#include "FastArcDigitiser.h"
#include "SweptCircleLineColl.h"
#include "VInterval.h"
#include "SweptCircleArcColl.h"
#include "VArc2d.h"
#include "FastArcDigitiser.h"

std::vector<VLine> MultiWireCollCheck::TrimLine(const VLine &l, const std::vector<SequentialLineSet *> &curvVec, double offsetRad , int ignorei)
{
	std::vector<VInterval> nocoll;
	nocoll.push_back(VInterval (0,1));
	VLine2d l2d (VPoint2d(l.GetP1().x(),l.GetP1().y()),VPoint2d(l.GetP2().x(),l.GetP2().y()));
	SweptCircleLineColl mycoll (offsetRad , l2d.GetP1(),l2d.GetP2());

	size_t mysize = curvVec.size();
	for (size_t i=0;i<mysize;++i)
	{
		if (i == ignorei)
			continue;
		SequentialLineSet *sl = curvVec[i];
		int mysize2 = sl->Size();
		for (int j=0;j<mysize2;++j)
		{
			VLine l2 = sl->operator [](j);
			VInterval ck = mycoll.Coll (VPoint2d (l2.GetP1().x(),l2.GetP1().y()),VPoint2d (l2.GetV().x(),l2.GetV().y()));
			if (SweptCircleLineColl::IsColliding(ck))
			{
				nocoll = VInterval::Subtract (nocoll , ck);
			}
		}
	}
	std::vector<VLine> res;
	size_t mysize3 = nocoll.size();
	for (size_t k=0;k<mysize3;++k)
	{
		VInterval &ia = nocoll[k];
		VLine toadd (l.GetP(ia.GetT0()),l.GetP(ia.GetT1()));
		res.push_back(toadd);
	}
	return res;
}

std::vector<VArc> MultiWireCollCheck::TrimArc (const VArc &a , double tol , const std::vector<SequentialLineSet *> &curvVec , double offsetRad , int ignorei)
{
	SequentialLineSet asl (100);
	std::vector<double> angles;
	FastArcDigitiser::Digitise (a , tol , asl , angles);
	std::vector<VInterval> totalcoll;// This is angles in degrees
	int mysize = asl.Size();
	for (int i=0; i<mysize;++i)
	{
		VLine l = asl.operator [](i);
		double ang0 = angles.at(i);
		double ang1 = angles.at(i+1);
		double da = ang1 - ang0;
		std::vector<VInterval> colli = GetCollInt(l,curvVec,offsetRad,ignorei);
		size_t mysize2 = colli.size();
		if (mysize2)
		{
			std::vector<VInterval> collangles;
			for (size_t j=0;j<mysize2;++j)
			{
				const VInterval &ic = colli[j];
				double a0 = ang0 + da * ic.GetT0();
				double a1 = ang0 + da * ic.GetT1();
				VInterval cai (a0,a1);
				collangles.push_back(cai);
			}
			totalcoll = VInterval::Merge(totalcoll,collangles);
		}
	}

	// Subtract collision from circle angle sweep
	std::vector<VInterval> mycircle;
	mycircle.push_back (VInterval (0.0 , a.GetSweepAngle()));
	std::vector<VInterval> remcircle = VInterval::Subtract (mycircle , totalcoll);
	
	std::vector<VArc> res;
	// Now fill the res with remaining circles
	size_t mysizen = remcircle.size();
	for (size_t i=0;i<mysizen;++i)
	{
		const VInterval &curi = remcircle[i];
		double a0 = curi.GetT0();
		double a1 = curi.GetT1();
		VPoint sp = a.GetP (a0);
		VArc mya (a.GetCenter(),a.GetRadius(),a.GetAx(),sp,a1-a0);
		res.push_back (mya);
	}
	
	return res;
}

std::vector<VInterval> MultiWireCollCheck::GetCollInt (const VLine &l , const std::vector<SequentialLineSet *> &curvVec , double offsetRad , int ignorei)
{
	std::vector<VInterval> coll;
	VLine2d l2d (VPoint2d(l.GetP1().x(),l.GetP1().y()),VPoint2d(l.GetP2().x(),l.GetP2().y()));
	SweptCircleLineColl mycoll (offsetRad , l2d.GetP1(),l2d.GetP2());

	size_t mysize = curvVec.size();
	for (size_t i=0;i<mysize;++i)
	{
		if (i == ignorei)
			continue;
		SequentialLineSet *sl = curvVec[i];
		int mysize2 = sl->Size();
		for (int j=0;j<mysize2;++j)
		{
			VLine l2 = sl->operator [](j);
			VInterval ck = mycoll.Coll (VPoint2d (l2.GetP1().x(),l2.GetP1().y()),VPoint2d (l2.GetV().x(),l2.GetV().y()));
			if (SweptCircleLineColl::IsColliding(ck))
			{
				coll = VInterval::Merge (coll , ck);
			}
		}
	}
	return coll;
}

std::vector<VLine> MultiWireCollCheck::TrimLine (const VLine &l , const std::vector<LineOrArc> &curvVec , double offsetRad , int parenti , int startneighbori , int endneighbori , double jointol)
{
	std::vector<VInterval> nocoll;
	nocoll.push_back(VInterval (0,1));
	VLine2d l2d (VPoint2d(l.GetP1().x(),l.GetP1().y()),VPoint2d(l.GetP2().x(),l.GetP2().y()));
	SweptCircleLineColl mycoll (offsetRad-jointol , l2d.GetP1(),l2d.GetP2());
	SweptCircleLineColl mycollfine (offsetRad , l2d.GetP1() , l2d.GetP2());
	SweptCircleArcColl myarcColl (offsetRad-jointol , l2d.GetP1() , l2d.GetP2());
	SweptCircleArcColl myarcCollFine (offsetRad , l2d.GetP1() , l2d.GetP2());
	size_t mysize = curvVec.size();
	for (size_t i=0;i<mysize;++i)
	{
		if (i==parenti)
		{
			continue;
		}

		const LineOrArc &geo = curvVec[i];
		
		if (geo.GetType() == LineOrArc::line)
		{
			const VLine * l2 = geo.GetLine ();
			VPoint2d curlp (l2->GetP1().x(),l2->GetP1().y());
			VPoint2d curlv (l2->GetV().x(),l2->GetV().y());
			VInterval ck = mycoll.Coll (curlp , curlv );
			bool iscollidingnow = SweptCircleLineColl::IsColliding(ck);
			//if (i==startneighbori && iscollidingnow)
			//{
			//	CleanBoundaryCollisionAtStart (ck , 0.0 , 1.0 , l.GetLength() , jointol);
			//	iscollidingnow = SweptCircleLineColl::IsColliding (ck);
			//}
			//else if (i==endneighbori &&  iscollidingnow)
			//{
			//	CleanBoundaryCollisionAtEnd (ck , 0.0 , 1.0 , l.GetLength() , jointol);
			//	iscollidingnow = SweptCircleLineColl::IsColliding (ck);
			//}
			if (iscollidingnow)
			{
				ck = mycollfine.Coll (curlp , curlv); 
				nocoll = VInterval::Subtract (nocoll , ck);
			}
		}
		else 
		{
			VArc as = *geo.GetArc ();
			VArc2d as2d = Get2dArc (as);
			std::vector<VInterval> collin = myarcColl.Coll (as2d);
			if (collin.size())
			{
				collin = myarcCollFine.Coll (as2d);
			}
			//if (i==startneighbori || i==endneighbori)
			//{
			//	std::vector<VInterval> tempcollin;
			//	size_t mycollinsize = collin.size();
			//	for (size_t jj=0;jj<mycollinsize;++jj)
			//	{
			//		VInterval &curcolres = collin.at(jj);
			//		if (SweptCircleLineColl::IsColliding (curcolres))
			//		{
			//			if (i==startneighbori)
			//				CleanBoundaryCollisionAtStart (curcolres , 0.,1.,l.GetLength(),jointol);
			//			else if (i==endneighbori)
			//				CleanBoundaryCollisionAtEnd (curcolres , 0.,1.,l.GetLength(),jointol);
			//			if (SweptCircleLineColl::IsColliding (curcolres))
			//				tempcollin.push_back (curcolres);
			//		}
			//	}
			//	collin = tempcollin;	// collision noise removed at start or end 
			//}
			nocoll = VInterval::Subtract (nocoll , collin);
		}
	}
	nocoll = GetSortedVec(nocoll);
	std::vector<VLine> res;
	size_t mysize3 = nocoll.size();
	for (size_t k=0;k<mysize3;++k)
	{
		VInterval &ia = nocoll[k];
		VLine toadd (l.GetP(ia.GetT0()),l.GetP(ia.GetT1()));
		res.push_back(toadd);
	}
	return res;
}
std::vector<VLine> MultiWireCollCheck::TrimLine (const VLine &l , const std::vector<LineOrArc *> &curvVec , double offsetRad , int parenti , int startneighbori , int endneighbori , double jointol)
{
	std::vector<VInterval> nocoll;
	nocoll.push_back(VInterval (0,1));
	VLine2d l2d (VPoint2d(l.GetP1().x(),l.GetP1().y()),VPoint2d(l.GetP2().x(),l.GetP2().y()));
	SweptCircleLineColl mycoll (offsetRad-jointol , l2d.GetP1(),l2d.GetP2());
	SweptCircleLineColl mycollfine (offsetRad , l2d.GetP1() , l2d.GetP2());
	SweptCircleArcColl myarcColl (offsetRad-jointol , l2d.GetP1() , l2d.GetP2());
	SweptCircleArcColl myarcCollFine (offsetRad , l2d.GetP1() , l2d.GetP2());
	size_t mysize = curvVec.size();
	for (size_t i=0;i<mysize;++i)
	{
		if (i==parenti)
		{
			continue;
		}

		const LineOrArc &geo = *curvVec[i];
		
		if (geo.GetType() == LineOrArc::line)
		{
			const VLine * l2 = geo.GetLine ();
			VPoint2d curlp (l2->GetP1().x(),l2->GetP1().y());
			VPoint2d curlv (l2->GetV().x(),l2->GetV().y());
			VInterval ck = mycoll.Coll (curlp , curlv );
			bool iscollidingnow = SweptCircleLineColl::IsColliding(ck);
			//if (i==startneighbori && iscollidingnow)
			//{
			//	CleanBoundaryCollisionAtStart (ck , 0.0 , 1.0 , l.GetLength() , jointol);
			//	iscollidingnow = SweptCircleLineColl::IsColliding (ck);
			//}
			//else if (i==endneighbori &&  iscollidingnow)
			//{
			//	CleanBoundaryCollisionAtEnd (ck , 0.0 , 1.0 , l.GetLength() , jointol);
			//	iscollidingnow = SweptCircleLineColl::IsColliding (ck);
			//}
			if (iscollidingnow)
			{
				ck = mycollfine.Coll (curlp , curlv); 
				nocoll = VInterval::Subtract (nocoll , ck);
			}
		}
		else 
		{
			VArc as = *geo.GetArc ();
			VArc2d as2d = Get2dArc (as);
			std::vector<VInterval> collin = myarcColl.Coll (as2d);
			if (collin.size())
			{
				collin = myarcCollFine.Coll (as2d);
			}
			//if (i==startneighbori || i==endneighbori)
			//{
			//	std::vector<VInterval> tempcollin;
			//	size_t mycollinsize = collin.size();
			//	for (size_t jj=0;jj<mycollinsize;++jj)
			//	{
			//		VInterval &curcolres = collin.at(jj);
			//		if (SweptCircleLineColl::IsColliding (curcolres))
			//		{
			//			if (i==startneighbori)
			//				CleanBoundaryCollisionAtStart (curcolres , 0.,1.,l.GetLength(),jointol);
			//			else if (i==endneighbori)
			//				CleanBoundaryCollisionAtEnd (curcolres , 0.,1.,l.GetLength(),jointol);
			//			if (SweptCircleLineColl::IsColliding (curcolres))
			//				tempcollin.push_back (curcolres);
			//		}
			//	}
			//	collin = tempcollin;	// collision noise removed at start or end 
			//}
			nocoll = VInterval::Subtract (nocoll , collin);
		}
	}
	nocoll = GetSortedVec(nocoll);
	std::vector<VLine> res;
	size_t mysize3 = nocoll.size();
	for (size_t k=0;k<mysize3;++k)
	{
		VInterval &ia = nocoll[k];
		VLine toadd (l.GetP(ia.GetT0()),l.GetP(ia.GetT1()));
		res.push_back(toadd);
	}
	return res;
}
VArc2d MultiWireCollCheck::Get2dArc (VArc &as)
{
	if (as.GetAx().Compare(-UZ , 0.001))
	{
		as.ReverseAxis();
	}
	if (as.GetAx().Compare(UZ , 0.001) == false)
		throw VException (_T( IDS_MulExcArcIsNotO),1);
	VPoint sp = as.GetStartPoint();
	VPoint ep = as.GetEndPoint();
	VPoint2d sp2d (sp.x() , sp.y());
	VPoint2d ep2d (ep.x() , ep.y());
	VPoint2d cp2d (as.GetCenter().x(),as.GetCenter().y());
	VArc2d as2d (cp2d , as.GetRadius() , sp2d , ep2d , as.IsCCW());
	return as2d;
}

std::vector<VArc> MultiWireCollCheck::TrimArc (const VArc &a , double tol , const std::vector<LineOrArc> &curvVec , double offsetRad , int parenti , int startneighbori , int endneighbori , double jointol)
{
	SequentialLineSet asl (100);
	std::vector<double> angles;
	FastArcDigitiser::Digitise (a , tol * 0.02 , asl , angles);
	std::vector<VInterval> totalcoll;// This is all intervals  between [0,1]
	int mysize = asl.Size();
	double arclen = a.GetArcLength();
	for (int i=0; i<mysize;++i)
	{
		VLine l = asl.operator [](i);
		double ang0 = angles.at(i);
		double ang1 = angles.at(i+1);
		double da = ang1 - ang0;
		std::vector<VInterval> colli = GetCollInt(l,curvVec,offsetRad,parenti,startneighbori,endneighbori,ang0/da,ang1/da,arclen,jointol);
		size_t mysize2 = colli.size();
		if (mysize2)
		{
			std::vector<VInterval> colliarc;
			for (size_t j=0;j<mysize2;++j)
			{
				const VInterval &ic = colli[j];
				double a0 = ang0 + da * ic.GetT0();
				double a1 = ang0 + da * ic.GetT1();
				double t0arc = a0 / a.GetSweepAngle();
				double t1arc = a1 / a.GetSweepAngle();
				VInterval cai (t0arc,t1arc);
				colliarc.push_back(cai);
			}
			totalcoll = VInterval::Merge(totalcoll,colliarc);
		}
	}

	// Subtract collision from circle angle sweep
	std::vector<VInterval> mycircle;
	mycircle.push_back (VInterval (0.0 , 1.0));
	std::vector<VInterval> remcircle = VInterval::Subtract (mycircle , totalcoll);
	remcircle = GetSortedVec(remcircle);

	std::vector<VArc> res;
	// Now fill the res with remaining circles
	size_t mysizen = remcircle.size();
	for (size_t i=0;i<mysizen;++i)
	{
		const VInterval &curi = remcircle[i];
		double a0 = curi.GetT0() * a.GetSweepAngle();
		double a1 = curi.GetT1() * a.GetSweepAngle();
		VPoint sp = a.GetP (a0);
		VArc mya (a.GetCenter(),a.GetRadius(),a.GetAx(),sp,a1-a0);
		res.push_back (mya);
	}
	
	return res;
}
std::vector<VArc> MultiWireCollCheck::TrimArc (const VArc &a , double tol , const std::vector<LineOrArc *> &curvVec , double offsetRad , int parenti , int startneighbori , int endneighbori , double jointol)
{
	SequentialLineSet asl (100);
	std::vector<double> angles;
	FastArcDigitiser::Digitise (a , tol , asl , angles);
	std::vector<VInterval> totalcoll;// This is all intervals  between [0,1]
	int mysize = asl.Size();
	double arclen = a.GetArcLength();
	for (int i=0; i<mysize;++i)
	{
		VLine l = asl.operator [](i);
		double ang0 = angles.at(i);
		double ang1 = angles.at(i+1);
		double da = ang1 - ang0;
		std::vector<VInterval> colli = GetCollInt(l,curvVec,offsetRad,parenti,startneighbori,endneighbori,ang0/da,ang1/da,arclen,jointol);
		size_t mysize2 = colli.size();
		if (mysize2)
		{
			std::vector<VInterval> colliarc;
			for (size_t j=0;j<mysize2;++j)
			{
				const VInterval &ic = colli[j];
				double a0 = ang0 + da * ic.GetT0();
				double a1 = ang0 + da * ic.GetT1();
				double t0arc = a0 / a.GetSweepAngle();
				double t1arc = a1 / a.GetSweepAngle();
				VInterval cai (t0arc,t1arc);
				colliarc.push_back(cai);
			}
			totalcoll = VInterval::Merge(totalcoll,colliarc);
		}
	}

	// Subtract collision from circle angle sweep
	std::vector<VInterval> mycircle;
	mycircle.push_back (VInterval (0.0 , 1.0));
	std::vector<VInterval> remcircle = VInterval::Subtract (mycircle , totalcoll);
	remcircle = GetSortedVec(remcircle);

	std::vector<VArc> res;
	// Now fill the res with remaining circles
	size_t mysizen = remcircle.size();
	for (size_t i=0;i<mysizen;++i)
	{
		const VInterval &curi = remcircle[i];
		double a0 = curi.GetT0() * a.GetSweepAngle();
		double a1 = curi.GetT1() * a.GetSweepAngle();
		VPoint sp = a.GetP (a0);
		VArc mya (a.GetCenter(),a.GetRadius(),a.GetAx(),sp,a1-a0);
		res.push_back (mya);
	}
	
	return res;
}
std::vector<VInterval> MultiWireCollCheck::GetCollInt(const VLine &l, const std::vector<LineOrArc> &curvVec, double offsetRad , int parenti,int startneighbori,int endneighbori,double myt0 , double myt1 , double arclen , double jointol)
{
	std::vector<VInterval> coll;
	VLine2d l2d (VPoint2d(l.GetP1().x(),l.GetP1().y()),VPoint2d(l.GetP2().x(),l.GetP2().y()));
	SweptCircleLineColl mycoll (offsetRad-jointol , l2d.GetP1(),l2d.GetP2());
	SweptCircleLineColl mycollfine (offsetRad , l2d.GetP1() , l2d.GetP2());
	SweptCircleArcColl myarcColl (offsetRad-jointol , l2d.GetP1() , l2d.GetP2());
	SweptCircleArcColl myarcCollFine (offsetRad , l2d.GetP1() , l2d.GetP2());
	size_t mysize = curvVec.size();
	for (size_t i=0;i<mysize;++i)
	{
		if (i == parenti) 
			continue;

		const LineOrArc &geo = curvVec[i];
	
		if (geo.GetType() == LineOrArc::line)
		{
			const VLine *l2 = geo.GetLine();
			VPoint2d curlp (l2->GetP1().x(),l2->GetP1().y());
			VPoint2d curlv (l2->GetV().x(),l2->GetV().y());
			VInterval ck = mycoll.Coll (curlp,curlv);
			//bool iscollidingnow = ;
			//if (i==startneighbori && iscollidingnow)
			//{
			//	CleanBoundaryCollisionAtStart (ck , myt0 , myt1 , arclen , jointol);
			//}
			//else if (i==endneighbori && iscollidingnow)
			//{
			//	CleanBoundaryCollisionAtEnd (ck , myt0 , myt1 , arclen , jointol);
			//}
			if (SweptCircleLineColl::IsColliding(ck))
			{
				ck = mycollfine.Coll (curlp , curlv);
				if (SweptCircleLineColl::IsColliding (ck))
					coll = VInterval::Merge (coll , ck);
			}
		}
		else
		{
			VArc as = *geo.GetArc();
			VArc2d as2d = Get2dArc (as);
			std::vector<VInterval> collin = myarcColl.Coll (as2d);
			//if (i == startneighbori || i == endneighbori)
			//{
			//	std::vector<VInterval> tempcollin;
			//	size_t mysizecollin = collin.size();
			//	for (size_t jj=0;jj<mysizecollin;++jj)
			//	{
			//		VInterval &curcolres = collin.at(jj);
			//		if (SweptCircleLineColl::IsColliding (curcolres))
			//		{
			//			if (i==startneighbori)
			//			{
			//				CleanBoundaryCollisionAtStart (curcolres , myt0,myt1 ,arclen,jointol);
			//			}
			//			else if (i==endneighbori)
			//			{
			//				CleanBoundaryCollisionAtEnd (curcolres , myt0 ,myt1 , arclen , jointol);
			//			}
			//			if (SweptCircleLineColl::IsColliding (curcolres))
			//				tempcollin.push_back (curcolres);
			//		}
			//	}
			//	collin = tempcollin;	// noise at start or end removed
			//}
			if (collin.size())
			{
				collin = myarcCollFine.Coll (as2d);
				if (collin.size())
					coll = VInterval::Merge (coll , collin);
			}
		}
	}
	return coll;
}
std::vector<VInterval> MultiWireCollCheck::GetCollInt(const VLine &l, const std::vector<LineOrArc *> &curvVec, double offsetRad , int parenti,int startneighbori,int endneighbori,double myt0 , double myt1 , double arclen , double jointol)
{
	std::vector<VInterval> coll;
	VLine2d l2d (VPoint2d(l.GetP1().x(),l.GetP1().y()),VPoint2d(l.GetP2().x(),l.GetP2().y()));
	SweptCircleLineColl mycoll (offsetRad-jointol , l2d.GetP1(),l2d.GetP2());
	SweptCircleLineColl mycollfine (offsetRad , l2d.GetP1() , l2d.GetP2());
	SweptCircleArcColl myarcColl (offsetRad-jointol , l2d.GetP1() , l2d.GetP2());
	SweptCircleArcColl myarcCollFine (offsetRad , l2d.GetP1() , l2d.GetP2());
	size_t mysize = curvVec.size();
	for (size_t i=0;i<mysize;++i)
	{
		if (i == parenti) 
			continue;

		const LineOrArc &geo = *curvVec[i];
	
		if (geo.GetType() == LineOrArc::line)
		{
			const VLine *l2 = geo.GetLine();
			VPoint2d curlp (l2->GetP1().x(),l2->GetP1().y());
			VPoint2d curlv (l2->GetV().x(),l2->GetV().y());
			VInterval ck = mycoll.Coll (curlp,curlv);
			//bool iscollidingnow = ;
			//if (i==startneighbori && iscollidingnow)
			//{
			//	CleanBoundaryCollisionAtStart (ck , myt0 , myt1 , arclen , jointol);
			//}
			//else if (i==endneighbori && iscollidingnow)
			//{
			//	CleanBoundaryCollisionAtEnd (ck , myt0 , myt1 , arclen , jointol);
			//}
			if (SweptCircleLineColl::IsColliding(ck))
			{
				ck = mycollfine.Coll (curlp , curlv);
				if (SweptCircleLineColl::IsColliding (ck))
					coll = VInterval::Merge (coll , ck);
			}
		}
		else
		{
			VArc as = *geo.GetArc();
			VArc2d as2d = Get2dArc (as);
			std::vector<VInterval> collin = myarcColl.Coll (as2d);
			//if (i == startneighbori || i == endneighbori)
			//{
			//	std::vector<VInterval> tempcollin;
			//	size_t mysizecollin = collin.size();
			//	for (size_t jj=0;jj<mysizecollin;++jj)
			//	{
			//		VInterval &curcolres = collin.at(jj);
			//		if (SweptCircleLineColl::IsColliding (curcolres))
			//		{
			//			if (i==startneighbori)
			//			{
			//				CleanBoundaryCollisionAtStart (curcolres , myt0,myt1 ,arclen,jointol);
			//			}
			//			else if (i==endneighbori)
			//			{
			//				CleanBoundaryCollisionAtEnd (curcolres , myt0 ,myt1 , arclen , jointol);
			//			}
			//			if (SweptCircleLineColl::IsColliding (curcolres))
			//				tempcollin.push_back (curcolres);
			//		}
			//	}
			//	collin = tempcollin;	// noise at start or end removed
			//}
			if (collin.size())
			{
				collin = myarcCollFine.Coll (as2d);
				if (collin.size())
					coll = VInterval::Merge (coll , collin);
			}
		}
	}
	return coll;
}
void MultiWireCollCheck::CleanBoundaryCollisionAtStart(VInterval &ck, double myt0 , double myt1 , double arclen , double jointol)
{
	double realt0 = myt0 + (myt1 - myt0) * ck.GetT0();
	double realt1 = myt0 + (myt1 - myt0) * ck.GetT1();
	if (realt1 * arclen < jointol)
	{	
		//collision noise with start neighbor so cancel coll i
		ck.SetT0 (-2);
		ck.SetT1 (-1);
	}
}

void MultiWireCollCheck::CleanBoundaryCollisionAtEnd(VInterval &ck, double myt0 , double myt1 , double arclen , double jointol)
{
	double realt0 = myt0 + (myt1 - myt0) * ck.GetT0();
	double realt1 = myt0 + (myt1 - myt0) * ck.GetT1();
	if ((1-realt0) * arclen < jointol)
	{
		//collision noise with end neighbor so cancel coll i
		ck.SetT0 (-2);
		ck.SetT1 (-1);
	}		
}

bool MultiWireCollCheck::IsColliding(const LineOrArc &tock, const SequentialLineSet &sl, double r , double tol)
{
	if (tock.GetType() == LineOrArc::line)
	{
		const VLine *l = tock.GetLine();
		if (IsColliding (l,sl,r))
			return true;
	}
	else
	{
		const VArc *a = tock.GetArc();
		int segmentno = FastArcDigitiser::GetDigitiseSegmentNo (*a,tol);		
		SequentialLineSet asl (segmentno+2);
		FastArcDigitiser::Digitise (*a,segmentno+1,asl);
		int mysize = asl.Size();
		for (int i=0;i<mysize;++i)
		{
			VLine curarcline = asl.operator [] (i);
			if (IsColliding (&curarcline , sl , r))
				return true;
		}
	}
	return false;
}

bool MultiWireCollCheck::IsColliding ( const VLine *l , const SequentialLineSet &sl , double r)
{
	VPoint2d a (l->GetP1().x(),l->GetP1().y());
	VPoint2d b (l->GetP2().x(),l->GetP2().y());
	SweptCircleLineColl cck (r,a,b);
	int mysize = sl.Size();
	for (int i=0;i<mysize;++i)
	{
		VLine l2 = sl.operator [] (i);
		VPoint deltav = l2.GetP2()-l2.GetP1();
		VPoint2d p0 (l2.GetP1().x(),l2.GetP1().y());
		VPoint2d v (deltav.x(),deltav.y());
		VInterval colli = cck.Coll (p0,v);
		if (SweptCircleLineColl::IsColliding (colli))
			return true;
	}
	return false;
}

//std::vector<VArc> MultiWireCollCheck::NewTrimArc(const VArc &a, double tol, const std::vector<LineOrArc> &curvVec, double offsetRad, int parenti, int startneighbori, int endneighbori, double jointol)
//{
//	std::vector<VArc> res;
//	vreal roughTol = tol * 15.0;
//	int digitiseSegmentNo = FastArcDigitiser::GetDigitiseSegmentNo(a, roughTol);
//	int endi = -1;
//	std::vector<VPoint> pv;
//	FastArcDigitiser::Digitise(a, digitiseSegmentNo + 1, pv);
//	int psize = (int)pv.size();
//	VPoint prevp = pv.at(0);
//	std::list<std::pair<int, int>> nonColliding;
//	int pi = 0;
//	vreal nearDistance = offsetRad - tol;
//	for (pi = 0; pi < psize; ++pi)
//	{
//		const VPoint &curp = pv.at(pi);
//		bool isColliding = IsColliding(nearDistance , curp, a, curvVec , roughTol);
//		if (isColliding)
//		{
//			if (pi - endi > 1)
//				nonColliding.push_back(std::pair<int, int>(endi,pi));
//			endi = pi;
//		}
//	}
//	if (pi - endi > 0)
//		nonColliding.push_back(std::pair<int, int>(endi,pi));
//
//	std::list<std::pair<int, int>>::iterator ii = nonColliding.begin();
//	std::list<std::pair<int, int>>::iterator ee = nonColliding.end();
//	for (ii; ii != ee; ++ii)
//	{
//		int bi = ii->first;
//		int ei = ii->second;
//		RefineResultAndAddToRemaining(nearDistance , bi, ei, pv , a , curvVec , roughTol , tol , res);
//	}
//
//	return res;
//}

bool MultiWireCollCheck::IsColliding(vreal nearDistance , const VPoint &curp, const VArc &srcArc, const std::vector<LineOrArc> &curves , vreal tol)
{
	int geoSize = (int)curves.size();
	for (int gi = 0; gi < geoSize; ++gi)
	{
		const LineOrArc &curGeo = curves.at(gi);
		VPoint pointOnGeo;
		vreal mydist = curGeo.GetMinDist(curp, pointOnGeo);
		if (mydist < tol)
			continue;
		if (mydist < nearDistance)
			return true;
	}
	return false;
}

void MultiWireCollCheck::RefineResultAndAddToRemaining(vreal nearDistance , int bi, int ei, const std::vector<VPoint> &pv, const VArc &a, const std::vector<LineOrArc> &curves
	, vreal roughTol, vreal tol, std::vector<VArc> &remaining)
{
	int bii = bi;
	bii--;
	if (bii < 0)
		bii = 0;
	int eii = ei + 1;
	if (eii >= pv.size())
		eii = (int)pv.size() - 1;
	const VPoint &sp = pv.at(bii);
	const VPoint &ep = pv.at(eii);
	LineOrArc src(a);
	vreal st, et;
	bool ck1 = src.GetParIf(sp, st, roughTol);
	bool ck2 = src.GetParIf(ep, et, roughTol);
	if (ck1 == false && ck2 == false)
	{
		throw VException("MultiWireCollCheck::RefineResultAndAddToRemaining exception , point is not on source",1);
	}
	if (st < 0.0)
		st = 0.0;
	if (et > 1.0)
		et = 1.0;
	if (a.IsFull() == true && fabs(et) < tol*0.1)
	{
		et = 1.0;
	}
	LineOrArc subsrc = src.GetSubSet(st, et);
	int segmentCount = FastArcDigitiser::GetDigitiseSegmentNo(*subsrc.GetArc(), tol);
	std::vector<VPoint> pvsub;
	FastArcDigitiser::Digitise(*subsrc.GetArc(), segmentCount + 1, pvsub);
	// find first non colliding at start
	// find first non colliding at end
	vreal newst, newet;
	bool ck = FirstNonCollidingAtStartAndEnd(nearDistance , pvsub, src, curves, roughTol, tol, newst, newet);
	if (ck == false)
		return;//it seems no part left to remaining//throw VException("MultiWireCollCheck::RefineResultAndAddToRemaining exception , refine failed",1);
	if (newst < 0.0)
		newst = 0.0;
	if (newet > 1.0)
		newet = 1.0;
	if (a.IsFull() == true && fabs(newet) < tol*0.1)
	{
		newet = 1.0;
	}
	LineOrArc refinedsub = subsrc.GetSubSet(newst, newet);
	remaining.push_back(*refinedsub.GetArc());
}

bool MultiWireCollCheck::FirstNonCollidingAtStartAndEnd (vreal nearDistance , const std::vector<VPoint> &pvsub, const LineOrArc &src, const std::vector<LineOrArc> &curves, vreal roughTol, vreal tol, vreal &newst, vreal &newet)
{
	bool isStartOk(false), isEndOk(false);
	int psize = (int)pvsub.size();
	for (int si = 0; si < psize; ++si)
	{
		const VPoint &curp = pvsub.at(si);
		bool ck = IsColliding(nearDistance, curp, *src.GetArc(), curves, roughTol);
		if (ck == false)
		{
			bool ckst = src.GetParIf(curp, newst, roughTol);
			if (ckst)
			{
				isStartOk = true;
				break;
			}
		}
	}
	for (int ei = psize - 1; ei >= 0; --ei)
	{
		const VPoint &curp = pvsub.at(ei);
		bool ck = IsColliding(nearDistance, curp, *src.GetArc(), curves, roughTol);
		if (ck == false)
		{
			bool cket = src.GetParIf(curp, newet, roughTol);
			if (cket)
			{
				isEndOk = true;
				break;
			}
		}
	}
	return (isStartOk && isEndOk);
}
