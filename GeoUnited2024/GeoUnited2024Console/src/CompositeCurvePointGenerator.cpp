// CompositeCurvePointGenerator.cpp

#include "stdafx.h"
#include "CompositeCurvePointGenerator.h"
#include "VPoint.h"
#include "DoubleVec.h"
#include "IgesEntity.h"
#include "IgesEntitySet.h"

CompositeCurvePointGenerator::CompositeCurvePointGenerator (IgesEntity &ent , IgesEntitySet &allent)
{
	//m_curvecount=(unsigned int)(m_curves.m_allcurves.size());
	//m_singleRange=1./m_curvecount;
}

CompositeCurvePointGenerator::~CompositeCurvePointGenerator()
{
	for (unsigned int i=0;i<m_allcurves.size();i++)
	{
		CurvePointGenerator *currc=m_allcurves[i];
		delete currc;
	}
}

VPoint CompositeCurvePointGenerator::GetPointDerived (const double &v)const
{
	return VPoint(0.,0.,0.);
}

void CompositeCurvePointGenerator::GetDefIntervalsDerived (DoubleVec &defint)const
{
	for (unsigned int j=0;j<m_curvecount;j++)
	{
		_CurvePointGenerator * currc=m_curves.m_allcurves[j];
		DoubleVec tmp;
		currc->GetDefIntervals(tmp);
		for (int k=0;k<tmp.Size();k++)
		{
			double currp=tmp[k];
			double absPar=mem2abs(currp,j);
			defint.AddDouble(absPar);
		}
	}
}
	
double CompositeCurvePointGenerator::abs2mem(const double &absolutePar,int &curveNo)const
{
	curveNo=(int)(absolutePar/m_singleRange);
	double tmp=absolutePar-(curveNo*m_singleRange);
	double mempar=tmp/m_singleRange;
	return mempar;	
}

double CompositeCurvePointGenerator::mem2abs(const double &memPar,const int &curveNo)const
{
	double tmp=curveNo*m_singleRange;
	double tmp2=memPar*m_singleRange;
	double absPar=tmp+tmp2;	
	return absPar;	
}
