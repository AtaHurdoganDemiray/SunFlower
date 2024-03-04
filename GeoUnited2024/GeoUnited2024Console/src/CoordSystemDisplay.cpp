// CoordSystemDisplay.cpp

#include "stdafx.h"
#include "CoordSystemDisplay.h"
#include "CoordinateSystem.h"
#include "ViewVolume.h"
//#include "gl/glut.h"
#include "PointSet.h"
#include "TransformationMatrix.h"
#include "TransformData.h"
#include "ViewPointDefaults.h"
#include "TextBox.h"
#include "OglWindowData.h"

#define X 0
#define Y 1
#define Z 2

CoordSystemDisplay::CoordSystemDisplay ( const CoordinateSystem * cs , LineVisualAtt & _visatt )
	:visatt ( _visatt ) , m_cs (cs) , fontcolor (VGlobalValues::viewpointdefaults.m_defaultWireframeColor)
{
	//fontcolor.R(0.0);
	//fontcolor.G(0.0);
	//fontcolor.B(0.25);

	//originalCS = cs;

	//origin[0]=cs.GetOrigin().x();
	//origin[1]=cs.GetOrigin().y();
	//origin[2]=cs.GetOrigin().z();

	//dirx[0]=cs.GetDirx().x();
	//dirx[1]=cs.GetDirx().y();
	//dirx[2]=cs.GetDirx().z();

	//diry[0]=cs.GetDiry().x();
	//diry[1]=cs.GetDiry().y();
	//diry[2]=cs.GetDiry().z();

	//dirz[0]=cs.GetDirz().x();
	//dirz[1]=cs.GetDirz().y();
	//dirz[2]=cs.GetDirz().z();
	//PointSet tmp (1);
	//tmp.InsertPoint(cs.GetOrigin().GetArray() );
	//org.MakeDisplayList(tmp);
	//
	////VColor defaultcol ( 0.5803 , 0.0039 , 0.0470 );
	////VColor defcol ( 0.1 , 0.5 , 1. );
	////visatt.ChangeWireColor ( defcol );
	////visatt.SetDraw(true);
}

CoordSystemDisplay::CoordSystemDisplay ( CoordSystemDisplay & Foo )
:m_cs(Foo.m_cs),visatt(Foo.visatt),org(Foo.org),fontcolor(Foo.fontcolor) 
{
	//origin[0]=Foo.origin[0];
	//origin[1]=Foo.origin[1];
	//origin[2]=Foo.origin[2];

	//dirx[0]=Foo.dirx[0];
	//dirx[1]=Foo.dirx[1];
	//dirx[2]=Foo.dirx[2];

	//diry[0]=Foo.diry[0];
	//diry[1]=Foo.diry[1];
	//diry[2]=Foo.diry[2];
	//
	//dirz[0]=Foo.dirz[0];
	//dirz[1]=Foo.dirz[1];
	//dirz[2]=Foo.dirz[2];
}

void CoordSystemDisplay::SetCs (const CoordinateSystem *cs)
{
	m_cs = cs;
}


void CoordSystemDisplay::Draw(const ViewVolume & view_volume, const std::vector<Display *> *asciiDisp) const
{
	GLfloat inform[4],infx[4],infy[4],infz[4];
	glRasterPos3f( (GLfloat)m_cs->GetOrigin().x() 
		, (GLfloat)m_cs->GetOrigin().y()  
		, (GLfloat)m_cs->GetOrigin().z() );
	
	GLboolean chk ;

	glGetBooleanv( GL_CURRENT_RASTER_POSITION_VALID , &chk );

	if (!chk )
		return;

	glGetFloatv( GL_CURRENT_RASTER_POSITION , inform );
	
//	glRasterPos3f( 0.0f , 0.0f , 0.0f ); 

//	glGetBooleanv( GL_CURRENT_RASTER_POSITION_VALID , &chk );

//	if (!chk )
//		return;

					
//	glGetGLfloatv( GL_CURRENT_RASTER_POSITION , inf0 );;
	 
	glRasterPos3f( (GLfloat)(m_cs->GetOrigin().x()+m_cs->GetDirx().x())//dirx[X]) 
						, (GLfloat)(m_cs->GetOrigin().y()+m_cs->GetDirx().y())//dirx[Y])  
						, (GLfloat)(m_cs->GetOrigin().z()+m_cs->GetDirx().z()));//dirx[Z]) );

	glGetBooleanv( GL_CURRENT_RASTER_POSITION_VALID , &chk );

	if (!chk )
		return;

	glGetFloatv( GL_CURRENT_RASTER_POSITION , infx );

	glRasterPos3f( (GLfloat)m_cs->GetOrigin().x()+m_cs->GetDiry().x()//origin[X]+diry[X]) 
		, (GLfloat)m_cs->GetOrigin().y()+m_cs->GetDiry().y()//(origin[Y]+diry[Y])  
		, (GLfloat)m_cs->GetOrigin().z()+m_cs->GetDiry().z());//(origin[Z]+diry[Z]) );

	glGetBooleanv( GL_CURRENT_RASTER_POSITION_VALID , &chk );

	if (!chk )
		return;
	
	
	glGetFloatv( GL_CURRENT_RASTER_POSITION , infy );

	glRasterPos3f( (GLfloat)m_cs->GetOrigin().x()+m_cs->GetDirz().x()//(origin[X]+dirz[X]) 
						,(GLfloat)m_cs->GetOrigin().y()+m_cs->GetDirz().y()//(origin[Y]+dirz[Y])  
						,(GLfloat)m_cs->GetOrigin().z()+m_cs->GetDirz().z());//(origin[Z]+dirz[Z]) );

	glGetBooleanv( GL_CURRENT_RASTER_POSITION_VALID , &chk );

	if (!chk )
		return;
	
	glGetFloatv( GL_CURRENT_RASTER_POSITION , infz );
	DrawByScreenCoord (inform ,infx,infy,infz,view_volume,asciiDisp);
}

void CoordSystemDisplay::DrawByScreenCoord (const float inform[] 
											, const float infx[] 
											, const float infy[] 
											,const float infz[]
											,const ViewVolume &view_volume
											, const std::vector<Display*> *asciiDisp)const
{
	vreal xd_sc[3] , yd_sc[3] , zd_sc[3];

	xd_sc[X] = infx[X] - inform[X];
	xd_sc[Y] = infx[Y] - inform[Y];
	xd_sc[Z] = 0.0;

	yd_sc[X] = infy[X] - inform[X];
	yd_sc[Y] = infy[Y] - inform[Y];
	yd_sc[Z] = 0.0;

	zd_sc[X] = infz[X] - inform[X];
	zd_sc[Y] = infz[Y] - inform[Y];
	zd_sc[Z] = 0.0;

	//v_norm_3d ( xd_sc , xd_sc );
	//v_norm_3d ( yd_sc , yd_sc );
	//v_norm_3d ( zd_sc , zd_sc );

	vreal fac = (view_volume.m_right - view_volume.m_left) / 25.0;

	//v_scale_3d ( fac , xd_sc , xd_sc );
	xd_sc[0]*=fac;
	xd_sc[1]*=fac;
	xd_sc[2]*=fac;

	//v_scale_3d ( fac , yd_sc , yd_sc );
	yd_sc[0]*=fac;
	yd_sc[1]*=fac;
	yd_sc[2]*=fac;
	
	//v_scale_3d ( fac , zd_sc , zd_sc );
	zd_sc[0]*=fac;
	zd_sc[1]*=fac;
	zd_sc[2]*=fac;

//	glPopMatrix();//display

	// Draw with open GL
	//glDisable ( GL_LIGHTING );
		
	//glColor3d ( displaycolor.R() , displaycolor.G() , displaycolor.B() );
	
	glMatrixMode ( GL_PROJECTION );
	glPushMatrix ();
	glLoadIdentity ();

	glOrtho ( 0.0 , (GLfloat) view_volume.corner_x , 0.0 , (GLfloat) view_volume.corner_y 
				, view_volume.m_near , view_volume.m_far );
	
	glMatrixMode ( GL_MODELVIEW );
	glPushMatrix ();
	glLoadIdentity ();
	
	vreal depth = (view_volume.m_far+view_volume.m_near) *0.5;
	//Drawing the axis
	glColor3d (1,0,0);
	//X 
	glBegin ( GL_LINES );
	glVertex3d ( inform[X] , inform[Y] , depth );
	glVertex3d ( inform[X]+xd_sc[X] , inform[Y]+xd_sc[Y] , depth );
	glEnd ();
	glColor3d (0,1,0);
	//Y
	glBegin ( GL_LINES );
	glVertex3d ( inform[X] , inform[Y] , depth );
	glVertex3d ( inform[X]+yd_sc[X] , inform[Y]+yd_sc[Y] , depth );
	glEnd ();
	glColor3d (0,0,1);
	//Z
	glBegin ( GL_LINES );
	glVertex3d ( inform[X] , inform[Y] , depth );
	glVertex3d ( inform[X]+zd_sc[X] , inform[Y]+zd_sc[Y] , depth );
	glEnd ();

	//Write the labels.
	//glColor3d ( fontcolor.R() , fontcolor.G() , fontcolor.B() );
	//glRasterPos3f( inform[X]+xd_sc[X] , inform[Y]+xd_sc[Y]+4.0f , depth );
	//glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'X');
	

	//glRasterPos3f( inform[X]+yd_sc[X] , inform[Y]+yd_sc[Y]+4.0f , depth );
	//glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'Y');

	//glRasterPos3f(  , depth );
	//glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'Z');
	OglWindowData wd(view_volume.corner_x, view_volume.corner_y, view_volume.m_near , depth);
	TextBox xlet(6.0, 6.0, "X" , asciiDisp);
	TextBox ylet(6., 6., "Y" , asciiDisp);
	TextBox zlet(6., 6., "Z" , asciiDisp);
	xlet.fontw = 6.0;
	xlet.fonth = 6.0;
	ylet.fontw = 6.0;
	ylet.fonth = 6.0;
	zlet.fontw = 6.0;
	zlet.fonth = 6.0;
	GLfloat b1[2] = { inform[X] + xd_sc[X], inform[Y] + xd_sc[Y] + 4.0f};
	GLfloat b2[2] = { inform[X] + yd_sc[X], inform[Y] + yd_sc[Y] + 4.0f};
	GLfloat b3[2] = { inform[X] + zd_sc[X], inform[Y] + zd_sc[Y] + 4.0f};
	TextBoxPosData xpos(b1, b1, b1);
	TextBoxPosData ypos(b2, b2, b2);
	TextBoxPosData zpos(b3, b3, b3);
	TextBoxDisplayer d1(wd);
	//d3.DisplayOnlyText(zlet, zpos);
	//d2.DisplayOnlyText(ylet, ypos);
	d1.DisplayOnlyText(xlet, xpos);
	d1.DisplayOnlyText(ylet, ypos);
	d1.DisplayOnlyText(zlet, zpos);

	glMatrixMode ( GL_PROJECTION );
	glPopMatrix ();
	glMatrixMode ( GL_MODELVIEW );
	glPopMatrix ();

}

bool CoordSystemDisplay::IsHit ( vreal & depthvalue ) const
{
	return(org.IsHit ( depthvalue ));
}

void CoordSystemDisplay::GetHitPoints ( PointSet & selpoints , DoubleVec & depthvalues ) const
{
	org.GetHitPoints ( selpoints , depthvalues );
}

LineVisualAtt & CoordSystemDisplay::GetVisualAtt ()
{
	return visatt;
}

//void CoordSystemDisplay::UpdateCoordSystemData ( const TransformationMatrix & tmatrix , OglMatrix *x)
//{
//	CoordinateSystem newc = tmatrix.Transform ( originalCS );
//	
//	dirx[X]=newc.GetDirx().x();
//	dirx[Y]=newc.GetDirx().y();
//	dirx[Z]=newc.GetDirx().z();
//
//	diry[X]=newc.GetDiry().x();
//	diry[Y]=newc.GetDiry().y();
//	diry[Z]=newc.GetDiry().z();
//
//	dirz[X]=newc.GetDirz().x();
//	dirz[Y]=newc.GetDirz().y();
//	dirz[Z]=newc.GetDirz().z();
//
//	origin[X]=newc.GetOrigin().x();
//	origin[Y]=newc.GetOrigin().y();
//	origin[Z]=newc.GetOrigin().z();
//	
//	//org.Xmatrix = Xmatrix;
//	//be careful , this is the overall matrix not single transform
//	
//	//TransformationMatrix tmp(tmatrix);
//	//org.xmats.clear();
//	//org.AddMatrix(tmp);
//}
//void CoordSystemDisplay::UpdateCoordSystemData (const CoordinateSystem &newc)
//{
//	dirx[X]=newc.GetDirx().x();
//	dirx[Y]=newc.GetDirx().y();
//	dirx[Z]=newc.GetDirx().z();
//
//	diry[X]=newc.GetDiry().x();
//	diry[Y]=newc.GetDiry().y();
//	diry[Z]=newc.GetDiry().z();
//
//	dirz[X]=newc.GetDirz().x();
//	dirz[Y]=newc.GetDirz().y();
//	dirz[Z]=newc.GetDirz().z();
//
//	origin[X]=newc.GetOrigin().x();
//	origin[Y]=newc.GetOrigin().y();
//	origin[Z]=newc.GetOrigin().z();
//}
//void CoordSystemDisplay::AddMatrix(TransformData *x)
//{
//	xmats.push_front(x);
//	org.AddMatrix(x);
//
//}
//
////void CoordSystemDisplay::MatrisClear()
////{
////	xmats.clear();
////	org.xmats.clear();
////}
//
//TransformationMatrix CoordSystemDisplay::GetOverallMat()const
//{
//	return org.GetOverallMat();
//}
