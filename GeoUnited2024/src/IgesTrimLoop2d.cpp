// IgesTrimLoop2d.cpp

#include "stdafx.h"
#include "IgesTrimLoop2d.h"
#include "FakeSLSet.h"
#include "OrderSequentialLineSets.h"
#include "IgesEntitySet.h"
#include "igesTransformationMatrix.h"
#include "TransformationMatrix.h"

void IgesTrimLoop2d::Order (IgesEntitySet *entSet)
{
	//Building slVec
	int mysize = static_cast<int>(loopcurves.size());
	std::vector<FakeSLSet>slVec (mysize,4);
	for (int i=0;i<mysize;i++)
	{
		IgesEntity *current = loopcurves[i];
		TransformationMatrix tm;
		if (current->DE.pXform !=0)
		{
			IgesEntity *itme = entSet->GetEntityPointer(current->DE.pXform);		
			igesTransformationMatrix *itm = dynamic_cast<igesTransformationMatrix *> (itme);
			if (itm == 0)
				throw VException ("IgesTrimLoop2d::Order exception , matrix conversion failed",1);
			tm = TransformationMatrix (*itm);
		}
		FakeSLSet &sl = slVec[i];
		sl.Initialise (current,tm);
	}
	//orderingSLVec
	OrderSequentialLineSets<FakeSLSet , VPoint> myord;
	FakeSLSet * newsl = myord.MakeOrderedUnion (slVec); 
	//reOrderingLoop
	std::vector<IgesEntity *>newloop;
	std::vector<bool>newisreversed;
	
	std::list<const FakeSLSet *>&child = newsl->GetChild();
	std::list<const FakeSLSet *>::iterator i = child.begin();
	std::list<const FakeSLSet *>::iterator e = child.end();
	for (i;i!=e;i++)
	{
		const FakeSLSet * currsl = *i;
		IgesEntity *current = currsl->GetEntity();
		newloop.push_back (current);
		bool currrev = currsl->IsReversed();
		newisreversed.push_back (currrev);
	}
	loopcurves = newloop;
	m_isReversed =newisreversed;
}
