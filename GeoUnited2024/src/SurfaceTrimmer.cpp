// SurfaceTrimmer.cpp

#include "stdafx.h"
#include "SurfaceTrimmer.h"
#include "UntrimmedSurface.h"
#include "SequentialLineSet2d.h"
#include "VRefTriangleMesh.h"
#include "UntypedException.h"

SurfaceTrimmer::SurfaceTrimmer (const UntrimmedSurface &surf
								, const SequentialLineSet2d *outerLoop
								, const std::vector<SequentialLineSet2d *> &innerLoopVec)
								:m_surf (surf)
								,p_outerLoop (outerLoop)
								,p_innerLoopVec (innerLoopVec)
{

}

// NEW TRIMMING ALGORITHM

void SurfaceTrimmer::DoTrimInParSpace (int uStepNo
									   , int vStepNo
									   , int bigvStepNo
									   , const double &ubegin
									   , const double &uend
									   , const double &vbegin
									   , const double &vend
									   , VRefTriangleMesh *trimmedTri)
{
	try
	{
	m_trimmedTri = trimmedTri;
	m_uStepNo = uStepNo;
	m_vStepNo = vStepNo;
	m_bigvStepNo = bigvStepNo;
	m_ubegin = ubegin;
	m_uend = uend;
	m_vbegin = vbegin;
	m_vend = vend;
	double deltav = m_vend - m_vbegin;
	double deltau = m_uend - m_ubegin;
	m_smallvstep = deltav / m_bigvStepNo;
	m_vstep = deltav / m_vStepNo;
	m_ustep = deltau / m_uStepNo;

	std::vector<std::vector<VLine2d>> leftPoints;
	TrimInParamSpace (leftPoints);
	
	// Adding end triangles for trim loop neighborhood of lines
	std::vector<VTriangle2d> triVec;
	AddEndTriangles (leftPoints,triVec);// leftPoints are modified

	// Inserting End Triangles To Surfpack
	int mysize2 = (int)(triVec.size());
	for (int w=0;w<mysize2;w++)
	{
		VTriangle2d &curt = triVec[w];
		AddT (curt);
	}

	int qno = m_uStepNo * m_bigvStepNo;
	std::vector<bool>digitisedQuad(qno,false);
	std::vector<VTriangle2d> triVec2;
	DigitiseLineVec (leftPoints,digitisedQuad,triVec2);
	
	// Inserting End Triangles2 To Surfpack
	int mysize3 = (int)(triVec2.size());
	for (int w=0;w<mysize3;w++)
	{
		VTriangle2d &curt = triVec2[w];
		AddT (curt);
	}

	MakeTrianglesFromQuads (digitisedQuad);
	}
	catch (std::exception &exc)
	{
		throw exc;
	}
	catch (...)
	{
		throw UntypedException(( IDS_UnkExcInSurDoT),999);
	}
}
//void SurfaceTrimmer::AddT (const VTriangle2d &t)
//{
//	m_trimmedTri->surfpack.parampoints.InsertPoint(t.GetCorner1());
//	double u = t.GetCorner1().x();
//	double v = t.GetCorner1().y();
//	VPoint m1 = m_surf.GetPoint (u,v);
//	VPoint n1 = m_surf.GetNormal (u,v,m1);
//	m_trimmedTri->surfpack.modelpoints.InsertPoint (m1);
//	m_trimmedTri->surfpack.normals.InsertPoint (n1);
//	int i1 = m_trimmedTri->surfpack.modelpoints.Size();
//	
//	m_trimmedTri->surfpack.parampoints.InsertPoint (t.GetCorner2());	
//	u = t.GetCorner2().x();
//	v = t.GetCorner2().y();
//	VPoint m2 = m_surf.GetPoint (u,v);
//	VPoint n2 = m_surf.GetNormal (u,v,m2);
//	m_trimmedTri->surfpack.modelpoints.InsertPoint (m2);
//	m_trimmedTri->surfpack.normals.InsertPoint (n2);
//	int i2 = i1 +1;
//
//	m_trimmedTri->surfpack.parampoints.InsertPoint (t.GetCorner3());
//	u = t.GetCorner3().x();
//	v = t.GetCorner3().y();
//	VPoint m3 = m_surf.GetPoint(u,v);
//	VPoint n3 = m_surf.GetNormal (u,v,m3);
//	m_trimmedTri->surfpack.modelpoints.InsertPoint(m3);
//	m_trimmedTri->surfpack.normals.InsertPoint (n3);
//	int i3 = i2 +1;
//
//	RefTriangle rt (i1,i2,i3);
//	m_trimmedTri->AddTriangle (rt);
//}
void SurfaceTrimmer::AddT (const VTriangle2d &t)
{
	bool isexist1,isexist2,isexist3;
	int i1 = m_trimmedTri->surfpack.parampoints.InsertPointIf (t.GetCorner1(),isexist1);
	int i2 = m_trimmedTri->surfpack.parampoints.InsertPointIf (t.GetCorner2(),isexist2);
	int i3 = m_trimmedTri->surfpack.parampoints.InsertPointIf (t.GetCorner3(),isexist3);
	if (!isexist1)
	{
		double u = t.GetCorner1().x();
		double v = t.GetCorner1().y();
		VPoint m1 = m_surf.GetPoint (u,v);
		VPoint n1 = m_surf.GetNormal (u,v,m1);
		m_trimmedTri->surfpack.modelpoints.InsertPoint (m1);
		m_trimmedTri->surfpack.normals.InsertPoint (n1);
	}
	if (!isexist2)
	{
		double u = t.GetCorner2().x();
		double v = t.GetCorner2().y();
		VPoint m2 = m_surf.GetPoint (u,v);
		VPoint n2 = m_surf.GetNormal (u,v,m2);
		m_trimmedTri->surfpack.modelpoints.InsertPoint (m2);
		m_trimmedTri->surfpack.normals.InsertPoint (n2);
	}
	if (!isexist3)
	{
		double u = t.GetCorner3().x();
		double v = t.GetCorner3().y();
		VPoint m3 = m_surf.GetPoint(u,v);
		VPoint n3 = m_surf.GetNormal (u,v,m3);
		m_trimmedTri->surfpack.modelpoints.InsertPoint(m3);
		m_trimmedTri->surfpack.normals.InsertPoint (n3);
	}
	RefTriangle rt (i1,i2,i3);
	m_trimmedTri->AddTriangle (rt);
}

void SurfaceTrimmer::TrimInParamSpace (std::vector <std::vector<VLine2d>> &leftPoints)
{
	leftPoints.resize(m_bigvStepNo+1);
	std::vector<std::vector<double>> uparsVecVec (m_bigvStepNo+1);
	MarkTrimLoop (p_outerLoop
				, uparsVecVec );
	int mysize=(int)p_innerLoopVec.size();
	for (int i=0;i<mysize;i++)
	{
		SequentialLineSet2d *currLoop = p_innerLoopVec[i];
		MarkTrimLoop (currLoop,uparsVecVec);
	}
	
	// Line making out of u intervals which are not trimmed
	for (int i=0;i<m_bigvStepNo+1;i++)
	{
		std::vector<double> &cl = uparsVecVec[i];
		std::vector<double>::iterator rb = cl.begin();
		std::vector<double>::iterator re = cl.end();
		std::sort (rb,re);
		
		int mysize2 = (int)cl.size();
		double currv = m_vbegin + i * m_smallvstep;
		for (int w=0; w<mysize2-1;w+=2)
		{
			//if (w % 2 == 1)// trimming if...
			//	continue;
			
			double u0 = cl[w];
			double u1 = cl[w+1];
			VPoint2d p1 (u0,currv);
			VPoint2d p2 (u1,currv);
			VLine2d myl (p1,p2);
			leftPoints[i].push_back (myl);
		}
	}
}

void SurfaceTrimmer::MarkTrimLoop (const SequentialLineSet2d *currLoop
									, std::vector <std::vector<double>> &uparsListVec)
{
	int loopsize1 = currLoop->Size();
	for (int i=0;i<loopsize1;i++)
	{
		VLine2d currl = currLoop->operator [](i);
		int starti = (int)((currl.GetP1().y() - (m_vbegin+EPSILON)) / m_smallvstep);
		int endi = (int)((EPSILON + currl.GetP2().y() - m_vbegin) / m_smallvstep);
		if (starti > endi)
		{
			int tmp = starti;
			starti = endi;
			endi = tmp;
		}
		//if (endi != m_bigvStepNo)
		endi += 1;
		
		double u1 = currl.GetP1().x();
		double u2 = currl.GetP2().x();
		double v1 = currl.GetP1().y();
		double v2 = currl.GetP2().y();
		double bigv = currl.GetP2().y();
		if (v1 > v2)
		{
			double tmp2 = v2;
			v2 = v1;
			v1 = tmp2;
			tmp2 = u2;
			u2 = u1;
			u1 = tmp2;
		}
		double slope = (u2-u1) / (v2-v1);
		if (fabs(v2-v1) > 1.e-8 )
		{
			for (int j=starti;j<=endi;j++)
			{
				double currv = j * m_smallvstep + m_vbegin;
				if (currv+1.e-8 >= v1 && currv <= v2+1.e-8)
				{
					//Adding intersection
					double curru = u1 + (currv - v1) * slope;
					//clipping to [0,1]
					if (curru>1.0)
						curru = 1.;
					if (curru < 0.)
						curru = 0.;

					uparsListVec[j].push_back (curru);
				}
				if (fabs(currv-m_vbegin) < EPSILON)
				{
					//Adding intersection
					double curru = u1 + (currv - v1) * slope;
					//clipping to [0,1]
					if (curru >1.0)
						curru = 1.0;
					if (curru < 0.0)
						curru = 0.0;
					
					uparsListVec[j].push_back (curru);
				}
				if (fabs(currv-m_vend) < EPSILON)
				{
					//Adding intersection
					double curru = u1 + (currv - v1) * slope;
					//clipping to [0,1]
					if (curru >1.0)
						curru = 1.0;
					if (curru < 0.0)
						curru = 0.0;
					uparsListVec[j].push_back (curru);
				}
			}
		}
		else
		{
			//uparsListVec[starti].push_back (u1);
			//uparsListVec[starti].push_back (u2);
		}
	}	
	// for fixing start and end problem
	//uparsListVec[0] = uparsListVec[1];
	//uparsListVec[m_bigvStepNo] = uparsListVec[m_bigvStepNo-1];
}

void SurfaceTrimmer::AddTriOfUnitLine (const VLine2d &ul)
{
	double x = ul.GetP1().x();
	double xe = ul.GetP2().x();
	double y = ul.GetP1().y();
	PointSet2d tmp(250);
	VPoint2d pp(x,y);
	VPoint2d ppa (x,y+m_vstep);
	int ip,ipa;
	for (x;x<=xe;x+=m_ustep)
	{
		VPoint2d p(x,y);
		VPoint2d pa(x,y+m_vstep);
		VPoint mp = m_surf.GetPoint (p.x(),p.y());
		VPoint np = m_surf.GetNormal (p.x(),p.y(),mp);
		VPoint mpa = m_surf.GetPoint (pa.x(),pa.y());
		VPoint npa = m_surf.GetNormal (pa.x(),pa.y(),mpa);
		int i = m_trimmedTri->surfpack.InsertPoint (p,mp,np);
		int ia = m_trimmedTri->surfpack.InsertPoint (pa,mpa,npa);
		
		if (p.x() != pp.x())
		{
			RefTriangle r1(ia,i,ip);
			RefTriangle r2(ip,ipa,ia);
			m_trimmedTri->AddTriangle (r1);
			m_trimmedTri->AddTriangle (r2);
		}
		pp=p;
		ppa=pa;
		ip = i;
		ipa = ia;
	}
}

void SurfaceTrimmer::AddEndTriangle1 (std::vector<VLine2d>::iterator &a 
								, std::vector<VLine2d>::const_iterator &b
								, std::vector<VTriangle2d> &triVec)
{
	if (a->GetP1().x() < b->GetP1().x())
	{
		VPoint2d c1 (a->GetP1());
		VPoint2d c2 (b->GetP1().x(),a->GetP1().y());
		VPoint2d c3 (b->GetP1());
		//new line
		a->SetP1 (c2);
		//new tri
		VTriangle2d t (c1,c2,c3);
		triVec.push_back (t);
	}
	else if (a->GetP1().x() > b->GetP1().x())
	{
		VPoint2d c1 (a->GetP1());
		VPoint2d c2 (a->GetP1().x(),b->GetP1().y());
		VPoint2d c3 (b->GetP1());
		// new line
		//nl2.SetP1 (c2);
		// new tri
		VTriangle2d t(c1,c2,c3);
		triVec.push_back (t);
	}
}

void SurfaceTrimmer::AddEndTriangle2 (std::vector<VLine2d>::iterator &a 
								, std::vector<VLine2d>::const_iterator &b
								, std::vector<VTriangle2d> &triVec)
{
	if (a->GetP2().x() < b->GetP2().x())
	{
		VPoint2d c1 (a->GetP2());
		VPoint2d c2 (b->GetP2());
		VPoint2d c3 (a->GetP2().x(),b->GetP2().y());
		//new line
		//nv.push_back (nl1);
		//nl2.SetP2(c3);
		//nv.push_back (nl2);
		//new tri
		VTriangle2d t (c1,c2,c3);
		triVec.push_back (t);
	}
	else if (a->GetP2().x() > b->GetP2().x())
	{
		VPoint2d c1 (a->GetP2());
		VPoint2d c2 (b->GetP2());
		VPoint2d c3 (b->GetP2().x(),a->GetP2().y());
		//new line
		VPoint2d newp (c2.x(),a->GetP2().y());
		a->SetP2 (newp);	
		//nv.push_back (nl1);
		//nv.push_back (nl2);
		//new tri
		VTriangle2d t(c1,c2,c3);
		triVec.push_back (t);
	}
	else
	{
		//no triangle
		// no line modif
	}
}

void SurfaceTrimmer::AddEndTriangles (std::vector<std::vector<VLine2d>> &leftPoints 
								, std::vector<VTriangle2d> &triVec)
{
	int mysize = (int)(leftPoints.size());
	for (int j=0;j<mysize-1;j++)
	{
		std::vector<VLine2d> &v1 = leftPoints[j];
		std::vector<VLine2d> &v2 = leftPoints[j+1];
		
		if (v1.size() != v2.size())
		{
			ExceptionalEndTriangles(v1,triVec);
			continue;
		}

		std::vector<VLine2d>::iterator a = v1.begin();
		std::vector<VLine2d>::iterator e = v1.end();
		std::vector<VLine2d>::const_iterator b = v2.begin();
		for (a;a!=e;a++)
		{	
			AddEndTriangle1 (a,b,triVec);
			AddEndTriangle2 (a,b,triVec);
			b++;
		}
	}
}

void SurfaceTrimmer::DigitiseLineVec (std::vector<std::vector<VLine2d>> &leftPoints
									, std::vector<bool> &digitisedQuad
									, std::vector<VTriangle2d> &triVec)
{
	int mysize = leftPoints.size();

	for (int i=0;i<mysize;i++)
	{
		std::vector<VLine2d> &currv = leftPoints[i];
		std::vector<VLine2d>::iterator a = currv.begin();
		std::vector<VLine2d>::iterator ae = currv.end();
		for(a;a!=ae;a++)
		{
			DigitiseLine (a,digitisedQuad,triVec);
		}
	}
}

// cell indexing
//cellno -> cell_u_int , cell_v_int    	-------->>> cell_v_int = (int) cellno / m_uStepNo
//					cell_u_int = cellno - cell_v_int x m_uStepNo

//cell_u_int , cell_v_int -> cellno
//------- >>> cellno = cell_u_int + cell_v_int x m_uStepNo

// casting line coordinate to cellno

//cell_u_int = (lcx - m_ubegin) / m_ustep;
//cell_v_int = (lcy - m_vbegin) / m_smallvstep;
//cellno = ...

// cell start coord
//cell_start_u = cell_u_int x m_ustep + m_ubegin
//cell_start_v = cell_v_int x m_smallvstep + m_vbegin

// casting line to cells (cells are always subset of line (smaller but not bigger))
// line_begin_cellno = (cell_u_int_start + 1) + cell_v_int x m_uStepNo
//			OR IF (cell_start_u==lineP1x) cell_u_int_start + cell_v_int x m_uStepNo
// line_end_cellno = (cell_u_int_end - 1)  + cell_v_int x m_uStepNo
//			OR IF (cell_u_int_end) + cell_v_int x m_uStepNo
// 

// Explanation in    geometrycore\doc\SurfaceTrimmer_DigitiseLine.bmp
void SurfaceTrimmer::DigitiseLine (std::vector<VLine2d>::iterator &a
								, std::vector<bool> &digitisedQuad
								, std::vector<VTriangle2d> &triVec)
{
	// Finding start and end indexes of line in digitised matrice	
	int cell_u_int_start = (int)((a->GetP1().x() - m_ubegin) / m_ustep);
	int cell_v_int = (int)((EPSILON + a->GetP1().y() - m_vbegin) / m_smallvstep);	
	int cell_u_int_end = (int)((a->GetP2().x() - m_ubegin) / m_ustep);
	// rounding quads so that all quads are subset of line
	if (cell_u_int_start != m_uStepNo-1) 
		++cell_u_int_start;
	if ( cell_u_int_end == m_uStepNo) 
		--cell_u_int_end;
	if (cell_v_int == m_bigvStepNo)
		--cell_v_int;

	// cellno = cell_u_int + cell_v_int x m_uStepNo
	int cellnostart = cell_u_int_start + cell_v_int * m_uStepNo;
	int cellnoend = cell_u_int_end + cell_v_int * m_uStepNo;
	
	// Setting cells that are not trimmed
	for (int i=cellnostart;i<cellnoend;i++)
		digitisedQuad[i] = true;

	// AddEndQuads
	// Calculating coordinates of clipped line begin and end
	// Calculating the matrice quad's start and end coordinates
	double cell_start_v = cell_v_int * m_smallvstep + m_vbegin;
	double cell_start_u,cell_end_u;	
	if (cell_u_int_start < cell_u_int_end)
	{
		cell_start_u = cell_u_int_start * m_ustep + m_ubegin;
		cell_end_u = cell_u_int_end * m_ustep + m_ubegin;	
	}
	else
	{
		cell_start_u = a->GetMidPoint().x();
		cell_end_u = a->GetMidPoint().x();
	}

	VPoint2d c1 (cell_start_u,cell_start_v);
	VPoint2d yoffset(0.0,m_smallvstep);
	VPoint2d c2 = c1 + yoffset;
	VPoint2d c3 = a->GetP1() + yoffset;
	VTriangle2d t1 (c1,c2,c3);
	VTriangle2d t2 (c3,a->GetP1(),c1);
	triVec.push_back (t1);
	triVec.push_back (t2);
	VPoint2d ce1 (cell_end_u , cell_start_v);
	//VPoint2d ce2 = a->GetP2();
	VPoint2d ce3 = a->GetP2() + yoffset;
	VPoint2d ce4 = ce1 + yoffset;
	VTriangle2d te1 (ce1,a->GetP2(),ce3);
	VTriangle2d te2 (ce3 , ce4 , ce1);
	triVec.push_back (te1);
	triVec.push_back (te2);
}	

void SurfaceTrimmer::AddQuad (const double &u1,const double &v1
							  ,const double &u2,const double &v2
							  ,std::vector<int> &indiceVec)
{
	//cell_u_int = (lcx - m_ubegin) / m_ustep;
	//cell_v_int = (lcy - m_vbegin) / m_smallvstep;
	int cell_u_int_begin = (int)((EPSILON + u1 - m_ubegin) / m_ustep);
	int cell_v_int_begin = (int)((EPSILON + v1 - m_vbegin) / m_smallvstep);
	int cell_u_int_end = (int)((EPSILON + u2 - m_ubegin) / m_ustep);
	int cell_v_int_end = (int)((EPSILON + v2 - m_vbegin) / m_smallvstep);
	//cellno = cell_u_int + cell_v_int x m_uStepNo
	int cellno1 = cell_u_int_begin + cell_v_int_begin * m_uStepNo;
	int cellno2 = cell_u_int_end + cell_v_int_end * m_uStepNo;

	int i1 = indiceVec[cellno1];
	int i2 = indiceVec[cellno1+1];
	int i3 = indiceVec[cellno2];
	int i4 = indiceVec[cellno2-1];
	if (i1==-1)
	{
		VPoint2d p1 (u1,v1);
		VPoint m1 = m_surf.GetPoint(u1,v1);
		VPoint n1 = m_surf.GetNormal (u1,v1,m1);
		i1 = m_trimmedTri->surfpack.InsertPoint(p1,m1,n1);
		indiceVec[cellno1]=i1;
	}
	if (i2==-1)
	{
		VPoint2d p2 (u2,v1);
		VPoint m2 = m_surf.GetPoint (u2,v1);
		VPoint n2 = m_surf.GetNormal (u2,v1,m2);
		i2 = m_trimmedTri->surfpack.InsertPoint(p2,m2,n2);
		indiceVec[cellno1+1]=i2;
	}
	if (i3==-1)
	{
		VPoint2d p3 (u2,v2);
		VPoint m3 = m_surf.GetPoint (u2,v2);
		VPoint n3 = m_surf.GetNormal (u2,v2,m3);
		i3 = m_trimmedTri->surfpack.InsertPoint (p3,m3,n3);
		indiceVec[cellno2]=i3;
	}
	if (i4 == -1)
	{
		VPoint2d p4 (u1,v2);
		VPoint m4 = m_surf.GetPoint (u1,v2);
		VPoint n4 = m_surf.GetNormal (u1,v2,m4);
		i4 = m_trimmedTri->surfpack.InsertPoint (p4,m4,n4);
		indiceVec[cellno2-1]=i4;
	}

	RefTriangle r1 (i1,i2,i3);
	RefTriangle r2 (i3,i4,i1);
	m_trimmedTri->AddTriangle(r1);
	m_trimmedTri->AddTriangle(r2);
}

void SurfaceTrimmer::ExceptionalEndTriangles (std::vector<VLine2d> &v1,std::vector<VTriangle2d> &triVec)
{

}

void SurfaceTrimmer::MakeTrianglesFromQuads (std::vector<bool> &digitisedQuads)
{
	int myratio = (m_bigvStepNo / m_vStepNo);
	int mysize = static_cast<int>(digitisedQuads.size());
	std::vector<int> myindiceVec ((m_uStepNo+1)*(m_bigvStepNo+1),-1);
	int ilimit = mysize - myratio * m_uStepNo;
	for (int i=0;i<mysize;i++)
	{
		if (digitisedQuads[i])
		{
			//cell_v_int = (int) cellno / m_uStepNo
			//cell_u_int = cellno - cell_v_int x m_uStepNo
			int cell_v_int = (int) (i / m_uStepNo);
			int cell_u_int = i - cell_v_int * m_uStepNo;
			//cell_start_u = cell_u_int x m_ustep + m_ubegin
			//cell_start_v = cell_v_int x m_smallvstep + m_vbegin
			double cell_start_u = cell_u_int * m_ustep + m_ubegin;
			double cell_start_v = cell_v_int * m_smallvstep + m_vbegin;
			//Quad is not trimmed
			// check if can be merged
			bool merge = false;
			if (i<ilimit)
			{
				merge = true;
				for (int j=1;j<myratio;j++)
				{
					int curri = i + m_uStepNo*j;
					if (! digitisedQuads[curri])
						merge = false;
				}
			}
			if (merge)
			{
				for (int j=1;j<myratio;j++)
				{
					int curri = i + m_uStepNo * j;
					digitisedQuads[curri] = false;
				}
				int cell_end_v_int = (int)((i+m_uStepNo*myratio)/m_uStepNo);
				double cell_end_v = cell_end_v_int * m_smallvstep + m_vbegin;
				AddQuad (cell_start_u
						,cell_start_v
						,cell_start_u+m_ustep
						,cell_end_v
						,myindiceVec);
			}
			else
			{
				AddQuad (cell_start_u
						,cell_start_v
						,cell_start_u+m_ustep
						,cell_start_v+m_smallvstep
						,myindiceVec);
			}
		}
	}
}
