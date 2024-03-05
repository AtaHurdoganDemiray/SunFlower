// QueryVisible.cpp

#include "stdafx.h"
#include "QueryVisible.h"
#include "OGL_Frame.h"
#include "VRefTriangleMesh.h"
#include "PointSet.h"
#include "Display.h"
#include "VPoint.h"
#include "CoordinateSystem.h"
//#include "Display.h"
#include "Display.h"
#include "TransformationMatrix.h"

vreal QueryVisible::m_resolution = 0.04;

QueryVisible::QueryVisible (OGL_Frame &oglFrame ,const vreal &zoffset)
:m_oglFrame(oglFrame)
{
	CoordinateSystem cs = m_oglFrame.GetLookCoordinateSystem();
	m_xOffset =	/*cs.GetDirx()*/ UX * m_resolution;
	m_yOffset = /*cs.GetDiry()*/ UY * m_resolution;
	m_zOffset = cs.GetDirz() * m_resolution * zoffset;
	
}

void QueryVisible::Query (const VRefTriangleMesh &body
						, const Display *myself
						, const std::list<Display *> &others
						, std::list <int> &visible
						, std::list <int> &invisible )
{
	//for each point in VRefTriangleMesh
	const PointSet &ps = body.surfpack.modelpoints;
	int mysize = ps.Size();
	for (int i = 0;i<mysize; i++)
	{
		const VPoint *currp = ps[i];
		VPoint op = (*currp) + m_zOffset;

		SetViewVolumeAround (op);
		vreal z , znear;
		DrawPointInSelectionMode (op , z);
		glMatrixMode (GL_MODELVIEW);
		DrawOthersInSelectionMode (others , znear );
		if (z < znear)
		{
			if (IsMySelfVis (op , myself , z) == true)
				visible.push_back (i);
			else
				invisible.push_back (i);		
		}
		else
			invisible.push_back(i);
	}

}

void QueryVisible::Query (const PointSet &body
						, const Display *myself
						, const std::list<Display *> &others
						//, std::list<bool> &isHidden
						, std::list <int> &visible
						, std::list <int> &invisible )
{
	int mysize = body.Size();
	for (int i = 0;i<mysize; i++)
	{
		const VPoint *currp = body[i];
		VPoint op = (*currp) + m_zOffset;

		SetViewVolumeAround (op);
		vreal z , znear;
		DrawPointInSelectionMode (op , z);
		glMatrixMode (GL_MODELVIEW);
		//DrawOthersInSelectionMode (others , znear , z , isHidden);
		DrawOthersInSelectionMode (others , znear);
		
		if (z < znear)
		{
			if (IsMySelfVis (op , myself , z) == true)
				visible.push_back (i);
			else
				invisible.push_back (i);		
		}
		else
			invisible.push_back(i);
	}
}

void QueryVisible::SetViewVolumeAround (const VPoint &p)
{
	// Current matrix will be model view matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();      //Initialize matrix	
	glMultMatrixd(m_oglFrame.quat.QuatToMat()); 
	// Making view center the origin
	VPoint rot_center (m_oglFrame.rot_center);
	glTranslated ( -rot_center.x() , -rot_center.y() , -rot_center.z() );
	
	TransformationMatrix tm = m_oglFrame.GetOglModelTransformMatrix();
	VPoint tp = tm.Transform(p);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();		//Initialize matrix

	ViewVolume vol (m_oglFrame.view_volume);
	vol.SetLeft (tp.x()-m_xOffset.x());
	vol.SetRight (tp.x()+m_xOffset.x());
	vol.SetTop (tp.y()+m_yOffset.y());
	vol.SetBottom (tp.y()-m_yOffset.y());
	vol.SetViewVolume();
}
	
void QueryVisible::DrawPointInSelectionMode (const VPoint &p , vreal &z)
{
	vreal tempdepthvalue = 1.0 / EPSILON;

	unsigned int sel_ind[500];	
	glSelectBuffer ( 500 , sel_ind );
	glRenderMode ( GL_SELECT );
	glInitNames();
	glPushName(0);
	
		glBegin ( GL_POINTS );
		glVertex3f ( static_cast <GLfloat> (p.x())
					,static_cast <GLfloat> (p.y())
					,static_cast <GLfloat> (p.z()) );
		glEnd ();

	// get the selected points from record
	int hits = glRenderMode ( GL_RENDER );
	int offset = 0;

	for ( int i = 0; i < hits; i++ )
	{
		int nameno = sel_ind[offset++];
	
		vreal z1 =(static_cast<vreal> (sel_ind[offset++]))/OGL_SEL_ZCONST;
		vreal z2 =(static_cast<vreal> (sel_ind[offset++]))/OGL_SEL_ZCONST;
		
		if ( z1 < tempdepthvalue )
			tempdepthvalue = z1;
		if ( z2 < tempdepthvalue )
			tempdepthvalue = z2;

		for ( int j = 0; j < nameno; j++  )
		{
			int name = sel_ind[offset++];
		}
	}
	z = tempdepthvalue;
}
	
void QueryVisible::DrawOthersInSelectionMode ( const std::list<Display *> &others
												, vreal &znear)
{
	znear = 1. / EPSILON;
	std::list<Display *>::const_iterator i = others.begin();
	std::list<Display *>::const_iterator e = others.end();
	for(i;i!=e;i++)
	{
		vreal ztmp = 1. / EPSILON;
		(*i)->IsHitFacet (ztmp);
		if (ztmp < znear)
		{
			znear = ztmp;
		}
	}
}

void QueryVisible::DrawOthersInSelectionMode ( const std::list<Display *> &others
												, vreal &znear
												, const vreal &zbody
												, std::list<bool> &isHidden)
{
	znear = 1. / EPSILON;
	std::list<Display *>::const_iterator i = others.begin();
	std::list<Display *>::const_iterator e = others.end();
	std::list<bool>::iterator bi = isHidden.begin();
	for(i;i!=e;i++)
	{
		vreal ztmp = 1. / EPSILON;
		bool ck = (*i)->IsHitFacet (ztmp);
		if (ck)
		{
			if (ztmp < znear)
			{
				znear = ztmp;
			}
			if (zbody < ztmp)
			{
				(*bi) = true;//surface is hidden by checked point
			}
		}
		bi++;
	}
}

bool QueryVisible::IsMySelfVis (const VPoint &p 
								, const Display *myself 
								, const vreal &zmyself)
{	
	vreal ztmp = 1. / EPSILON;
	SetViewVolumeAround (p);
	myself->IsHitFacet(ztmp);
	if (zmyself < ztmp)
		return true;
	else
		return false;
}
									
