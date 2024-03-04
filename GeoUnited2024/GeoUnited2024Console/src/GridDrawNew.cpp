// GridDrawNew.cpp

#include "stdafx.h"
#include "GridDrawNew.h"
#include "VisualAttLoader.h"
#include "DoubleVec.h"
#include "PointSet.h"
#include "LinePointMinDist.h"
#include "VTriangleE.h"
#include "MoveMatrix.h"
#include "RotationMatrix.h"
#include "ScaleMatrix.h"
#include "ViewPointDefaults.h"

GridDrawNew::GridDrawNew(const CoordinateSystem &cs,const VisualAttLoader *visAttLoader)
:m_xShift(0),m_yShift(0),m_zShift(0)
,m_isVisible (true)
,m_visAttLoader(visAttLoader)
,m_cs(cs)
,m_absLinePoints(7)
//,m_whiteMesh(25)
//,m_blackMesh(25)
{
	m_unitLength = 5;
	m_xCellCount = 10;
	m_yCellCount = 10;
	m_visAtt.SetDraw (true);
	//VColor newcolor(0.5,0.5,0.75);
	m_visAtt.ChangeWireColor (VGlobalValues::viewpointdefaults.m_defaultGridColor);
	//m_visAtt.SetLineWidth (0.5);
	m_visAtt.SetLineWidth(LineVisualAtt::thin);
	m_selcolor = VColor (1.0,0.5,0.5);

	m_drawAbsCoords = true;
	m_absLineVisAtt.SetLineFont (LineVisualAtt::LINEFONT_DOTTED);
	m_absLineVisAtt.SetLineWidth (LineVisualAtt::thin);
	m_absLineVisAtt.ChangeWireColor (VGlobalValues::viewpointdefaults.m_defaultOriginAxLineColor);
	m_absLinesLength = 500.0;
	BuildAbsCoordGraphics();
}

const CoordinateSystem & GridDrawNew::GetCs() const
{
	return m_cs;
}

//void GridDrawNew::SetCs(const CoordinateSystem &cs)
//{
//	m_cs = cs;
//}

double GridDrawNew::GetUnitLength() const
{
	return m_unitLength;
}

void GridDrawNew::SetUnitLength (double unitLength)
{
	m_unitLength = unitLength;
}

int GridDrawNew::GetXCellCount() const
{
	return m_xCellCount;
}

void GridDrawNew::SetXCellCount(int xCellCount)
{
	m_xCellCount = xCellCount;
}

int GridDrawNew::GetYCellCount() const
{
	return m_yCellCount;
}

void GridDrawNew::SetYCellCount(int yCellCount)
{
	m_yCellCount = yCellCount;
}

double GridDrawNew::GetXShift() const
{
	return m_xShift;
}

void GridDrawNew::SetXShift(double xShift)
{
	m_xShift = xShift;
}

double GridDrawNew::GetYShift() const
{
	return m_yShift;
}

void GridDrawNew::SetYShift (double yShift)
{
	m_yShift = yShift;
}

void GridDrawNew::SetZShift (double zShift)
{
	m_zShift = zShift;
}

double GridDrawNew::GetZShift() const
{
	return m_zShift;
}

void GridDrawNew::Draw()
{
	if (m_isVisible)
	{
		m_visAttLoader->Load (m_visAtt,false,m_selcolor);
		VPoint minInCs = VPoint ((-m_unitLength * 0.5 * m_xCellCount)+m_xShift , (-m_unitLength * 0.5 * m_yCellCount) + m_yShift , m_zShift);
		VPoint maxInCs = VPoint ((m_unitLength * 0.5 * m_xCellCount)+m_xShift , (m_unitLength * 0.5 * m_yCellCount)+m_yShift , m_zShift);

		glBegin ( GL_LINES );
			// x lines by ofseting along y
			for (int i=0;i<m_yCellCount+1;++i)
			{
				VPoint ls (minInCs.x(),minInCs.y()+m_unitLength*i,m_zShift);
				VPoint le (maxInCs.x(),minInCs.y()+m_unitLength*i,m_zShift);
				VPoint als = m_cs.RelativeCoordToAbsoluteCoord (ls);
				VPoint ale = m_cs.RelativeCoordToAbsoluteCoord (le);
				glVertex3dv ( als.GetArray() );
				glVertex3dv ( ale.GetArray() );
			}
			// y lines by ofseting along x
			for (int i=0;i<m_xCellCount+1;++i)
			{
				VPoint ls (minInCs.x()+i*m_unitLength,minInCs.y(),m_zShift);
				VPoint le (minInCs.x()+i*m_unitLength,maxInCs.y(),m_zShift);
				VPoint als = m_cs.RelativeCoordToAbsoluteCoord (ls);
				VPoint ale = m_cs.RelativeCoordToAbsoluteCoord (le);
				glVertex3dv ( als.GetArray());
				glVertex3dv ( ale.GetArray());
			}
		glEnd();
	}
	if (m_drawAbsCoords)
	{
		m_visAttLoader->Load (m_absLineVisAtt , false , m_selcolor);
		glBegin (GL_LINES);
		for (int i=0;i<6;++i)
		{
			glVertex3d (m_absLinePoints[i]->x(),m_absLinePoints[i]->y(),m_absLinePoints[i]->z());
			++i;
			glVertex3d (m_absLinePoints[i]->x(),m_absLinePoints[i]->y(),m_absLinePoints[i]->z());
		}

		glEnd();
	}
}

//void GridDrawNew::DrawAbs ()
//{
//	if (m_drawAbsCoords)
//	{
//		m_visAttLoader->Load (m_whiteVisAtt , false , m_selcolor);
//		glBegin (GL_QUADS);
//		
//		for (int i=0;i<24;++i)
//		{
//			glVertex3d ( m_whiteMesh[i]->x(),m_whiteMesh[i]->y(),m_whiteMesh[i]->z());
//			i++;
//			glVertex3d ( m_whiteMesh[i]->x(),m_whiteMesh[i]->y(),m_whiteMesh[i]->z());
//			i++;
//			glVertex3d ( m_whiteMesh[i]->x(),m_whiteMesh[i]->y(),m_whiteMesh[i]->z());
//			i++;
//			glVertex3d ( m_whiteMesh[i]->x(),m_whiteMesh[i]->y(),m_whiteMesh[i]->z());
//		}
//		
//		glEnd ();
//
//		m_visAttLoader->Load (m_blackVisAtt , false, m_selcolor);
//		
//		glBegin (GL_QUADS);
//		
//		for (int i=0;i<24;++i)
//		{
//			glVertex3d ( m_blackMesh[i]->x(),m_blackMesh[i]->y(),m_blackMesh[i]->z());
//			++i;
//			glVertex3d ( m_blackMesh[i]->x(),m_blackMesh[i]->y(),m_blackMesh[i]->z());
//			++i;
//			glVertex3d ( m_blackMesh[i]->x(),m_blackMesh[i]->y(),m_blackMesh[i]->z());
//			++i;
//			glVertex3d ( m_blackMesh[i]->x(),m_blackMesh[i]->y(),m_blackMesh[i]->z());			
//		}
//		
//		glEnd ();	
//	}
//}

bool GridDrawNew::GetIsVisible()const
{
	return m_isVisible;
}

void GridDrawNew::SetIsVisible (bool isVisible)
{
	m_isVisible = isVisible;
}

const LineVisualAtt & GridDrawNew::GetVisAtt()const
{
	return m_visAtt;
}
	
void GridDrawNew::SetVisAtt (const LineVisualAtt &visAtt)
{
	m_visAtt = visAtt;
}

const LineVisualAtt & GridDrawNew::GetAbsLineVisAtt()const
{
	return m_absLineVisAtt;
}

void GridDrawNew::SetAbsLinesVisAtt (const LineVisualAtt &absLineVisAtt)
{
	m_absLineVisAtt = absLineVisAtt;
}

void GridDrawNew::GetHitPointsNew (PointSet &selpoints , DoubleVec &depthvalues,const VPoint &curserp,const VPoint &lookdir,double selr)const
{
	//int mysize = snapent.snappoints.Size();
	if (this->GetIsVisible() == false)
		return;
	VPoint minInCs = VPoint ((-m_unitLength * 0.5 * m_xCellCount)+m_xShift , (-m_unitLength * 0.5 * m_yCellCount) + m_yShift , m_zShift);
	LinePointMinDist calc;
	for (int x=0;x<=m_xCellCount;++x)
	{
		for (int y=0;y<=m_yCellCount;++y)
		{
			//const VPoint *curp = snapent.snappoints.operator [] (i);
			VPoint curp (x*m_unitLength , y*m_unitLength , 0.0);
			curp += minInCs;
			curp = m_cs.RelativeCoordToAbsoluteCoord (curp);
			double curdist = calc.InfiniteLinePointMinDist (curserp , lookdir , curp);
			if (curdist < selr)
			{
				selpoints.InsertPoint (curp);
				VPoint diff = (curp - curserp);
				double depthlen2 =  (diff.GetLength2() - curdist*curdist);
				if (diff % lookdir < 0.0)
					depthlen2 *= -1.;
				depthvalues.Addvreal (depthlen2);
			}
		}
	}
}

void GridDrawNew::BuildAbsCoordGraphics()
{
	//m_whiteVisAtt.ChangeShadeColor (VColor (1,1,1));
	//m_blackVisAtt.ChangeShadeColor (VColor (0,0,0));
	//
	//VPoint c1(0,0,0);
	//VPoint c2(10,0,0);
	//VPoint c3 (10,10,0);
	//VPoint c4 (0,10,0);

	//m_whiteMesh.InsertPoint (c1);
	//m_whiteMesh.InsertPoint (c2);
	//m_whiteMesh.InsertPoint (c3);
	//m_whiteMesh.InsertPoint (c4);

	//MoveMatrix M1 (VPoint (-10,-10,0));
	//c1 = M1.Transform(c1);
	//c2 = M1.Transform(c2);
	//c3 = M1.Transform(c3);
	//c4 = M1.Transform(c4);


	//m_whiteMesh.InsertPoint (c1);
	//m_whiteMesh.InsertPoint (c2);
	//m_whiteMesh.InsertPoint (c3);
	//m_whiteMesh.InsertPoint (c4);

	//m_blackMesh = m_whiteMesh;
	//RotationMatrix R ( VPoint (0.),UZ,90.0);
	//m_blackMesh.Transform (R);

	//PointSet tmpwhite1 (m_whiteMesh);
	//RotationMatrix R1 (VPoint(0.0),UX , 90.0);
	//tmpwhite1.Transform (R1);
	//PointSet tmpblack1 (m_blackMesh);
	//tmpblack1.Transform (R1);
	//PointSet tmpwhite2 (tmpwhite1);
	//tmpwhite2.Transform (R);
	//PointSet tmpblack2 (tmpblack1);
	//tmpblack2.Transform (R);
	//
	//m_whiteMesh.AddBack (tmpwhite1);
	//m_whiteMesh.AddBack (tmpwhite2);
	//m_blackMesh.AddBack (tmpblack1);
	//m_blackMesh.AddBack (tmpblack2);


	m_absLinePoints.InsertPoint (VPoint (-m_absLinesLength*0.5,0,0));
	m_absLinePoints.InsertPoint (VPoint (m_absLinesLength*0.5,0,0));
	m_absLinePoints.InsertPoint(VPoint (0,-m_absLinesLength*0.5,0));
	m_absLinePoints.InsertPoint (VPoint (0,m_absLinesLength*0.5,0));
	m_absLinePoints.InsertPoint (VPoint (0,0,-m_absLinesLength*0.5));
	m_absLinePoints.InsertPoint (VPoint (0,0,m_absLinesLength*0.5));
}

double GridDrawNew::GetAbsLinesLength() const
{
	return m_absLinesLength;
}

void GridDrawNew::SetAbsLinesLength(double absLinesLength)
{
	m_absLinesLength = absLinesLength;
}

void GridDrawNew::GetMinMax(VPoint &mn , VPoint &mx)const
{
	mn = VPoint(m_cs.GetOrigin() + m_cs.GetDirx()*m_unitLength*m_xCellCount);
	mx = VPoint(m_cs.GetOrigin() + m_cs.GetDiry()*m_unitLength*m_yCellCount);
	VPoint delta = mx - mn;
	vreal deltaBig = delta.x() > delta.y() ? delta.x() : delta.y();
	VPoint upVec = m_cs.GetDirz()*deltaBig;
	VPoint relz0 = m_cs.GetOrigin() - upVec;
	VPoint relz1 = m_cs.GetOrigin() + upVec;
	mn.ReplaceMin(relz0);
	mx.ReplaceMax(relz1);
}
