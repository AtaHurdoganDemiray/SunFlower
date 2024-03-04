// MultiPointGeneratorForCompositeCurve.cpp

#include "stdafx.h"
#include "MultiPointGeneratorForCompositeCurve.h"
#include "VPoint.h"

namespace geo
{
	MultiPointGeneratorForCompositeCurve::MultiPointGeneratorForCompositeCurve()
	{
		m_sequential=true;
		m_done=false;
	}

	MultiPointGeneratorForCompositeCurve
	::~MultiPointGeneratorForCompositeCurve()
	{
		for (unsigned int i=0;i<m_allcurves.size();i++)
		{
			_CurvePointGenerator *currc=m_allcurves[i];
			delete currc;
		}
	}

	void MultiPointGeneratorForCompositeCurve::Add(_CurvePointGenerator *newc)
	{
		_CurvePointGenerator *lastc=m_allcurves.back();
		bool ck=CheckIfSeq(lastc,newc);
		if(ck==false)
			m_sequential=false;
		m_allcurves.push_back(newc);
	}

	const double parbegin=0.0;
	const double parend=1.0;
	const double seqtol=1.e-6;

	bool MultiPointGeneratorForCompositeCurve::CheckIfSeq 
		(_CurvePointGenerator *lastcurve,_CurvePointGenerator *newcurve)const
	{
		VPoint lastp=lastcurve->GetPoint(parend);
		VPoint nextp=newcurve->GetPoint(parbegin);
		bool ck= lastp.Compare(nextp,seqtol);
		if(ck)
			return true;
		VPoint revnextp=newcurve->GetPoint(parend);
		ck=lastp.Compare(revnextp,seqtol);
		if(ck)
		{
			newcurve->Reverse();
			return true;
		}
		return false;
	}	

	void MultiPointGeneratorForCompositeCurve::FinishAdding()
	{
		m_done=true;
	}

	//void MultiPointGeneratorForCompositeCurve::PutInOrder()
	//{
	//
	//}

	bool MultiPointGeneratorForCompositeCurve::IsSequential()const
	{
		return m_sequential;
	}
	
	bool MultiPointGeneratorForCompositeCurve::IsDone()const
	{
		return m_done;
	}
};
