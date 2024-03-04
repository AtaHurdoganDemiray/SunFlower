// VRefTriangleMesh.cpp

#include "stdafx.h"
#include "VRefTriangleMesh.h"
#include "RefTriangle.h"
#include "VPoint.h"
#include "VTriangleE.h"
#include "VTriangle2dE.h"

#include "DoubleList.h"
#include "VLine.h"
//#include "..\image\texture.h"
#include "LineSet.h"
#include "PointSet.h"
#include "..\image\VTexture.h"
#include "VPlane.h"
#include "TriangleIntersection.h"
#include "SequentialLineSet.h"
#include "ReorientMatrix.h"

int VRefTriangleMesh::VTRI_MAX_SL_COUNT = 1200;

VRefTriangleMesh::VRefTriangleMesh ( const int &initialsize )
: rdata ( initialsize ) , surfpack ( initialsize )
{
	//m_min=GETMAXPOINT();
	//m_max=GETMINPOINT();
}
VRefTriangleMesh::VRefTriangleMesh(const int initialPointSize,const int initialTriSize)
:rdata(initialTriSize),surfpack(initialPointSize)
{

}

//long VRefTriangleMesh::Size() const
//{
//	return rdata.Size();
//}

VRefTriangleMesh::VRefTriangleMesh ( const VRefTriangleMesh & Foo )
:rdata ( Foo.rdata ) ,surfpack ( Foo.surfpack )
{
	//m_min= Foo.m_min;
	//m_max = Foo.m_max;
}

int VRefTriangleMesh::AddTriangle ( const RefTriangle &newtri )
{
	rdata.AddTriangle ( newtri );
	return (rdata.Size()-1);
}

//void VRefTriangleMesh::UpdateMinMaxForTri ( const RefTriangle &newtri )
//{
//	VPoint * c1 = surfpack.modelpoints[newtri.ref0];
//	VPoint * c2 = surfpack.modelpoints[newtri.ref1];
//	VPoint * c3 = surfpack.modelpoints[newtri.ref2];
//	
//	m_min.ReplaceMin ( *c1 );
//	m_min.ReplaceMin ( *c2 );
//	m_min.ReplaceMin ( *c3 );
//
//	m_max.ReplaceMax ( *c1 );
//	m_max.ReplaceMax ( *c2 );
//	m_max.ReplaceMax ( *c3 );
//}

const VPoint& VRefTriangleMesh::GetCor1Normal( const long &i ) const
{
	const RefTriangle *rt = (rdata)[i];
	const VPoint *nrml = surfpack.normals[rt->ref0];
	return *nrml;
	
}

const VPoint& VRefTriangleMesh::GetCor2Normal( const long &i ) const
{
	const RefTriangle *rt = (rdata)[i];
	const VPoint *nrml = surfpack.normals[rt->ref1];
	return *nrml;
}


const VPoint& VRefTriangleMesh::GetCor3Normal( const long &i ) const
{
	const RefTriangle *rt = (rdata)[i];
	const VPoint *nrml = surfpack.normals[rt->ref2];
	return *nrml;
}

const VPoint2d& VRefTriangleMesh::GetParamTriCor1 ( const long &i )const
{
	const RefTriangle *rt = (rdata)[i];
	const VPoint2d *cp = surfpack.parampoints[rt->ref0];
	return *cp;

}

const VPoint2d& VRefTriangleMesh::GetParamTriCor2 ( const long &i )const
{
	const RefTriangle *rt = (rdata)[i];
	const VPoint2d *cp = surfpack.parampoints[rt->ref1];
	return *cp;	
}

const VPoint2d& VRefTriangleMesh::GetParamTriCor3 ( const long &i )const
{

	const RefTriangle *rt = (rdata)[i];
	const VPoint2d *cp = surfpack.parampoints[rt->ref2];
	return *cp;
}

const VPoint& VRefTriangleMesh::GetModelTriCor1 ( const long &i )const
{
	const RefTriangle *rt = (rdata)[i];
	const VPoint *cp = surfpack.modelpoints[rt->ref0];
	return *cp;	
}

const VPoint& VRefTriangleMesh::GetModelTriCor2 ( const long &i )const
{
	const RefTriangle *rt = (rdata)[i];
	const VPoint *cp = surfpack.modelpoints[rt->ref1];
	return *cp;	
}

const VPoint& VRefTriangleMesh::GetModelTriCor3 ( const long &i )const
{
	const RefTriangle *rt = (rdata)[i];
	const VPoint *cp = surfpack.modelpoints[rt->ref2];
	return *cp;
}

//void VRefTriangleMesh::DrawWithTexture ( const texture *mytexture ) const
//{
//	glDisable ( GL_LIGHTING );
//	glShadeModel (GL_SMOOTH);
//	glEnable( GL_TEXTURE_2D);
//
//	glBindTexture( GL_TEXTURE_2D, mytexture->mtext.TextureID  );		
//	glBegin( GL_TRIANGLES ) ;
//
//	for ( long i=0; i < Size(); i++ )
//	{	
//		glNormal3dv ( GetCor1Normal(i).GetArray() );
//		
//		//VPoint2d uv1 ( GetParamTriCor1(i) );
//		//VPoint2d uv2 ( GetParamTriCor2 (i));
//		//VPoint2d uv3 (GetParamTriCor3 (i));
//		//PeriodicModifyTextureUV (uv1,uv2,uv3);
//		
//		glTexCoord2dv( GetParamTriCor1(i).GetArray() ); glVertex3dv ( GetModelTriCor1(i).GetArray() );
//		
//		glNormal3dv ( GetCor2Normal(i).GetArray() );
//		
//		glTexCoord2dv( GetParamTriCor2(i).GetArray() );  glVertex3dv( GetModelTriCor2(i).GetArray() );
//		
//		glNormal3dv ( GetCor3Normal(i).GetArray() );
//
//		glTexCoord2dv ( GetParamTriCor3(i).GetArray() ); glVertex3dv( GetModelTriCor3(i).GetArray() );
//		
//	}
//
//	glEnd();
//
//	glDisable( GL_TEXTURE_2D);
//	glEnable ( GL_LIGHTING );
//	glDisable ( GL_POLYGON_STIPPLE );
//
//}
void VRefTriangleMesh::DrawWithTexture ( const VTexture *mytexture ) const
{
	glDisable ( GL_LIGHTING );
	glShadeModel (GL_SMOOTH);
	glEnable( GL_TEXTURE_2D);

	glBindTexture( GL_TEXTURE_2D, mytexture->GetId());		
	glBegin( GL_TRIANGLES ) ;

	for ( long i=0; i < Size(); i++ )
	{	
		glNormal3dv ( GetCor1Normal(i).GetArray() );
		
		//VPoint2d uv1 ( GetParamTriCor1(i) );
		//VPoint2d uv2 ( GetParamTriCor2 (i));
		//VPoint2d uv3 (GetParamTriCor3 (i));
		//PeriodicModifyTextureUV (uv1,uv2,uv3);
		
		glTexCoord2dv( GetParamTriCor1(i).GetArray() ); glVertex3dv ( GetModelTriCor1(i).GetArray() );
		
		glNormal3dv ( GetCor2Normal(i).GetArray() );
		
		glTexCoord2dv( GetParamTriCor2(i).GetArray() );  glVertex3dv( GetModelTriCor2(i).GetArray() );
		
		glNormal3dv ( GetCor3Normal(i).GetArray() );

		glTexCoord2dv ( GetParamTriCor3(i).GetArray() ); glVertex3dv( GetModelTriCor3(i).GetArray() );
		
	}

	glEnd();

	glDisable( GL_TEXTURE_2D);
	glEnable ( GL_LIGHTING );
	glDisable ( GL_POLYGON_STIPPLE );

}
//#############################################################################

//VPoint VRefTriangleMesh::GetMin () const
//{
//	const VPoint &result = surfpack.modelpoints.GetMin();
//	return result;
//	//return m_min;
//}

//#############################################################################

//VPoint VRefTriangleMesh::GetMax () const
//{
//	const VPoint &result = surfpack.modelpoints.GetMax();
//	return result;
//	//return m_max;
//}

//#############################################################################

//void VRefTriangleMesh::UpdateMinMax () 
//{
//	surfpack.modelpoints.UpdateMinMax();
//}

//#############################################################################

vreal VRefTriangleMesh::GetArea () const
{
	vreal totalA=0.;

	for ( int i=0; i < Size(); i++ )
	{
		VTriangleE tri ( GetModelTriCor1(i) , GetModelTriCor2(i) , GetModelTriCor3(i) ); 
		vreal currA = tri.GetArea();
		totalA += currA;
	}	
	return totalA;
}

bool VRefTriangleMesh::operator == (const VRefTriangleMesh &toCompare)const
{
	if(this==&toCompare)
		return true;

	//if(m_min!=toCompare.m_min)
	//	return false;
	//if(m_max!=toCompare.m_max)
	//	return false;
	if(rdata!=toCompare.rdata)
		return false;
	if(surfpack!=toCompare.surfpack)
		return false;

	return true;
}

bool VRefTriangleMesh::operator != (const VRefTriangleMesh &toCompare)const
{
	return !(this->operator ==(toCompare));
}

void VRefTriangleMesh::Serialize (Bofstream &outfile)const
{
	int version = 1;
	outfile.write ((VChr*)&version , sizeof(int));
	if (version ==1)
	{
		//m_min.Serialize(outfile);
		//m_max.Serialize(outfile);
		rdata.Serialize (outfile);
		surfpack.Serialize (outfile);
	}
	else
	{
		throw VException (( IDS_VReSerExc),1);
	}
}
void VRefTriangleMesh::Serialize (Bifstream &infile)
{
	int version;
	infile.read ((VChr *)&version , sizeof (int));

	if (version == 1)
	{
		//m_min.Serialize (infile);
		//m_max.Serialize (infile);
		rdata.Serialize (infile);
		surfpack.Serialize (infile);
	}
	else
	{
		throw VException (( IDS_VReSerExc1),2);
	}
}

const RefTriangle & VRefTriangleMesh::GetRefTri (const long &i )const
{
	return *(rdata.operator [] (i));
}

RefTriangleMesh & VRefTriangleMesh::GetRefTriangleMesh()
{
	return rdata;
}

void VRefTriangleMesh::SetRefTri (const long &i , const RefTriangle &newtri)
{
	RefTriangle *t = rdata.operator [](i);
	(*t) = newtri;
}

void VRefTriangleMesh::CleanTrimmedTriangles()
{
	RefTriangleMesh tmp(rdata);
	rdata.Clear();
	for (int i=0;i<tmp.Size();i++)
	{
		RefTriangle *t = tmp[i];
		if (t->ref0 == -1 && t->ref1 == -1 && t->ref2 == -1)
			continue;
		rdata.AddTriangle (*t);
	}
}

const VTriangleE VRefTriangleMesh::operator[] (int i)const
{
	const VPoint &p1 = GetModelTriCor1(i);
	const VPoint &p2 = GetModelTriCor2(i);
	const VPoint &p3 = GetModelTriCor3(i);
	VTriangleE res (p1,p2,p3);
	return res;
}
VTriangleE VRefTriangleMesh::operator[] (int i)
{
	const VPoint &p1 = GetModelTriCor1(i);
	const VPoint &p2 = GetModelTriCor2(i);
	const VPoint &p3 = GetModelTriCor3(i);
	VTriangleE res (p1,p2,p3);
	return res;
}
const VTriangle VRefTriangleMesh::GetTri (int i)const
{
	const VPoint &p1 = GetModelTriCor1(i);
	const VPoint &p2 = GetModelTriCor2(i);
	const VPoint &p3 = GetModelTriCor3(i);
	VTriangle res (p1,p2,p3);
	return res;
}
int VRefTriangleMesh::Size()const
{
	return rdata.Size();
}
const VPoint VRefTriangleMesh::GetMin()const
{
	const VPoint &result = surfpack.modelpoints.GetMin();
	return result;
}
const VPoint VRefTriangleMesh::GetMax()const
{
	const VPoint &result = surfpack.modelpoints.GetMax();
	return result;
}
VTriangle2dE VRefTriangleMesh::GetParTri(int index)const
{
	VPoint2d p1 = GetParamTriCor1(index);
	VPoint2d p2 = GetParamTriCor2(index);
	VPoint2d p3 = GetParamTriCor3(index);
	VTriangle2dE res (p1,p2,p3);
	return res;
}
void VRefTriangleMesh::Transform(const TransformationMatrix &tm)
{
	surfpack.Transform (tm);
}

void VRefTriangleMesh::CalculateNormals ()
{
	int mysize = Size ();
	for (int i=0;i<mysize;i++)
	{
		const RefTriangle &rt = GetRefTri (i);
		const VPoint & p1 = * surfpack.modelpoints[rt.ref0];
		const VPoint & p2 = * surfpack.modelpoints[rt.ref1];
		const VPoint & p3 = * surfpack.modelpoints[rt.ref2];
		VPoint n = (p2 - p1) ^ (p3 - p1);
		n.NormThis();
		VPoint *n1 = surfpack.normals[rt.ref0];
		VPoint *n2 = surfpack.normals[rt.ref1];
		VPoint *n3 = surfpack.normals[rt.ref2];
		if ( n1->x() == -1. && n1->y() == -1. && n1->z() == -1.)
			*n1 = n;
		else
		{
			*n1 = (*n1 + n)*0.5;
			n1->NormThis();
		}
		if ( n2->x() == -1. && n2->y() == -1. && n2->z() == -1.)
			*n2 = n;
		else
		{
			*n2 = (*n2 + n)*0.5;
			n2->NormThis();
		}
		if ( n3->x() == -1. && n3->y() == -1. && n3->z() == -1.)
			*n3 = n;
		else
		{
			*n3 = (*n3 + n)*0.5;
			n3->NormThis();
		}
	}
}

vreal VRefTriangleMesh::GetAreaProjXY ()const
{
	vreal area = 0.;
	int mysize = Size ();
	for (int i=0;i<mysize;i++)
	{
		const RefTriangle &rt = GetRefTri (i);
		const VPoint & p1 = * surfpack.modelpoints[rt.ref0];
		const VPoint & p2 = * surfpack.modelpoints[rt.ref1];
		const VPoint & p3 = * surfpack.modelpoints[rt.ref2];
		VPoint p11 (p1.x(),p1.y(),0.0);
		VPoint p22 (p2.x(),p2.y(),0.0);
		VPoint p33 (p3.x(),p3.y(),0.0);
		VTriangleE t (p11,p22,p33);
		area += t.GetArea();
	}
	return area;
}	
vreal VRefTriangleMesh::GetAreaProjXZ ()const
{
	vreal area = 0.;
	int mysize = Size ();
	for (int i=0;i<mysize;i++)
	{
		const RefTriangle &rt = GetRefTri (i);
		const VPoint & p1 = * surfpack.modelpoints[rt.ref0];
		const VPoint & p2 = * surfpack.modelpoints[rt.ref1];
		const VPoint & p3 = * surfpack.modelpoints[rt.ref2];
		VPoint p11 (p1.x(),0.0,p1.z());
		VPoint p22 (p2.x(),0.0,p2.z());
		VPoint p33 (p3.x(),0.0,p3.z());
		VTriangleE t (p11,p22,p33);
		area += t.GetArea();
	}
	return area;
}	
vreal VRefTriangleMesh::GetAreaProjYZ ()const
{
	vreal area = 0.;
	int mysize = Size ();
	for (int i=0;i<mysize;i++)
	{
		const RefTriangle &rt = GetRefTri (i);
		const VPoint & p1 = * surfpack.modelpoints[rt.ref0];
		const VPoint & p2 = * surfpack.modelpoints[rt.ref1];
		const VPoint & p3 = * surfpack.modelpoints[rt.ref2];
		VPoint p11 (0.0,p1.y(),p1.z());
		VPoint p22 (0.0,p2.y(),p2.z());
		VPoint p33 (0.0,p3.y(),p3.z());
		VTriangleE t (p11,p22,p33);
		area += t.GetArea();
	}
	return area;
}

int VRefTriangleMesh::AddQuad(const VRefTriangleMesh::RefQuad &q)
{
	m_quad.push_back(q);
	return (int)m_quad.size()-1;
}

void VRefTriangleMesh::Clear()
{
	surfpack.Clear();
	rdata.Clear();
}

void VRefTriangleMesh::PeriodicModifyTextureUV ( VPoint2d &uv1
												,VPoint2d &uv2
												,VPoint2d &uv3)const
{
	VPoint2d uv1tmp (uv1.x() - floor (uv1.x()) , uv1.y() - floor (uv1.y()) );
	VPoint2d uv2tmp (uv2.x() - floor(uv2.x()) , uv2.y() - floor(uv2.y()) );
	VPoint2d uv3tmp (uv3.x() - floor(uv3.x()) , uv3.y() - floor(uv3.y()) );
	uv1 = uv1tmp;
	uv2 = uv2tmp;
	uv3 = uv3tmp;
	
	//double mydeltax = uv.x() > 0.5 ? -1.e-6 : 1.e-6;
	//double mydeltay = uv.y() > 0.5 ? -1.e-6 : 1.e-6;
	//VPoint2d tmpp1 ( uv.x() - floor(uv.x()+mydeltax) , uv.y() - floor(uv.y()+mydeltay));
	//return tmpp1;
}

VPoint VRefTriangleMesh::GetMinPointInCs (const CoordinateSystem &cs) const
{
	VPoint mnInCs (MAX_NUMBER_LIMIT);
	int mysize = surfpack.modelpoints.Size();
	for (int i=0;i<mysize;++i)
	{
		const VPoint *p = surfpack.modelpoints.operator[](i);
		mnInCs.ReplaceMin (cs.AbsoluteCoordToRelativeCoord(*p));
	}
	return mnInCs;
}

VPoint VRefTriangleMesh::GetMaxPointInCs (const CoordinateSystem &cs) const
{
	VPoint mxInCs (MIN_NUMBER_LIMIT);
	int mysize = surfpack.modelpoints.Size();
	for (int i=0;i<mysize;++i)
	{
		const VPoint *p = surfpack.modelpoints.operator[](i);
		mxInCs.ReplaceMax (cs.AbsoluteCoordToRelativeCoord (*p));
	}
	return mxInCs;
}

void VRefTriangleMesh::AddTriangleToSurfPackAndRefTri (const VTriangleE &tri)
{
	surfpack.modelpoints.InsertPoint(tri.GetCorner1());
	surfpack.modelpoints.InsertPoint(tri.GetCorner2());
	surfpack.modelpoints.InsertPoint(tri.GetCorner3());
	surfpack.normals.InsertPoint(tri.GetNormal());
	surfpack.normals.InsertPoint(tri.GetNormal());
	surfpack.normals.InsertPoint(tri.GetNormal());
	surfpack.parampoints.InsertPoint(VPoint2d(0.0,0.0));
	surfpack.parampoints.InsertPoint(VPoint2d(0.0,0.0));
	surfpack.parampoints.InsertPoint(VPoint2d(0.0,0.0));
	int ofset = surfpack.modelpoints.Size()-3;
	AddTriangle (RefTriangle (ofset,ofset+1,ofset+2));
}

void VRefTriangleMesh::FindEdges(LineSet &ls)
{
	int trino = this->GetRefTriangleMesh().Size();
	std::multimap<int,TriangleEdge> edgeMap;
	for (int i=0;i<trino;++i)
	{
		const RefTriangle *curtri = rdata.operator[](i);
		TriangleEdge e1,e2,e3;
		e1.m_trino = e2.m_trino = e3.m_trino = i;
		e1.m_vertex1i = curtri->ref0;
		e1.m_vertex2i = curtri->ref1;
		e2.m_vertex1i = curtri->ref1;
		e2.m_vertex2i = curtri->ref2;
		e3.m_vertex1i = curtri->ref2;
		e3.m_vertex2i = curtri->ref0;
		edgeMap.insert(std::pair<int,TriangleEdge>(e1.m_vertex1i*e1.m_vertex2i,e1));
		edgeMap.insert(std::pair<int,TriangleEdge>(e2.m_vertex1i*e2.m_vertex2i,e2));
		edgeMap.insert(std::pair<int,TriangleEdge>(e3.m_vertex1i*e3.m_vertex2i,e3));
	}
	std::multimap<int,TriangleEdge>::iterator ei = edgeMap.begin();
	std::multimap<int,TriangleEdge>::iterator ee = edgeMap.end();
	typedef std::multimap<int,TriangleEdge>::iterator itr;
	for (ei;ei!=ee;++ei)
	{
		if ((*ei).second.m_hasNeighbor == false)
		{
			std::pair<itr,itr> mysrc = edgeMap.equal_range (ei->first);
			for (itr si=mysrc.first;si!=mysrc.second;++si)
			{
				if ( &(*ei).second == &(*si).second )	//edge's itself
					continue;
				if ( (*ei).second.m_vertex1i == (*si).second.m_vertex1i && (*ei).second.m_vertex2i == (*si).second.m_vertex2i)
				{
					(*ei).second.m_hasNeighbor = true;
					(*si).second.m_hasNeighbor = true;
				}
				else if( (*ei).second.m_vertex1i == (*si).second.m_vertex2i && (*ei).second.m_vertex2i == (*si).second.m_vertex1i)
				{
					(*ei).second.m_hasNeighbor = true;
					(*si).second.m_hasNeighbor = true;
				}
			}
		}
	}
	ei = edgeMap.begin();
	for (ei;ei!=ee;++ei)
	{
		if ( (*ei).second.m_hasNeighbor == false)
		{
			const VPoint &p1 = surfpack.GetModelPoint ((*ei).second.m_vertex1i);
			const VPoint &p2 = surfpack.GetModelPoint ((*ei).second.m_vertex2i);
			VLine l (p1,p2);
			ls.AddLine (l);
		}
	}
}

void VRefTriangleMesh::MakeYSections(const std::map<vreal, vreal> coordMap, const CoordinateSystem &cs
	, std::multimap<vreal, SequentialLineSet *> &sections, const std::map<vreal, std::pair<vreal, vreal>> &boundary, vreal tol)const
{
	std::map<vreal, std::multimap<vreal, VLine>> section;
	std::map<vreal, std::multimap<vreal, vreal>> rsection;
	int trisize = this->Size();
	if (fabs(tol) < EPSILON)
		throw VException("VTriangles::MakeXSections exception , tol is zero", 1);

	//vreal oneOverTol = 1. / tol;
	vreal halftol = tol * 0.3;
	for (int ti = 0; ti < trisize; ++ti)
	{
		VTriangle curtri = this->GetTri(ti);
		VPoint tc1 = cs.AbsoluteCoordToRelativeCoord(curtri.GetCorner1());
		VPoint tc2 = cs.AbsoluteCoordToRelativeCoord(curtri.GetCorner2());
		VPoint tc3 = cs.AbsoluteCoordToRelativeCoord(curtri.GetCorner3());
		VTriangleE ttri(tc1, tc2, tc3);
		VPoint tmin = ttri.GetMin();
		VPoint tmax = ttri.GetMax();
		std::map<vreal, vreal>::const_iterator ci = coordMap.begin();
		std::map<vreal, vreal>::const_iterator ce = coordMap.end();
		for (ci; ci != ce; ++ci)
		{
			vreal cplaned = ci->first;
			if (cplaned <= tmax.y() && cplaned >= tmin.y())
			{
				tc1.y(cplaned);
				VPlane sep(tc1, UY);
				VLine intl;
				bool ck = TriangleIntersection::Int(ttri, sep, intl);
				if (ck)
				{
					if (intl.GetV().Compare(VPoint(0.0), halftol) == false)
					{
						//int p2i = (int)(intl.GetP2().y()*oneOverTol);
						//section[cplanei].operator[](p1i) = intl.GetP1();
						//section[cplanei].operator[](p2i) = intl.GetP2();
						if (intl.GetP1().x() > intl.GetP2().x())
							intl.Reverse();
						bool ckIsIn = XTrimLineIf(intl, boundary.at(cplaned));
						if (ckIsIn)
						{
							vreal p1d = intl.GetP1().x();
							vreal p2d = intl.GetP2().x();
							//section[cplanei].operator[](p1i)=intl;
							section[cplaned].insert(std::pair<vreal, VLine>(p1d, intl));
							rsection[cplaned].insert(std::pair<vreal, vreal>(p2d, p1d));
						}
					}
				}
			}
		}
	}
	std::map<vreal, std::multimap<vreal, VLine>>::iterator ssi = section.begin();
	std::map<vreal, std::multimap<vreal, VLine>>::iterator sse = section.end();
	std::map<vreal, std::multimap<vreal, vreal>>::iterator rsi = rsection.begin();
	CoordinateSystem abs;
	ReorientMatrix Re(abs, cs);
	for (ssi; ssi != sse; ++ssi)
	{
		SequentialLineSet *sl = 0;
		int counter = 0;
		do
		{
			sl = OneLoopY(ssi->second, rsi->second, boundary, tol);
			if (sl)
			{
				sl->Transform(Re);
				sections.insert(std::pair<vreal, SequentialLineSet *>(sl->Start()->y(), sl));
			}
			++counter;
		} while (sl != 0 && counter < VTRI_MAX_SL_COUNT);
		++rsi;
	}
}

bool VRefTriangleMesh::XTrimLineIf(VLine &l, const std::pair<vreal, vreal> &curpair)
{
	if (fabs(curpair.first - MAX_NUMBER_LIMIT) < EPSILON || fabs(curpair.second - MIN_NUMBER_LIMIT) < EPSILON)
		return false;
	if (l.GetP1().x() < curpair.first - EPSILON && l.GetP2().x() < curpair.first - EPSILON)
		return false;
	if (l.GetP1().x() > curpair.second + EPSILON && l.GetP2().x() > curpair.second + EPSILON)
		return false;
	if (l.GetP1().x() < curpair.first)
	{
		vreal deltax = l.GetP2().x() - l.GetP1().x();
		vreal dx = curpair.first - l.GetP1().x();
		vreal newz = l.GetP1().z() + (l.GetP2().z() - l.GetP1().z()) * dx / deltax;
		l.GetP1().x(curpair.first);
		l.GetP1().z(newz);
	}
	if (l.GetP1().x() > curpair.second)
	{
		vreal deltax = l.GetP2().x() - l.GetP1().x();
		vreal dx = curpair.second - l.GetP1().x();
		vreal newz = l.GetP1().z() + (l.GetP2().z() - l.GetP1().z()) * dx / deltax;
		l.GetP1().x(curpair.second);
		l.GetP1().z(newz);
	}
	if (l.GetP2().x() < curpair.first)
	{
		vreal deltax = l.GetP2().x() - l.GetP1().x();
		vreal dx = curpair.first - l.GetP1().x();
		vreal newz = l.GetP1().z() + (l.GetP2().z() - l.GetP1().z()) * dx / deltax;
		l.GetP2().x(curpair.first);
		l.GetP2().z(newz);
	}
	if (l.GetP2().x() > curpair.second)
	{
		vreal deltax = l.GetP2().x() - l.GetP1().x();
		vreal dx = curpair.second - l.GetP1().x();
		vreal newz = l.GetP1().z() + (l.GetP2().z() - l.GetP1().z()) * dx / deltax;
		l.GetP2().x(curpair.second);
		l.GetP2().z(newz);
	}
	return true;
}

bool VRefTriangleMesh::YTrimLineIf(VLine &l, const std::pair<vreal, vreal> &curpair)
{
	if (fabs(curpair.first - MAX_NUMBER_LIMIT) < EPSILON || fabs(curpair.second - MIN_NUMBER_LIMIT) < EPSILON)
		return false;
	if (l.GetP1().y() < curpair.first - EPSILON && l.GetP2().y() < curpair.first - EPSILON)
		return false;
	if (l.GetP1().y() > curpair.second + EPSILON && l.GetP2().y() > curpair.second + EPSILON)
		return false;
	if (l.GetP1().y() < curpair.first)
	{
		vreal deltay = l.GetP2().y() - l.GetP1().y();
		vreal dy = curpair.first - l.GetP1().y();
		vreal newz = l.GetP1().z() + (l.GetP2().z() - l.GetP1().z()) * dy / deltay;
		l.GetP1().y(curpair.first);
		l.GetP1().z(newz);
	}
	if (l.GetP1().y() > curpair.second)
	{
		vreal deltay = l.GetP2().y() - l.GetP1().y();
		vreal dy = curpair.second - l.GetP1().y();
		vreal newz = l.GetP1().z() + (l.GetP2().z() - l.GetP1().z()) * dy / deltay;
		l.GetP1().y(curpair.second);
		l.GetP1().z(newz);
	}
	if (l.GetP2().y() < curpair.first)
	{
		vreal deltay = l.GetP2().y() - l.GetP1().y();
		vreal dy = curpair.first - l.GetP1().y();
		vreal newz = l.GetP1().z() + (l.GetP2().z() - l.GetP1().z()) * dy / deltay;
		l.GetP2().y(curpair.first);
		l.GetP2().z(newz);
	}
	if (l.GetP2().y() > curpair.second)
	{
		vreal deltay = l.GetP2().y() - l.GetP1().y();
		vreal dy = curpair.second - l.GetP1().y();
		vreal newz = l.GetP1().z() + (l.GetP2().z() - l.GetP1().z()) * dy / deltay;
		l.GetP2().y(curpair.second);
		l.GetP2().z(newz);
	}
	return true;
}

int VTRI_MAX_SECTION_ITERATION = 1000000;

SequentialLineSet * VRefTriangleMesh::OneLoopX(std::multimap<vreal, VLine> &lMap, const std::multimap<vreal, vreal> &rMap,
	const std::map<vreal, std::pair<vreal, vreal>> &boundary, vreal tol)
{
	if (lMap.size() == 0)
		return 0;
	else
	{
		SequentialLineSet *sl = new SequentialLineSet(10);
		std::vector<int> toDelete;
		VLine l = lMap.begin()->second;
		sl->InsertPoint(l.GetP1());
		sl->InsertPoint(l.GetP2());
		//toDelete.push_back (si);
		lMap.erase(lMap.begin());
		VLine conLine;
		vreal searchGap = tol * 2.;
		std::multimap<vreal, VLine>::iterator fi = SearchConnected(lMap, l.GetP2().y(), l.GetP2(), searchGap, tol, conLine, rMap);
		std::multimap<vreal, VLine>::iterator fe = lMap.end();
		int counter = 0;
		while (fi != fe && counter < VTRI_MAX_SECTION_ITERATION)
		{
			VPoint keyPoint = conLine.GetP2();
			sl->InsertPoint(keyPoint);
			vreal newkey = keyPoint.y();
			//toDelete.push_back (fi->first);
			lMap.erase(fi);
			fi = SearchConnected(lMap, newkey, keyPoint, searchGap, tol, conLine, rMap);
			fe = lMap.end();
			++counter;
		}
		//DeleteItems(lMap,toDelete);
		return sl;
	}
}

SequentialLineSet * VRefTriangleMesh::OneLoopY(std::multimap<vreal, VLine> &lMap, const std::multimap<vreal, vreal> &rMap
	, const std::map<vreal, std::pair<vreal, vreal>> &boundary, vreal tol)
{
	if (lMap.size() == 0)
		return 0;
	else
	{
		SequentialLineSet *sl = new SequentialLineSet(10);
		std::vector<int> toDelete;
		VLine l = lMap.begin()->second;
		sl->InsertPoint(l.GetP1());
		sl->InsertPoint(l.GetP2());
		//toDelete.push_back (si);
		lMap.erase(lMap.begin());
		VLine conLine;
		vreal searchGap = tol * 2.;
		std::multimap<vreal, VLine>::iterator fi = SearchConnected(lMap, l.GetP2().x(), l.GetP2(), searchGap, tol, conLine, rMap);
		std::multimap<vreal, VLine>::iterator fe = lMap.end();
		int counter = 0;
		while (fi != fe && counter < VTRI_MAX_SECTION_ITERATION)
		{
			VPoint keyPoint = conLine.GetP2();
			sl->InsertPoint(keyPoint);
			vreal newkey = keyPoint.x();
			//toDelete.push_back (fi->first);
			lMap.erase(fi);
			fi = SearchConnected(lMap, newkey, keyPoint, searchGap, tol, conLine, rMap);
			fe = lMap.end();
			++counter;
		}
		//DeleteItems(lMap,toDelete);
		return sl;
	}
}

std::multimap<vreal, VLine>::iterator VRefTriangleMesh::SearchConnected(std::multimap<vreal, VLine> &lMap, vreal key, const VPoint &keyPoint
	, vreal searchGap, vreal tol, VLine &connectedL
	, const std::multimap<vreal, vreal> &rMap)
{
	std::multimap<vreal, VLine>::iterator li = lMap.lower_bound(key - searchGap);
	std::multimap<vreal, VLine>::iterator hi = lMap.lower_bound(key + searchGap);
	if (hi != lMap.end())
		++hi;
	for (li; li != hi; ++li)
	{
		if (keyPoint.Compare(li->second.GetP1(), tol))
		{
			connectedL = li->second;
			return li;
		}
	}
	// search reversed lines
	std::multimap<vreal, vreal>::const_iterator rli = rMap.lower_bound(key - searchGap);
	std::multimap<vreal, vreal>::const_iterator rhi = rMap.lower_bound(key + searchGap);
	if (rhi != rMap.end())
		rhi++;
	for (rli; rli != rhi; ++rli)
	{
		std::multimap<vreal, VLine>::iterator sli = lMap.lower_bound(rli->second - tol);
		std::multimap<vreal, VLine>::iterator shi = lMap.lower_bound(rli->second + tol);
		if (shi != lMap.end())
			++shi;
		for (sli; sli != shi; ++sli)
		{
			connectedL = sli->second;
			if (keyPoint.Compare(connectedL.GetP2(), tol))
			{
				connectedL.Reverse();
				return sli;
			}
		}
	}
	return lMap.end();
}

void VRefTriangleMesh::ReverseNormals()
{
	int mysize = surfpack.normals.Size();
	for (int i = 0; i<mysize; i++)
	{
		VPoint *n1 = surfpack.normals[i];
		(*n1) *= -1.;
	}
	//int mysize2 = Size();
	//for (int i = 0; i < mysize2; ++i)
	//{
	//	RefTriangle &rt = *(rdata.operator [] (i));
	//	RefTriangle temp(rt.ref2, rt.ref1, rt.ref0);
	//	rt = temp;
	//}
}