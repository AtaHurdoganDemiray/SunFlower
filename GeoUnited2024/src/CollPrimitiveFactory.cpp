// CollPrimitiveFactory.cpp

#include "stdafx.h"
#include "CollPrimitiveFactory.h"
#include "mwCollCylinder.hpp"
#include "mwCollCone.hpp"
#include "mwCollSphere.hpp"
#include "mwCollTorus.hpp"
#include "ToolLine.h"
#include "ToolCircle.h"
#include "ToolVerticalLine.h"

CollPrimitiveFactory::CollPrimitiveFactory ()
:m_offtype (cc::OFFTYPE_ROUGH)
,m_tol(measures::mwUnitsFactory::METRIC,0.01)
{

}

void CollPrimitiveFactory::Make (const vTool<float> &src,std::vector<cc::mwCollPrimitiveGeo *> &primVec)
{
	m_products = &primVec;
	cc::mwTolerance tmptol(measures::mwUnitsFactory::METRIC,0.01);
	int mysize = src.Size();
	for (int i=0;i<mysize;++i)
	{
		const ToolPrim<float> *currtp = src[i];
		currtp->AcceptVisitor (*this);
	}
}

void CollPrimitiveFactory::Visit(const ToolPrim<float> &visitor)
{
	throw VException (_T( IDS_ColVisByBasCla),1);
}

void CollPrimitiveFactory::Visit(const ToolVerticalLine<float> &visitor) 
{
	// Add a collcylinder
	vreal disttotip = static_cast<double> (visitor.GetLowerPoint()[1]);
	vreal cylinder_rad = static_cast<double> (visitor.GetLowerPoint()[0]);
	vreal cylinder_height = static_cast<double> (visitor.GetHeight());

	cc::mwCollCylinder *nc = new cc::mwCollCylinder (disttotip
													,cylinder_rad
													,cylinder_height
													,m_offtype
													,m_tol);
	m_products->push_back (nc);
}

void CollPrimitiveFactory::Visit(const ToolLine<float> &visitor)
{
	vreal disttotip = static_cast<double> (visitor.GetLowerPoint()[1]);
	vreal bot_rad = static_cast<double> (visitor.GetLowerPoint()[0]);
	vreal top_rad = static_cast<double> (visitor.GetUpperPoint()[0]);
	vreal cone_height = static_cast<double>(visitor.GetHeight());

	cc::mwCollCone *nc = new cc::mwCollCone (disttotip , bot_rad , top_rad , cone_height , m_offtype , m_tol);
	m_products->push_back (nc);
}

void CollPrimitiveFactory::Visit(const ToolCircle<float> &visitor)
{
	sVector<float,2> cen = visitor.GetCenter();
	bool isSph;
	if (fabs (cen[0]) < 1.e-6)
		isSph=true;
	else
		isSph=false;

	if (isSph)
	{
		vreal sph_rad = static_cast<double>(visitor.GetRadius());
		vreal disttotip = static_cast<double>(cen[1]) - sph_rad;
		cc::mwCollSphere *ns = new cc::mwCollSphere (disttotip , sph_rad,m_tol);
		m_products->push_back (ns);
	}
	else
	{
		vreal tiprad = static_cast<double>(visitor.GetRadius());
		vreal torusrad = static_cast<double>(cen[0]) + tiprad;
		vreal disttotip = static_cast<double>(cen[1]) - tiprad;	
		cc::mwCollTorus *nt = new cc::mwCollTorus (disttotip,torusrad,tiprad,m_tol);
		m_products->push_back (nt);
	}
}
