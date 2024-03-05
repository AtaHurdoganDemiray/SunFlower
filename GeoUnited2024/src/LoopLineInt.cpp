// LoopLineInt.cpp

#include "stdafx.h"
#include "LoopLineInt.h"
#include "SequentialLineSet2d.h"
#include "PosCubes2d.h"
#include "LineIntersection2d.h"
#include "SequentialLineSet.h"
#include "VPoint.h"
#include "CoordinateSystem.h"

bool LoopLineInt::IsPointInLoop ( const VPoint2d &p
								, const SequentialLineSet2d *sl 
								, PosCubes2d *cubes)
{
	vreal a = -1.02;
	vreal b = -1.03;
	//if (p.x()>0.5)
	//	a = 2.;
	//if (p.y() > 0.5)
	//	b=2.;
	//if ( p.y() < p.x() )
	//	b = p.y();
	//else
	//	a = p.x();
	VLine2d fake (VPoint2d(a , b) , p);
	LongVec relind,nii;
	cubes->SetRelatedIndiceListForLine (fake , relind , nii);
	//relind.RemoveDuplicate();
	PointSet2d rec;
	int intno = 0;
	for ( int i =0; i < relind.Size(); i++ )
	//for (int i=0;i<sl->Size();++i)
	{
		long currlineind = relind[i];
		VLine2d currline = sl->operator [](currlineind);	
		//VLine2d currline = sl->operator [](i);	
		
		LineIntersection2d::LineIntersectionData2d info 
			= LineIntersection2d::CalcInt ( fake , currline );	
		if ( info.intinfo == LineIntersection2d::LINESINTERSECT )
		{
			if(info.intno == 1)
			{
				int ind;
				bool ck = rec.GetIndex (info.intpoint[0] , ind , 1.E-5);
				if (ck==false)
				{
					intno++;	
					rec.InsertPoint(info.intpoint[0]);
				}
			}
		}
	}
	if (intno % 2 == 1)
		return true;
	else
		return false;
}
void LoopLineInt::LineLoopInt (const SequentialLineSet2d *loop 
							, PosCubes2d *cubes 
							, const VLine2d &l1 
							, std::vector<VPoint2d> &intpvec)
{
	LongVec relind,nii;
	cubes->SetRelatedIndiceListForLine (l1,relind,nii);
	int mysize = relind.Size();
	//int mysize = loop->Size();
	//VPoint2d intp;
	for (int i=0;i<mysize;i++)
	{
		int curri = relind[i];
		VLine2d l2 = loop->operator [] (curri);
		
		//if (SurfaceQuadTrimmer::RegularLineInt (l1,l2,intp))
		LineIntersection2d::LineIntersectionData2d info 
			= LineIntersection2d::CalcInt ( l1 , l2 );	
		if ( info.intinfo == LineIntersection2d::LINESINTERSECT )
		{
			if(info.intno == 1)
			{		
				bool isexist=false;
				std::vector<VPoint2d>::iterator ii=intpvec.begin();
				std::vector<VPoint2d>::iterator ee=intpvec.end();
				for(ii;ii!=ee;++ii)
				{
					if(ii->Compare(info.intpoint[0],1.e-7))
						isexist=true;
				}
				if (isexist==false)
					intpvec.push_back (info.intpoint[0]);
			}
		}
	}
}

void LoopLineInt::CalcParamOfLoopPoints(const SequentialLineSet2d *loop, std::vector<vreal> &paramVec)
{
	vreal totalLength=loop->GetLength();

	paramVec.push_back(0.0);
	int mysize=loop->Size();
	vreal L=0.0;
	for(int i=0;i<mysize;++i)
	{
		VLine2d currl =loop->operator [](i);
		L+=currl.GetLength();
		vreal currparam=L/totalLength;
		paramVec.push_back (currparam);
	}
}

void LoopLineInt::LineLoopInt(const SequentialLineSet2d *loop
							  ,const std::vector<vreal> &parameterValues
							  ,int loopNo
							  , PosCubes2d *cubes
							  , const VLine2d &l1
							  , std::vector<LoopLineInt::LoopLineIntData> &intpvec) 
{
	LongVec relind,nii;
	cubes->SetRelatedIndiceListForLine (l1,relind,nii);
	int mysize = relind.Size();
	//int mysize = loop->Size();
	//VPoint2d intp;
	for (int i=0;i<mysize;i++)
	{
		int curri = relind[i];
		VLine2d l2 = loop->operator [] (curri);
		
		//if (SurfaceQuadTrimmer::RegularLineInt (l1,l2,intp))
		LineIntersection2d::LineIntersectionData2d info 
			= LineIntersection2d::CalcInt ( l1 , l2 );	
		if ( info.intinfo == LineIntersection2d::LINESINTERSECT )
		{
			if(info.intno == 1)
			{		
				bool isexist=false;
				std::vector<LoopLineIntData>::iterator ii=intpvec.begin();
				std::vector<LoopLineIntData>::iterator ee=intpvec.end();
				for(ii;ii!=ee;++ii)
				{
					if(ii->m_p.Compare(info.intpoint[0],1.e-7))
						isexist=true;
				}
				if (isexist==false)
				{
					//Calculating loop parameter
					vreal s=info.sofline2[0];
					vreal t0=parameterValues[curri];
					vreal t1=parameterValues[curri+1];
					vreal myt=t0+(t1-t0)*s;
					LoopLineIntData mydata;
					mydata.m_p=info.intpoint[0];
					mydata.m_t=myt;
					mydata.m_loopno =loopNo;
					intpvec.push_back (mydata);
				}
			}
		}
	}	
}

int LoopLineInt::GetPrevNodeParam (const std::vector <vreal> &paramVec , const vreal t)
{
	int mysize=(int)paramVec.size();
	for (int i=0;i<mysize;++i)
	{
		const vreal currt = paramVec[i];
		if (currt < t)
			return i;
	}
	return mysize;
}

bool LoopLineInt::IsPointInLoop ( const VPoint &p
								 , const SequentialLineSet *sl
								 , const VPoint &outp)
{
	VLine2d fake (VPoint2d (outp.x(),outp.y()) , VPoint2d(p.x(),p.y()) );

	PointSet2d rec;
	int intno = 0;

	int mysize = sl->Size();
	for ( int i =0; i < mysize; i++ )
	{
		VLine currline = sl->operator [](i);
		VLine2d currline2d (VPoint2d (currline.GetP1().x(),currline.GetP1().y()) , VPoint2d(currline.GetP2().x(),currline.GetP2().y()));
		LineIntersection2d::LineIntersectionData2d info 
			= LineIntersection2d::CalcInt ( fake , currline2d );	
		if ( info.intinfo == LineIntersection2d::LINESINTERSECT )
		{
			if(info.intno == 1)
			{
				int ind;
				bool ck = rec.GetIndex (info.intpoint[0] , ind , 1.E-5);
				if (ck==false)
				{
					intno++;	
					rec.InsertPoint(info.intpoint[0]);
				}
			}
		}
	}
	if (intno % 2 == 1)
		return true;
	else
		return false;
}

bool LoopLineInt::IsPointInLoop (const VPoint &p , const SequentialLineSet *sl , const VPoint &outp , const CoordinateSystem &planeCs)
{
	VPoint outpt = planeCs.AbsoluteCoordToRelativeCoord(outp);
	VPoint pt = planeCs.AbsoluteCoordToRelativeCoord(p);
	VLine2d fake (VPoint2d (outpt.x(),outpt.y()) , VPoint2d(pt.x(),pt.y()) );

	PointSet2d rec;
	int intno = 0;

	int mysize = sl->Size();
	for ( int i =0; i < mysize; i++ )
	{
		VLine currline = sl->operator [](i);
		VPoint clp1 = planeCs.AbsoluteCoordToRelativeCoord(currline.GetP1());
		VPoint clp2 = planeCs.AbsoluteCoordToRelativeCoord(currline.GetP2());
		VLine2d currline2d (VPoint2d (clp1.x(),clp1.y()) , VPoint2d(clp2.x(),clp2.y()));
		LineIntersection2d::LineIntersectionData2d info 
			= LineIntersection2d::CalcInt ( fake , currline2d );	
		if ( info.intinfo == LineIntersection2d::LINESINTERSECT )
		{
			if(info.intno == 1)
			{
				int ind;
				bool ck = rec.GetIndex (info.intpoint[0] , ind , 1.E-5);
				if (ck==false)
				{
					intno++;	
					rec.InsertPoint(info.intpoint[0]);
				}
			}
		}
	}
	if (intno % 2 == 1)
		return true;
	else
		return false;
}

void LoopLineInt::SingleLoopLineInt (const SequentialLineSet2d *loop , const VLine2d &l , std::map<int,VPoint2d> &res , bool keyIsU , double keyScaleF)
{
	int loopSize = loop->Size();
	for (int i=0;i<loopSize;++i)
	{
		VLine2d curline = loop->operator [](i);
		LineIntersection2d::LineIntersectionData2d info
			= LineIntersection2d::CalcInt(l,curline);
		if (info.intinfo == LineIntersection2d::LINESINTERSECT)
		{
			if (info.intno == 1)
			{
				VPoint2d &ip = info.intpoint[0];
				int ki = keyIsU ? (int) (ip.x() * keyScaleF) : (int) (ip.y() * keyScaleF);
				res.insert (std::pair<int,VPoint2d> (ki , ip));
			}
		}
	}
}

void LoopLineInt::AllLoopsLineInt (const SequentialLineSet2d *outerLoop , const std::vector<SequentialLineSet2d *> &innerLoopVec , const VLine2d &l , std::map<int,VPoint2d> &res , bool keyIsU , double keyScaleF)
{
	SingleLoopLineInt (outerLoop , l , res , keyIsU , keyScaleF);
	size_t mysize = innerLoopVec.size();
	for (size_t i=0;i<mysize;++i)
	{
		SequentialLineSet2d *curloop = innerLoopVec.at (i);
		SingleLoopLineInt (curloop , l , res , keyIsU , keyScaleF);
	}
}
