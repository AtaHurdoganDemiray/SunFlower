// ZMap.cpp

#include "stdafx.h"
#include "ZMap.h"
#include "Triangles.h"
#include "VTriangleE.h"
#include "PointSet.h"
#include "VPlane.h"
#include "LineIntersection.h"
#include "VSurface.h"
#include "VTriangles.h"
#include "dSurface.h"
#include "LongVec.h"
#include <map>

namespace collcheck
{
	int ZMap::m_memoryLimitInBytes = 2000000000; //  2000 e6  == 2GByte
	int ZMap::QueryMemory (const VPoint &mn , const VPoint &mx , double precision)
	{
		VPoint delta = mx - mn;
		int xcount = (int)(delta.x() / precision);
		int ycount = (int)(delta.y() / precision);
		int totalmemory = xcount * ycount * 4;
		return  totalmemory + 1;
	}
	double ZMap::GetPrecisionForMemory (const VPoint &mn , const VPoint &mx , int memoryLimitInBytes)
	{
		VPoint delta = mx - mn;
		// cellcount = invprecision * deltax * invprecision * deltay
		double cellcount = (memoryLimitInBytes / 4);
		double precision = sqrt ((delta.x() * delta.y()) / cellcount);
		return precision;
	}
	double ZMap::GetMaxDeltaForPrecision (double precision , int memoryLimitInBytes)
	{
		double cellcount = memoryLimitInBytes / 4;
		double maxdelta = sqrt (cellcount * (precision*precision));
		return maxdelta;
	}


	ZMap::ZMap (const VPoint &mn , const VPoint &mx , double precision)
	:m_min (mn) , m_max (mx) , m_precision (precision)
	{
		m_minx = m_min.x();
		m_miny = m_min.y();
		m_invPrecision = 1. / m_precision;
		m_delta = m_max - m_min;
		m_xsize = (int)(m_delta.x() * m_invPrecision) + 2;
		m_ysize = (int)(m_delta.y() * m_invPrecision) + 2;
		if (m_xsize * m_ysize * 4 > (int)m_memoryLimitInBytes)
			throw VException (_T( IDS_ZMaConExcMemLim),1);
		MakeZMap((float)m_min.z());
		m_sourceTesselationTol = -1.;
		m_sourceTriCount = 0;
		m_feedbackTotalTriangles = 0;
		m_feedbackCalculatedTriangles = 0;
	}
	ZMap::ZMap (const VPoint &mn , const VPoint &mx , vreal defZ , vreal precision)
	:m_min (mn) , m_max (mx) , m_precision (precision)
	{
		m_minx = m_min.x();
		m_miny = m_min.y();
		m_invPrecision = 1. / m_precision;
		m_delta = m_max - m_min;
		m_xsize = (int)(m_delta.x() * m_invPrecision) + 2;
		m_ysize = (int)(m_delta.y() * m_invPrecision) + 2;
		if (m_xsize * m_ysize * 4 > (int)m_memoryLimitInBytes)
			throw VException (_T( IDS_ZMaConExcMemLim),1);
		MakeZMap(defZ);
		m_sourceTesselationTol = -1.;
		m_sourceTriCount = 0;
		m_feedbackTotalTriangles = 0;
		m_feedbackCalculatedTriangles = 0;
	}

	ZMap::~ZMap ()
	{
		DestroyZMap();
	}

	const VPoint & ZMap::GetMin()const
	{
		return m_min;
	}

	const VPoint & ZMap::GetMax()const
	{
		return m_max;
	}

	double ZMap::GetPrecision() const
	{
		return m_precision;
	}

	void ZMap::MakeZMap(float defZ)
	{
		m_zmap = new float * [m_xsize];
		m_imap = new int *[m_xsize];
		for (int i=0; i<m_xsize; ++i)
		{
			m_zmap[i] = new float [m_ysize];
			m_imap[i] = new int [m_ysize];
			for (int j=0;j<m_ysize;++j)
			{
				m_zmap[i][j] = defZ;//(float)m_min.z();
				m_imap[i][j] = -1;
			}
		}
	}
	void ZMap::DestroyZMap ()
	{
		for (int i=0;i<m_xsize;++i)
		{
			delete [] m_zmap[i];
		}
		delete [] m_zmap;
		m_zmap = 0;
	}
	int ZMap::GetXi (double xc)const
	{
		//if (xc <= m_minx)
		//	return 0;
		int xi = (int) ((xc - m_minx) * m_invPrecision);
		//if (xi >= m_xsize)
		//	return (m_xsize - 1);
		return xi;
	}

	int ZMap::GetYi(double yc) const
	{
		//if (yc <= m_miny)
		//	return 0;
		int yi = (int)((yc - m_miny) * m_invPrecision);
		//if (yi >= m_ysize)
		//	return (m_ysize - 1);
		return yi;
	}

	void ZMap::UpdateZ (const VPoint &p,int trii)
	{
		float tocomp = (float)p.z();
		int xi = GetXi (p.x());
		int yi = GetYi (p.y());
		if (GetZ(xi,yi) < tocomp)
			SetZ (xi,yi,tocomp,trii);
	}

	void ZMap::FillZMap(const Triangles *trimesh  ,const VPoint &mn , const VPoint &mx)
	{
		int i=0;		
		try
		{
			//long mysize = relind.Size();
			//if (m_feedbackTotalTriangles != 0)
			//	(*m_feedbackTotalTriangles) += mysize; 
			int mysize = trimesh->Size();
			for (i=0;i<mysize;++i)
			{
				//long curind = relind.operator[](i);
				VTriangleE curtri = trimesh->operator [] (i);
				VPoint curmn (curtri.GetMin());
				VPoint curmx (curtri.GetMax());
				if (curmx.x() < mn.x() || curmx.y() < mn.y() || curmn.x() > mx.x() || curmn.y() > mx.y())
					continue;
				else
					UpdateWithSingleTri (curtri,i);
			}
		}
		catch (VException &e)
		{
			CString temp;
			CString temp2(e.GetMyMessage().c_str());
			temp.Format (_T( IDS_TrinoD),i); 
			throw VException ((LPCTSTR)(temp+temp2),1);
		}
		catch (std::exception &e)
		{
			CString temp;
			CString temp2(e.what());
			temp.Format (_T( IDS_TrinoD),i); 
			throw VException ((LPCTSTR)(temp+temp2),1);
		}
		catch (...)
		{
			CString temp;
			CString temp2(_T( IDS_ColZMaFilUnkExc));
			temp.Format (_T( IDS_TrinoD),i); 
			throw VException ((LPCTSTR)(temp+temp2),1);
		}
	}
	void ZMap::FillZMap(const Triangles *trimesh , double sourceSurfaceTesselationTol)
	{
		m_sourceTesselationTol = sourceSurfaceTesselationTol;
		m_triangles.push_back (trimesh);
		int i=-1;
		CString msg;
		try
		{
			int mysize = trimesh->Size();
			if (m_feedbackTotalTriangles != 0)
				(*m_feedbackTotalTriangles) += mysize; 
			for (i=0;i<mysize;++i)
			{
				VTriangleE curtri = trimesh->operator [] (i);
				UpdateWithSingleTri (curtri,i);
				//std::vector<VPoint> pvec;
				//curtri.Digitise (m_precision*0.5, pvec);
				//int mysize2 = pvec.size();
				//for (int j=0;j<mysize2;++j)
				//{
				//	const VPoint &curp = pvec[j];
				//	this->UpdateZ (curp);
				//}
			}
		}
		catch (VException &e)
		{
			CString temp;
			CString temp2(e.GetMyMessage().c_str());
			temp.Format (_T( IDS_TrinoD),i); 
			throw VException ((LPCTSTR)(temp+temp2),1);
		}
		catch (std::exception &e)
		{
			CString temp;
			CString temp2(e.what());
			temp.Format (_T( IDS_TrinoD),i); 
			throw VException ((LPCTSTR)(temp+temp2),1);
		}
		catch (...)
		{
			CString temp;
			CString temp2(_T( IDS_ColZMaFilUnkExc));
			temp.Format (_T( IDS_TrinoD),i); 
			throw VException ((LPCTSTR)(temp+temp2),1);
		}
	}
	PointSet * ZMap::MakePointSet ()const
	{
		int psize = m_xsize * m_ysize;
		PointSet *ps = new PointSet ((int)psize);
		for (int xi = 0; xi < m_xsize; ++xi)
		{
			for (int yi = 0;yi< m_ysize; ++yi)
			{
				double curx = m_minx + xi * m_precision;
				double cury = m_miny + yi * m_precision;
				double curz = m_zmap[xi][yi];
				ps->InsertPoint (VPoint (curx,cury,curz));
			}
		}
		return ps;
	}

	void ZMap::UpdateWithSingleTri (const VTriangleE &tri,int trii)
	{
		//VTriangle2dE tri2d (VPoint2d(tri.GetCorner1().x() , tri.GetCorner1().y())
		//	,VPoint2d (tri.GetCorner2().x() , tri.GetCorner2().y())
		//	,VPoint2d (tri.GetCorner3().x() , tri.GetCorner3().y()));
		++m_sourceTriCount;
		if (m_feedbackCalculatedTriangles != 0)
			(*m_feedbackCalculatedTriangles)++;
		VPoint mn(tri.GetMin());
		VPoint mx(tri.GetMax());
		if (mn.x() > m_max.x() || mn.y() > m_max.y() || mx.x() < m_min.x() || mx.y() < m_min.y())
			return;	// triangle is out of box !
		int xi = GetXi (mn.x());
		int xe = GetXi (mx.x());
		int yi = GetYi (mn.y());
		int ye = GetYi (mx.y());
		if (xi > 0)
			xi--;
		if (yi > 0)
			yi--;
		if (xe < m_xsize-1)
			xe++;
		if (ye < m_ysize-1)
			ye++;
		int oriyi = yi;
		for (xi;xi<=xe;++xi)
		{
			for (yi;yi<=ye;++yi)
			{
				double xc = m_min.x() + m_precision * xi;
				double yc = m_min.y() + m_precision * yi;
				//bool ckisintri2d = tri2d.IsInTriangle (VPoint2d(xc,yc));
				//if (ckisintri2d)
				{
					VLine l (VPoint(xc,yc,m_min.z()-1.),VPoint(xc,yc,m_max.z()+1.));
					VPlane pln = tri.GetPlane();
					double t;
					VPlane::IntType ck = pln.Int (l,t);
					if (ck == VPlane::REGULAR)
					{
						VPoint ip = l.GetP (t);
						if (tri.IsInTriangle (ip))
						{
							if (GetZ(xi,yi) < (float)ip.z())
								SetZ (xi,yi,(float)ip.z(),trii);
						}
					}
					else if (ck == VPlane::COINCIDENT)
					{
						LineIntersection calc;
						VPoint intp , parlint;
						double coinTol (m_precision*0.1);
						LineIntersection::intersectiontype ck1 = 
						calc.LinesIntersect (l.GetP1() , l.GetV() , tri.GetCorner1() , tri.GetEdge1() , intp , parlint , coinTol);
						if (ck1 == LineIntersection::LINESINTERSECT)
						{
							if (GetZ(xi,yi) < (float)intp.z())
								SetZ(xi,yi,(float)intp.z(),trii);
						}
						LineIntersection::intersectiontype ck2 = 
							calc.LinesIntersect (l.GetP1() , l.GetV() , tri.GetCorner2() , tri.GetEdge2() , intp , parlint , coinTol);
						if (ck2 == LineIntersection::LINESINTERSECT)
						{
							if (GetZ(xi,yi) < (float)intp.z())
								SetZ(xi,yi,(float)intp.z(),trii);
						}
						LineIntersection::intersectiontype ck3 = 
							calc.LinesIntersect (l.GetP1() , l.GetV() , tri.GetCorner3() , tri.GetEdge3() , intp , parlint , coinTol);
						if (ck3 == LineIntersection::LINESINTERSECT)
						{
							if (GetZ(xi,yi) < (float)intp.z())
								SetZ(xi,yi,(float)intp.z(),trii);
						}
					}
				}
			}
			yi = oriyi;
		}
	}

	collcheck::ZMap * ZMap::MakeZMapAndFill (double tolerance , const std::vector<VSurface *> &surfVec,const std::vector<VTriangles *> &triVec,int *feedbackTotalTriangles,int *feedbackCalculatedTriangles)
	{
		VPoint mn (MAX_NUMBER_LIMIT);
		VPoint mx (MIN_NUMBER_LIMIT);
		std::vector<VSurface *>::const_iterator i = surfVec.begin();
		std::vector<VSurface *>::const_iterator e = surfVec.end();
		for (i;i!=e;++i)
		{
			mn.ReplaceMin ( (*i)->GetMinPoint());
			mx.ReplaceMax ( (*i)->GetMaxPoint());
		}
		std::vector<VTriangles *>::const_iterator ii = triVec.begin();
		std::vector<VTriangles *>::const_iterator ee = triVec.end();
		for (ii;ii!=ee;++ii)
		{
			mn.ReplaceMin ( (*ii)->GetMinPoint());
			mx.ReplaceMax ( (*ii)->GetMaxPoint());
		}	
		collcheck::ZMap *zmap = new collcheck::ZMap (mn , mx , tolerance);
		zmap->SetZMapFeedbackParameters (feedbackTotalTriangles , feedbackCalculatedTriangles);
		i = surfVec.begin();
		ii = triVec.begin();
		for (i;i!=e;++i)
		{
			zmap->FillZMap ( &(*i)->GetTriangles(),(*i)->GetDSurf()->GetTolerance());
		}
		double tesstol = zmap->GetSourceTesselationTol();
		for (ii;ii!=ee;++ii)
		{
			zmap->FillZMap ( (*ii) , tesstol);
		}
		return zmap;
	}

	float ZMap::GetLiftFor (const std::vector<int> &toolxi 
							, const std::vector<int> &toolyi 
							, const std::vector<float> &toolz 
							, int &maxi 
							, float &maxToolZ 
							, float &maxModelZ 
							, bool &isNearToZMapVerticalEdge)
	{
		float maxlift = 0.0f;
		maxi = -1;
		int mysize = (int)toolxi.size();
		int xi,yi;
		float zc,modelzc;
		for (int i=0;i<mysize;++i)
		{
			xi = toolxi[i];
			yi = toolyi[i];
			//if (xi<0 || yi<0 || xi >= m_xsize || yi >= m_ysize)
			//	continue;
			zc = toolz[i];
			modelzc = GetZ(xi,yi);//m_zmap[xi][yi];
			if (zc < modelzc)
			{
				float curdelta = modelzc - zc;
				if (curdelta > maxlift)
				{
					maxlift = curdelta;
					maxi = i;
					maxToolZ = zc;
					maxModelZ = modelzc;
				}
			}
		}
		isNearToZMapVerticalEdge = false;
		if (maxlift > 0.0f && maxi!=-1)
		{
			// check if zmap vertical edge
			xi = toolxi[maxi];
			yi = toolyi[maxi];
			int xinext = xi+1;
			int xiprev = xi-1;
			int yinext = yi+1;
			int yiprev = yi-1;
			double zval = GetZ (xi,yi);
			double trshold = m_precision * 5;
			if (xinext!=m_xsize)
				if (fabs (GetZ(xinext,yi)-zval) > trshold)
					isNearToZMapVerticalEdge = true;
			if (xiprev >= 0)
				if (fabs (GetZ(xiprev,yi)-zval) > trshold)
					isNearToZMapVerticalEdge = true;
			if (yinext!=m_ysize)
				if (fabs (GetZ(xi,yinext)-zval) > trshold)
					isNearToZMapVerticalEdge = true;
			if (yiprev >=0)
				if (fabs (GetZ(xi,yiprev)-zval) > trshold)
					isNearToZMapVerticalEdge = true;
			if (xinext!=m_xsize && yinext!=m_ysize)
				if (fabs(GetZ(xinext,yinext)-zval) > trshold)
					isNearToZMapVerticalEdge = true;
			if (xinext!=m_xsize && yiprev >= 0)
				if (fabs(GetZ(xinext,yiprev)-zval) > trshold)
					isNearToZMapVerticalEdge = true;
			if (xiprev>=0 && yiprev >= 0)
				if (fabs (GetZ(xiprev,yiprev)-zval) > trshold)
					isNearToZMapVerticalEdge = true;
			if (xiprev>=0 && yinext != m_ysize)
				if (fabs (GetZ(xiprev,yinext)-zval) > trshold)
					isNearToZMapVerticalEdge = true;
		}
		return maxlift;
	}
		
	float ZMap::GetZ(int xi,int yi)const
	{
		if (xi >= 0 && yi >= 0 && xi < m_xsize && yi < m_ysize)
			return m_zmap[xi][yi];
		else
			return ((float)m_min.z());
	}
	bool ZMap::SetZ (int xi,int yi,float zc,int trii)
	{
		if (xi >= 0 && yi >= 0 && xi < m_xsize && yi < m_ysize)
		{
			m_zmap[xi][yi] = zc;
			m_imap[xi][yi] = trii;
			return true;
		}
		else
		{
			return false;
		}
	}
	VStr ZMap::GetInfo ()const
	{
		VOStrStream mystr;
		mystr << _T( IDS_Cozunurluk)<<m_precision<<_T( IDS_N);
		mystr << _T( IDS_XNoktaSayisi)<<m_xsize<<_T( IDS_YNoktaSayisi)<<m_ysize<<_T( IDS_N);
		mystr << _T( IDS_TopNokSay)<<m_xsize*m_ysize<<_T( IDS_N);
		mystr << _T( IDS_MinXYZ)<<m_min.x()<<_T("/")<<m_min.y()<<_T("/")<<m_min.z()<<_T( IDS_MaxXYZ)<<m_max.x()<<_T("/")<<m_max.y()<<_T("/")<<m_max.z()<<_T( IDS_N);
		mystr << _T( IDS_KayYuzÜcgTol)<<m_sourceTesselationTol<<_T( IDS_N);
		mystr << _T( IDS_KayÜcgSay) << m_sourceTriCount << _T( IDS_N);
		return mystr.str();
	}
	double ZMap::GetSourceTesselationTol()const
	{
		return m_sourceTesselationTol;
	}
	void ZMap::SetSourceTesselationTol (double tol)
	{
		m_sourceTesselationTol = tol;
	}
	void ZMap::IncrementSourceTriCount (int toAdd)
	{
		m_sourceTriCount += toAdd;
	}
		
	int ZMap::GetSourceTriCount()
	{
		return m_sourceTriCount;
	}
	const std::vector<const Triangles *> & ZMap::GetTriangles()const
	{
		return m_triangles;
	}
	
	void ZMap::SetZMapFeedbackParameters (int *totalTriangles , int *calculatedTriangles)
	{
		m_feedbackTotalTriangles = totalTriangles;
		m_feedbackCalculatedTriangles = calculatedTriangles;
	}
	ZMap * ZMap::GetZMapInside(const VPoint &mn, const VPoint &mx, double newPrec) const
	{
		if (m_triangles.size() != 1)
			throw VException (_T( IDS_ZMaGetExcMorTha),1);
		const Triangles *tri = m_triangles.front ();

		collcheck::ZMap *finezmap = new collcheck::ZMap (mn,mx,newPrec);
		finezmap->m_triangles.push_back (tri);
		std::map<int,int> usedMap;
		for (double x=mn.x();x<=mx.x();x+=newPrec)
		{
			for (double y=mn.y();y<=mx.y();y+=newPrec)
			{
				int xi = GetXi(x);
				int yi = GetYi(y);
				if (xi>=0 && xi < m_xsize && yi>=0 && yi < m_ysize)
				{
					int trii = m_imap[xi][yi];
					if (trii != -1)
					{
						// check if already used
						std::map<int,int>::iterator fi = usedMap.find (trii);
						if (fi == usedMap.end())
						{
							usedMap.insert (std::pair<int,int>(trii,trii));
							VTriangleE curtri = tri->operator [] (trii);
							finezmap->UpdateWithSingleTri (curtri,trii);
						}
					}
				}
			}
		}
		return finezmap;
	}
	VPoint ZMap::GetPoint (int xi , int yi)const
	{
		double xc = m_min.x() + m_precision * xi;
		double yc = m_min.y() + m_precision * yi;
		double zc = (double)GetZ (xi,yi);
		return VPoint (xc,yc,zc);
	}
	int ZMap::GetXSize ()const
	{
		return m_xsize;
	}
	int ZMap::GetYSize ()const
	{
		return m_ysize;
	}
	void ZMap::GetAllPointsForXi (int xi,std::vector<VPoint> &ypVec)const
	{
		int ySize = GetYSize();
		for (int y=0;y<ySize;++y)
		{
			VPoint curp = GetPoint (xi,y);
			ypVec.push_back (curp);
		}
	}
	void ZMap::GetAllPointsForYi (int yi,std::vector<VPoint> &xpVec)const
	{
		int xSize = GetXSize();
		for (int x=0;x<xSize;++x)
		{
			VPoint curp = GetPoint (x,yi);
			xpVec.push_back (curp);
		}
	}
}


