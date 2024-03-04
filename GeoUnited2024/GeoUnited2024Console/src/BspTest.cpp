// BspTest.cpp

#include "stdafx.h"
#include "BspTest.h"
#include "BspTree.h"
#include "BspTreeBooleanOp.h"
#include "VArc.h"
#include "ArcPointGenerator.h"
#include "SequentialLineSet.h"
#include "DoubleVec.h"
#include "BspTreeConstructor.h"
#include "SequentialLineSet2d.h"
#include "LineSet.h"
#include "BspTreeToLineSet2d.h"
#include "LineSet2d2SequentialLineSet2d.h"
#include "IgesEntitySet.h"

namespace geo
{
	void BspTest::Test()
	{
		VPoint c(0.);
		vreal r1(4.0);
		VPoint a (0.,0.,1);
		VPoint s1 (4.,0.,0.);
		VArc  a1 (c,r1,a,s1,360.0);		
		vreal r2(6.);
		VPoint s2 (6.,0.,0.);
		VArc a2 (c,r2,a,s2,360.);
		IgesEntitySet entset;
		int ia1 = a1.MakeAndAddIgesArc (entset);
		ArcPointGenerator mygen1(*(igesArc*)entset[ia1],entset);
		vreal tol (0.06);
		SequentialLineSet res1(100);
		DoubleVec par1;
		mygen1.Digitise (tol,res1,par1);
		SequentialLineSet res2(100);
		DoubleVec par2;
		int ia2 = a2.MakeAndAddIgesArc (entset);
		ArcPointGenerator mygen2(*(igesArc *)entset[ia2],entset);
		mygen2.Digitise (tol,res2,par2);
		BspTreeConstructor myconstructor;
		SequentialLineSet2d sl1 (res1);
		SequentialLineSet2d sl2 (res2);
		BspTree *b1 = myconstructor.ConstructTree (sl1);
		BspTree *b2 = myconstructor.ConstructTree (sl2);
		BspTree *b3 = BspTreeBooleanOp::Subtract (*b1,*b2);
		BspTree *b4 = BspTreeBooleanOp::Unite (*b3,*b2);
		BspTree *b5 = BspTreeBooleanOp::Subtract (*b4,*b3);
		VPoint c2 (50.,0.,0.);
		VPoint ss (50.+r2,0.,0.);
		VArc a3 (c2,r2,a,ss,360.);
		SequentialLineSet fararc (205);
		int ia3 = a3.MakeAndAddIgesArc (entset);
		ArcPointGenerator mygen3 (*(igesArc *)entset[ia3],entset);
		DoubleVec par3;
		mygen3.Digitise (tol,fararc,par3);
		SequentialLineSet2d fararc2d(fararc);
	
		BspTree *bfar = myconstructor.ConstructTree (fararc2d);
		BspTree * b6 = BspTreeBooleanOp::Unite (*b5,*bfar);

		BspTreeToLineSet2d b2sl;
		LineSet2d *ls2d  = b2sl.Make (*b6);
		LineSet2d2SequentialLineSet2d conv;
		std::vector<SequentialLineSet2d *> myres = conv.Make (*ls2d);

		int stp(0);
		stp=1;
	}

};
