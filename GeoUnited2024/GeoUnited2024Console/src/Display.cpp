// Display.cpp

#include "stdafx.h"
#include "Display.h"
#include "TransformData.h"
#include "OglMatrix.h"
#include "VTexture.h"
#include "QuadPointGenerator.h"
#include "ViewPointDefaults.h"

GLfloat Display::selectionbuff[SELECTION_BUFFER_SIZE];
bool Display::m_isGraphicViewChanging = false;
size_t Display::QUICK_STEP = 1;

int Display::m_maxOnLineItemDrawLimit = 100000;

Display::Display()
{
	//disp_list = 0;
	m_dispListLines = 0;
	m_dispListFacet = 0;
	m_dispListPoints = 0;
	//usercount = new int(0);	
	//m_isScaled = false;
	//m_isMirrored = false;

	// From LinesDisplay
	m_seqLine = 0;

	// From FacetDisplay
	m_ftri=0;
}

Display::Display(const Display &Foo)
{
	//m_isMirrored = tocopy.m_isMirrored;
	//m_isScaled = tocopy.m_isScaled;
	//xmats=tocopy.xmats;
	//disp_list = Foo.disp_list;
	m_dispListLines = Foo.m_dispListLines;
	m_dispListFacet = Foo.m_dispListFacet;
	m_dispListPoints = Foo.m_dispListPoints;
	//usercount = tocopy.usercount;
	//(*usercount)++;

	//Xmatrix = Foo.Xmatrix;
	
	// LinesDisplay
	visualatt = Foo.visualatt;
	m_seqLine = Foo.m_seqLine;
	m_drwInt = Foo.m_drwInt;
	m_visualAttVec = Foo.m_visualAttVec;
	m_limitbi = Foo.m_limitbi;
	m_limitei = Foo.m_limitei;
	// LinesDisplay
	// FacetDisplay
	visualattFacet = Foo.visualattFacet;
	m_ftri = Foo.m_ftri;
	// FacetDisplay
	// PointsDisplay
	visualattPoints = Foo.visualattPoints;
}

Display::~Display()
{
	//(*usercount)--;

	//if ( (*usercount) == 0 )
	DestroyDisplayListLines();
	DestroyDisplayListFacet();
	DestroyDisplayListPoints();
	//if ((*usercount)<1)
	//	delete usercount;
}
void Display::DestroyDisplayListLines()
{
	//if (disp_list)
	//{
	//	glDeleteLists ( disp_list , 1 );
	//	disp_list = 0;
	//}
	if (m_dispListLines)
	{
		glDeleteLists ( m_dispListLines , 1);
		m_dispListLines = 0;
	}
}
void Display::DestroyDisplayListFacet()
{
	if (m_dispListFacet)
	{
		glDeleteLists ( m_dispListFacet , 1);
		m_dispListFacet = 0;
	}
}
void Display::DestroyDisplayListPoints()
{
	if (m_dispListPoints)
	{
		glDeleteLists ( m_dispListPoints , 1);
		m_dispListPoints = 0;
	}
}
//void Display::Draw()const
//{
//	if ( disp_list )
//	{
//		////glMatrixMode(GL_MODELVIEW);
//		//glPushMatrix ();
//		////if ( Xmatrix )
//		////	glMultMatrixd(Xmatrix); 
//		//if (xmats.size()>0)
//		//{
//		//	std::list<TransformData*>::const_iterator itr;
//		//	for(itr=xmats.begin();itr!=xmats.end();itr++)
//		//	{
//		//		if ((*itr)->isactive)
//		//			glMultMatrixd((*itr)->m_om.GetArray());
//		//	}
//		//}
//
//
//		//if ( IsScaled() )
//		//	glEnable ( GL_NORMALIZE );
//		//else
//		//	glDisable ( GL_NORMALIZE );
//
//		//if ( IsMirrored () )
//		//	glFrontFace(GL_CW);
//		//else
//		//	glFrontFace(GL_CCW);   
//
//		glCallList ( disp_list );
//		
//		//glPopMatrix();
//	}
//	else
//	{
//		OnlineDraw();
//	}
//}
void Display::DrawPoints()const
{
	if (m_dispListPoints)
	{
		glCallList (m_dispListPoints);
	}
	else
	{
		OnlineDrawPoints ();
	}
	DrawAttachedDisplayPoints();
}
void Display::DrawLines(bool isSelected)const
{
	if (m_dispListLines)
	{
		glCallList (m_dispListLines);
	}
	else
	{
		OnlineDraw(isSelected);
	}
	DrawAttachedDisplay(isSelected);
}
void Display::DrawFacet()const
{
	if (m_dispListFacet)
	{
		glCallList (m_dispListFacet);
	}
	else
	{
		OnlineDrawFacet();
	}
	DrawAttachedDisplayFacet();
}
bool Display::IsScaled (const OglMatrix &m)
{
	const vreal *Xmatrix= m.GetArray();
	if ( fabs( (Xmatrix[0]*Xmatrix[0] + Xmatrix[5]*Xmatrix[5] + Xmatrix[10]*Xmatrix[10]) - 1. ) > 1.e-6 )
		return true;
	return false;
}
bool Display::IsMirrored (const OglMatrix &m)
{
	const vreal *Xmatrix=m.GetArray();
	if ( Xmatrix[0] * Xmatrix[5] * Xmatrix[10] < 0.0 )
		return true;
	return false;
}

//bool Display::IsScaled() const
//{
//	return m_isScaled;
//}
//
//bool Display::IsMirrored() const
//{
//	return m_isMirrored;
//}

//void Display::AddMatrix(TransformData *x)
//{
//	xmats.push_front(x);
//	//SetIsScaled();
//	//SetIsMirrored();
//}

bool Display::IsDefined() const
{
	//if ( disp_list )
	//	return true;
	//else
	//	return false;
	if (m_dispListLines)
		return true;
	if (m_dispListFacet)
		return true;
	if (m_dispListPoints)
		return true;
	return false;
}

Display & Display::operator = (const Display &tocopy)
{
	//m_isMirrored = tocopy.m_isMirrored;
	//m_isScaled = tocopy.m_isScaled;
	//xmats=tocopy.xmats;
	//disp_list = tocopy.disp_list;
	m_dispListPoints = tocopy.m_dispListPoints;
	m_dispListLines = tocopy.m_dispListLines;
	m_dispListFacet = tocopy.m_dispListFacet;

	//usercount = tocopy.usercount;
	//(*usercount)++;
	//return (*this);

	// LinesDisplay
	visualatt = tocopy.visualatt;
	m_seqLine = tocopy.m_seqLine;
	m_drwInt = tocopy.m_drwInt;
	m_visualAttVec = tocopy.m_visualAttVec;
	m_limitbi = tocopy.m_limitbi;
	m_limitei = tocopy.m_limitei;
	//return (*this);
	// LinesDisplay

	//FacetDisplay & FacetDisplay::operator = ( const FacetDisplay & Foo )
	//Display::operator =(Foo);
	visualattFacet = tocopy.visualattFacet;
	m_ftri = tocopy.m_ftri;
	//return (*this);	
	// FacetDisplay
	//PointsDisplay
	visualattPoints = tocopy.visualattPoints;
	return (*this);

}

//TransformationMatrix Display::GetOverallMat()const
//{
//	TransformationMatrix res;
//	std::list<TransformData*>::const_iterator itr;
//	for(itr=xmats.begin();itr!=xmats.end();itr++)
//	{
//		//TransformationMatrix currM (*(*itr));
//		if ((*itr)->isactive)
//			res *= (*itr)->m_tm;
//	}
//	return res;
//}


// LinesDisplay.cpp
//#include "stdafx.h"
#include "PointSet.h"
#include "VPoint.h"
//#include "LinesDisplay.h"
#include "DoubleVec.h"
#include "gl/glu.h"
#include "LineSet.h"
#include "SequentialLineSet.h"
//#include "SurfaceWireFrame.h"

#include "VisualAttLoader.h"

//Display::Display ()
//{
//	//Xmatrix = 0;
//	m_seqLine = 0;
//}

//Display::Display ( const Display & Foo )
//{
//	//Xmatrix = Foo.Xmatrix;
//	visualatt = Foo.visualatt;
//	m_seqLine = Foo.m_seqLine;
//	m_drwInt = Foo.m_drwInt;
//	m_visualAttVec = Foo.m_visualAttVec;
//	m_limitbi = Foo.m_limitbi;
//	m_limitei = Foo.m_limitei;
//}

Display::Display ( const LineSet & lines )
{
	m_dispListLines = 0;
	m_dispListFacet = 0;
	m_dispListPoints = 0;
	m_seqLine = 0;
	m_ftri=0;
	MakeDisplayList ( lines );
}

Display::Display ( const SequentialLineSet & seqlines )
{
	m_dispListLines = 0;
	m_dispListFacet = 0;
	m_dispListPoints = 0;
	m_seqLine = 0;
	m_ftri=0;
	MakeDisplayList ( seqlines );
}

//LinesDisplay::~LinesDisplay ()
//{
//}
/*
void LinesDisplay::Draw () const 
{
	if ( disp_list )
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix ();
		if (Xmatrix)
			glMultMatrixd(Xmatrix); 
		
		glCallList ( disp_list );

		glPopMatrix();
	}
}
*/
void Display::GetHitPoints ( PointSet & selpoints , DoubleVec & depthvalues ) const
{
	//if ( disp_list == 0 )
	//	return;
	if (m_dispListLines == 0 && m_seqLine == 0)
		return;
	vreal projMatrix[16];
	vreal modelMatrix[16];
	int viewPort[4];
	glGetDoublev ( GL_MODELVIEW_MATRIX , modelMatrix );
	glGetDoublev ( GL_PROJECTION_MATRIX , projMatrix );
	glGetIntegerv ( GL_VIEWPORT , viewPort );
		
	glFeedbackBuffer ( SELECTION_BUFFER_SIZE , GL_3D , selectionbuff );
	glRenderMode ( GL_FEEDBACK );
	
	// Draw all triangles and record the ones that are hit
	
	if (m_seqLine == 0)
		DrawLines(false);		
	else
		OnlineDraw (false);

	// get the selected points from record
	int count = glRenderMode ( GL_RENDER );
	int currcount = 0;

 	while ( currcount < count )
	{	
		GLfloat token = selectionbuff[currcount];
		currcount++;

		if ( token == GL_LINE_TOKEN || token == GL_LINE_RESET_TOKEN )
		{
			for ( int j=0; j <2; j++ )
			{
				vreal xyz[3];
				xyz[0] = static_cast <vreal> (selectionbuff[currcount]);
				currcount++;
				xyz[1] = static_cast <vreal> (selectionbuff[currcount]);
				currcount++;
				xyz[2] = static_cast <vreal> (selectionbuff[currcount]);
				currcount++;
						
				vreal result[3];
				gluUnProject ( xyz[0] , xyz[1] , xyz[2] , modelMatrix , projMatrix , viewPort , 
								&(result[0]) , &(result[1]) , &(result[2]) );
				
				selpoints.InsertPoint  ( result );
				depthvalues.Addvreal ( xyz[2] );
			}	
		}
	}
	
}

void Display::GetHitLines ( LineSet & sellines , DoubleVec & depthvalues ) const
{
	//if ( disp_list == 0 )
	//	return;
	if (m_dispListLines == 0 && m_seqLine == 0)
		return;
	vreal projMatrix[16];
	vreal modelMatrix[16];
	int viewPort[4];
	glGetDoublev ( GL_MODELVIEW_MATRIX , modelMatrix );
	glGetDoublev ( GL_PROJECTION_MATRIX , projMatrix );
	glGetIntegerv ( GL_VIEWPORT , viewPort );
		
	glFeedbackBuffer ( SELECTION_BUFFER_SIZE , GL_3D , selectionbuff );
	glRenderMode ( GL_FEEDBACK );
	
	// Draw all triangles and record the ones that are hit
	
	if (m_seqLine == 0)
		DrawLines(false);
	else
		OnlineDraw(false);

	// get the selected points from record
	int count = glRenderMode ( GL_RENDER );
	int currcount = 0;

 	while ( currcount < count )
	{	
		GLfloat token = selectionbuff[currcount];
		currcount++;

		if ( token == GL_LINE_TOKEN || token == GL_LINE_RESET_TOKEN )
		{
			//for ( int j=0; j <2; j++ )
			{
				vreal xyz1[3];
				vreal xyz2[3];
				xyz1[0] = static_cast <vreal> (selectionbuff[currcount]);
				currcount++;
				xyz1[1] = static_cast <vreal> (selectionbuff[currcount]);
				currcount++;
				xyz1[2] = static_cast <vreal> (selectionbuff[currcount]);
				currcount++;
				
				xyz2[0] = static_cast <vreal> (selectionbuff[currcount]);
				currcount++;
				xyz2[1] = static_cast <vreal> (selectionbuff[currcount]);
				currcount++;
				xyz2[2] = static_cast <vreal> (selectionbuff[currcount]);
				currcount++;
						
				vreal result1[3];
				vreal result2[3];

				gluUnProject ( xyz1[0] , xyz1[1] , xyz1[2] , modelMatrix , projMatrix , viewPort , 
								&(result1[0]) , &(result1[1]) , &(result1[2]) );
				
				gluUnProject ( xyz2[0] , xyz2[1] , xyz2[2] , modelMatrix , projMatrix , viewPort , 
								&(result2[0]) , &(result2[1]) , &(result2[2]) );				
				
				VPoint p1 ( result1 );
				VPoint p2 ( result2 );
				sellines.AddLine  ( p1 , p2  );
				if ( xyz1[2] > xyz2[2] )
					depthvalues.Addvreal ( xyz1[2] );
				else
					depthvalues.Addvreal ( xyz2[2] );
			}	
		}
	}
	
}

bool Display::IsHit ( vreal & depthvalue ) const
{
	//if ( disp_list == 0 )
	//	return false;
	if (m_dispListLines == 0 && m_seqLine == 0)
		return false;

	bool result = false;
	vreal tempdepthvalue = 1.0 / EPSILON;

	unsigned int sel_ind[500];	
	glSelectBuffer ( 500 , sel_ind );
	glRenderMode ( GL_SELECT );
	glInitNames();
	glPushName(0);

	if (m_seqLine == 0)
		DrawLines(false);
	else
		OnlineDraw(false);

	// get the selected points from record
	int hits = glRenderMode ( GL_RENDER );
	int offset = 0;

	for ( int i = 0; i < hits; i++ )
	{
		result = true;
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
	
	if ( result == true )
		depthvalue = tempdepthvalue;

	return result;
}

void Display::MakeDisplayList ( const LineSet & lines )
{
	//if ( disp_list != 0 )
	if (m_dispListLines != 0)
	{
		//glDeleteLists ( disp_list , 1 );
		glDeleteLists (m_dispListLines , 1);
	}

	visualatt.SetDraw ( true );
	//disp_list = glGenLists(1);
	m_dispListLines = glGenLists (1);
	//(*usercount)=1;

	//if ( disp_list < 1 )
	if (m_dispListLines < 1)
		throw (VException(( IDS_OpeGlDisLisCanN),0));

	//glNewList(disp_list, GL_COMPILE);
	glNewList (m_dispListLines , GL_COMPILE);

	glBegin ( GL_LINES );
	for ( long i = 0; i < lines.Size(); i++ )
	{
		glVertex3f ( static_cast <GLfloat> (lines[i]->GetP1().x())
					,static_cast <GLfloat> (lines[i]->GetP1().y())
					,static_cast <GLfloat> (lines[i]->GetP1().z()) );
		
		glVertex3f ( static_cast <GLfloat> (lines[i]->GetP2().x())
					,static_cast <GLfloat> (lines[i]->GetP2().y())
					,static_cast <GLfloat> (lines[i]->GetP2().z()) );
			
	}
	glEnd ();

	glEndList();
}


void Display::MakeDisplayList ( const SequentialLineSet & seqlines )
{
	//if ( disp_list != 0 )
	//	return;
	if (m_dispListLines != 0)
	{
		glDeleteLists (m_dispListLines , 1);
	}


	visualatt.SetDraw ( true );
	//disp_list = glGenLists(1);
	m_dispListLines = glGenLists (1);
	//(*usercount)=1;

	//if ( disp_list < 1 )
	if (m_dispListLines < 1)
		throw (VException(( IDS_OpeGlDisLisCanN),0));

	//glNewList(disp_list, GL_COMPILE);
	glNewList (m_dispListLines,GL_COMPILE);
	glBegin ( GL_LINE_STRIP);
	
	int psize = seqlines.GetPointSize();
	for ( int i = 0; i < psize; ++i )
	{
		const VPoint * p = seqlines.GetPoint(i);
		glVertex3f ( static_cast <GLfloat> (p->x())
					,static_cast <GLfloat> (p->y())
					,static_cast <GLfloat> (p->z()) );
	}
	glEnd ();

	glEndList();
}

//void LinesDisplay::MakeDisplayList ( const SurfaceWireFrame & wiredata )
//{
//	if ( disp_list != 0 )
//		return;
//
//	visualatt.SetDraw ( true );
//	disp_list = glGenLists(1);
//	(*usercount)=1;
//
//	if ( disp_list < 1 )
//		throw (VException(("open gl display list can not be generated"),0));
//
//	glNewList(disp_list, GL_COMPILE);
//	glBegin ( GL_LINES );
//	
//	for ( int i=0; i < wiredata.Size(); i++ )
//	{
//		const SequentialLineSet * seqlines = wiredata[i];
//		for ( long j = 0; j < seqlines->Size(); j++ )
//		{
//			VLine currline = (*seqlines)[j];
//			glVertex3f ( static_cast <GLfloat> (currline.GetP1().x())
//						,static_cast <GLfloat> (currline.GetP1().y())
//						,static_cast <GLfloat> (currline.GetP1().z()) );
//			
//			glVertex3f ( static_cast <GLfloat> (currline.GetP2().x())
//						,static_cast <GLfloat> (currline.GetP2().y())
//						,static_cast <GLfloat> (currline.GetP2().z()) );
//			
//		}
//	}
//	
//	glEnd ();
//
//	glEndList();
//
//}

LineVisualAtt & Display::GetVisualAtt ()
{
	return visualatt;
}

const LineVisualAtt & Display::GetVisualAtt ()const
{
	return visualatt;
}
/*
bool LinesDisplay::IsDefined() const
{
	if ( disp_list )
		return true;
	else
		return false;
}
*/
void Display::SetVisualAtt ( const LineVisualAtt &newvisatt )
{
	visualatt = newvisatt;
}

//Display & Display::operator = (const Display &tocopy)
//{
//	Display::operator =(tocopy);	
//	visualatt = tocopy.visualatt;
//	m_seqLine = tocopy.m_seqLine;
//	m_drwInt = tocopy.m_drwInt;
//	m_visualAttVec = tocopy.m_visualAttVec;
//	m_limitbi = tocopy.m_limitbi;
//	m_limitei = tocopy.m_limitei;
//	return (*this);
//}

void Display::OnlineDraw (bool isSelected) const
{
	//try
	//{
		size_t limitbi(m_limitbi) , limitei(m_limitei);
		int counter = 0;
		//if (m_isGraphicViewChanging)
		//{
		//	VpAfxMessageBox(("hi"));
		//	if ( (limitei - limitbi) > 100)
		//		limitei = limitbi + 100;
		//}
		if (m_seqLine)
		{
			/*VColor tmp (1.0,0.0,0.0);*/
			VisualAttLoader myloader;
		
			size_t mysize = m_drwInt.size();
			for (size_t i=0;i<mysize;++i)
			{
				std::pair<size_t,size_t> curi = m_drwInt.at(i);
				const LineVisualAtt &visatt = m_visualAttVec.at(i);
				myloader.Load(visatt,isSelected,VGlobalValues::viewpointdefaults.m_selectionColor);
				glFinish();
				size_t i1 = curi.first;
				size_t i2 = curi.second;
				if (i2 < limitbi || i1 > limitei)
					continue;
				//++i2;//i2 included
				glBegin ( GL_LINE_STRIP);
				for ( size_t j = i1; j < i2; j+=QUICK_STEP)//++j )
				{
					if (j < limitbi || j > limitei)
						continue;
					const VPoint & p = m_seqLine->operator[](j);
					glVertex3dv (p.GetArray()); 
					++counter;
					if (counter > m_maxOnLineItemDrawLimit)
						return;
						//( static_cast <GLfloat> (p.x())
						//, static_cast <GLfloat> (p.y())
						//, static_cast <GLfloat> (p.z()) );
				}
				glEnd ();
			}
			m_limitbi = limitbi;
			m_limitei = limitei;
		}
	//}
	//catch (VException &e)
	//{
		//LogFileWriter::WriteExceptionLog (e);
	//}
	//catch (std::exception &e)
	//{
		//LogFileWriter::WriteExceptionLog (e);
	//}
	//catch (...)
	//{
		//LogFileWriter::WriteUnknownExceptionLog (("LinesDisplay::MakeDisplayList unknown exception"));
	//}	

}

void Display::MakeDisplayList(const std::vector<VPoint> *sl, const std::vector<std::pair<size_t,size_t> > &drwInt , const std::vector<LineVisualAtt> &visualAttVec)
{
	m_seqLine = sl;
	m_drwInt = drwInt;
	m_visualAttVec = visualAttVec;
	if (m_visualAttVec.size() != m_drwInt.size())
		throw VException (( IDS_LinMakExcVisAtt),1);

	// check if ranges are valid
	size_t mysize = m_drwInt.size();
	for (size_t i=0;i<mysize;++i)
	{
		LineVisualAtt &curva = m_visualAttVec.at(i);
		//curva.SetLineWidth(LineVisualAtt::thin);
		std::pair<size_t,size_t> curi = m_drwInt.at(i);
		size_t i1 = curi.first;
		size_t i2 = curi.second;
		//++i2;//i2 included
		// check if range is ok
		if (i1>=m_seqLine->size() || i2 > m_seqLine->size())
			throw VException (( IDS_LinOnlExcSlIntW),1);
	}
	m_limitbi = 0;
	m_limitei = sl->size()-1;
}

size_t Display::GetLimitBi ()const
{
	return m_limitbi;
}
void Display::SetLimitBi (size_t limitbi)
{
	m_limitbi = limitbi;
}
size_t Display::GetLimitEi ()const
{
	return m_limitei;
}
void Display::SetLimitEi (size_t limitei)
{
	m_limitei = limitei;
}

void Display::RemoveLimitsForOnlineDraw ()
{
	if (m_seqLine == 0)
		return;
	m_limitbi = 0;
	m_limitei = m_seqLine->size() - 1;
}

// LinesDisplay.cpp

// FacetDisplay.cpp
//#include "stdafx.h"
#include "VRefTriangleMesh.h"
#include "TriangleMesh.h"
#include "PointSet.h"
#include "VPoint.h"
//#include "FacetDisplay.h"
#include "DoubleVec.h"
//#include "gl/glu.h"

#include "OglRefMesh.h"
#include "FMesh.h"

//Display::Display ()
//{
//	m_ftri=0;
//}

//Display::Display ( const Display & Foo )
////:Display(Foo)
//{
//	//Xmatrix = Foo.Xmatrix;
//	visualatt = Foo.visualatt;
//	m_ftri = Foo.m_ftri;
//}

//FacetDisplay & FacetDisplay::operator = ( const FacetDisplay & Foo )
//{
//	Display::operator =(Foo);
//	visualatt = Foo.visualatt;
//	m_ftri = Foo.m_ftri;
//	return (*this);	
//}

Display::Display ( const VRefTriangleMesh & tri )
//:Display()
{
	m_dispListLines = 0;
	m_dispListFacet = 0;
	m_dispListPoints = 0;
	m_seqLine = 0;
	m_ftri=0;
	MakeDisplayList ( tri );
}

Display::Display ( const TriangleMesh & tri )
//:Display()
{
	m_dispListLines = 0;
	m_dispListFacet = 0;
	m_dispListPoints = 0;
	m_seqLine = 0;
	m_ftri=0;
	MakeDisplayList ( tri );
}

//FacetDisplay::~FacetDisplay ()
//{
//
//}
/*
void FacetDisplay::Draw () const 
{
}
*/
void Display::GetHitPointsFacet ( PointSet & selpoints , DoubleVec & depthvalues )const 
{
	//if ( disp_list == 0 && m_ftri == 0)
	//	return;
	if ( m_dispListFacet == 0 && m_ftri == 0)
		return;

	vreal projMatrix[16];
	vreal modelMatrix[16];
	int viewPort[4];
	glGetDoublev ( GL_MODELVIEW_MATRIX , modelMatrix );
	glGetDoublev ( GL_PROJECTION_MATRIX , projMatrix );
	glGetIntegerv ( GL_VIEWPORT , viewPort );

	glFeedbackBuffer ( SELECTION_BUFFER_SIZE , GL_3D , selectionbuff );
	glRenderMode ( GL_FEEDBACK );
	
	// Draw all triangles and record the ones that are hit
	
	DrawFacet();		

	// get the selected points from record
	int count = glRenderMode ( GL_RENDER );
	int currcount = 0;

 	while ( currcount < count )
	{		
		GLfloat token = selectionbuff[currcount];
		currcount++;

		if ( token == GL_POLYGON_TOKEN )
		{
			currcount++; // for n
			for ( int j=0; j <3; j++ )
			{
				vreal xyz[3];
				xyz[0] = static_cast <vreal> (selectionbuff[currcount]);
				currcount++;
				xyz[1] = static_cast <vreal> (selectionbuff[currcount]);
				currcount++;
				xyz[2] = static_cast <vreal> (selectionbuff[currcount]);
				currcount++;
						
				vreal result[3];
				gluUnProject ( xyz[0] , xyz[1] , xyz[2] , modelMatrix , projMatrix , viewPort , 
								&(result[0]) , &(result[1]) , &(result[2]) );
				
				selpoints.InsertPoint  ( result );
				depthvalues.Addvreal ( xyz[2] );
			}	
		}
	}
	
}

bool Display::IsHitFacet ( vreal & depthvalue ) const
{
	//if ( disp_list == 0 && m_ftri == 0)
	//	return false;
	if (m_dispListFacet == 0 && m_ftri == 0)
		return false;

	bool result = false;
	vreal tempdepthvalue = 1.0 / EPSILON;

	unsigned int sel_ind[500];	
	glSelectBuffer ( 500 , sel_ind );
	glRenderMode ( GL_SELECT );
	glInitNames();
	glPushName(0);

	if (m_dispListFacet)
	{
		glCallList(m_dispListFacet);
	}
	else
	{
		if (m_ftri == 0)
		{

		}
		else
		{
			int myOnlineDrawSelectionLimit = 10;
			int asize = m_ftri->GetArraySize();
			if (asize > myOnlineDrawSelectionLimit)
			{
				VPoint mn = m_ftri->GetMin();
				VPoint mx = m_ftri->GetMax();
				VTriangleE t1(VPoint (mn),VPoint(mx.x(),mn.y(),mn.z()),VPoint(mx.x(),mx.y(),mx.z()));
				VTriangleE t2(VPoint(mx),VPoint(mn.x(),mx.y(),mx.z()),VPoint(mn));
				glBegin(GL_TRIANGLES);
					glNormal3d(t1.GetNormal().x(), t1.GetNormal().y(),t1.GetNormal().z());
					glVertex3d(t1.GetCorner1().x(), t1.GetCorner1().y(), t1.GetCorner1().z());
					glVertex3d(t1.GetCorner2().x(), t1.GetCorner2().y(), t1.GetCorner2().z());
					glVertex3d(t1.GetCorner3().x(), t1.GetCorner3().y(), t1.GetCorner3().z());
					glNormal3d(t2.GetNormal().x(), t2.GetNormal().y(), t2.GetNormal().z());
					glVertex3d(t2.GetCorner1().x(), t2.GetCorner1().y(), t2.GetCorner1().z());
					glVertex3d(t2.GetCorner2().x(), t2.GetCorner2().y(), t2.GetCorner2().z());
					glVertex3d(t2.GetCorner3().x(), t2.GetCorner3().y(), t2.GetCorner3().z());
				glEnd();
			}
			else
			{
				OnlineDrawFacet();
			}
		}
	}

	// get the selected points from record
	int hits = glRenderMode ( GL_RENDER );
	int offset = 0;

	for ( int i = 0; i < hits; i++ )
	{
		result = true;
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
	
	if ( result == true )
		depthvalue = tempdepthvalue;

	return result;
}

void Display::MakeDisplayList ( const VRefTriangleMesh & tri )
{
	//if ( disp_list != 0 )
	//	return;
	if (m_dispListFacet != 0)
	{
		DestroyDisplayListFacet ();
	}

	visualatt.SetDraw (true);
	//disp_list = glGenLists(1);
	m_dispListFacet = glGenLists (1);
	//(*usercount) = 1;

	//if ( disp_list < 1 )
	if (m_dispListFacet < 1)
		throw (VException(( IDS_OpeGlDisLisCanN),0));
	
	int mysize = tri.GetRefTriangleMesh().Size();
	//glNewList(disp_list, GL_COMPILE);
	glNewList (m_dispListFacet , GL_COMPILE);
	glBegin ( GL_TRIANGLES );
	for ( long i = 0; i <mysize; ++i )
	{
		glNormal3f ( static_cast <GLfloat> (tri.GetCor1Normal(i).x())
					,static_cast <GLfloat> (tri.GetCor1Normal(i).y())
					,static_cast <GLfloat> (tri.GetCor1Normal(i).z()) );

		glVertex3f ( static_cast <GLfloat> (tri.GetModelTriCor1(i).x())
					, static_cast <GLfloat> (tri.GetModelTriCor1(i).y())
					, static_cast <GLfloat> (tri.GetModelTriCor1(i).z()) );

		glNormal3f ( static_cast <GLfloat> (tri.GetCor2Normal(i).x())
					,static_cast <GLfloat> (tri.GetCor2Normal(i).y())
					,static_cast <GLfloat> (tri.GetCor2Normal(i).z()) );

		glVertex3f ( static_cast <GLfloat> (tri.GetModelTriCor2(i).x())
					, static_cast <GLfloat> (tri.GetModelTriCor2(i).y())
					, static_cast <GLfloat> (tri.GetModelTriCor2(i).z()) );
	
		glNormal3f ( static_cast <GLfloat> (tri.GetCor3Normal(i).x())
					,static_cast <GLfloat> (tri.GetCor3Normal(i).y())
					,static_cast <GLfloat> (tri.GetCor3Normal(i).z()) );

		glVertex3f ( static_cast <GLfloat> (tri.GetModelTriCor3(i).x())
					, static_cast <GLfloat> (tri.GetModelTriCor3(i).y())
					, static_cast <GLfloat> (tri.GetModelTriCor3(i).z()) );			
	}
	glEnd ();

	glEndList();
}

void Display::MakeDisplayList ( const TriangleMesh & tri )
{
	//if ( disp_list != 0 )
	//	return;
	if (m_dispListFacet)
	{
		//	return;
		DestroyDisplayListFacet();
	}
	visualatt.SetDraw ( true );
	//disp_list = glGenLists(1);
	m_dispListFacet = glGenLists(1);
	//(*usercount) = 1;

	//if ( disp_list < 1 )
	if (m_dispListFacet < 1)
 		throw (VException(( IDS_OpeGlDisLisCanN),0));

	//glNewList(disp_list, GL_COMPILE);
	glNewList (m_dispListFacet , GL_COMPILE);
	glBegin ( GL_TRIANGLES );
	for ( long i = 0; i < tri.Size(); i++ )
	{
		const VPoint & normal = tri[i]->GetNormal();
		const VPoint & c1 = tri[i]->GetCorner1();
		const VPoint & c2 = tri[i]->GetCorner2();
		const VPoint & c3 = tri[i]->GetCorner3();

		glNormal3f ( static_cast <GLfloat> (normal.x())
					,static_cast <GLfloat> (normal.y())
					,static_cast <GLfloat> (normal.z()) );

		glVertex3f (  static_cast <GLfloat> (c1.x())
					, static_cast <GLfloat> (c1.y())
					, static_cast <GLfloat> (c1.z()) );

		glVertex3f (  static_cast <GLfloat> (c2.x())
					, static_cast <GLfloat> (c2.y())
					, static_cast <GLfloat> (c2.z()) );
		
		glVertex3f (  static_cast <GLfloat> (c3.x())
					, static_cast <GLfloat> (c3.y())
					, static_cast <GLfloat> (c3.z()) );
	
	}
	glEnd ();

	glEndList();
}

//void FacetDisplay::MakeDisplayList ( const FMesh & tri )
//{
//	if ( disp_list != 0 )
//		return;
//
//	visualatt.SetDraw ( true );
//	disp_list = glGenLists(1);
//	//(*usercount) = 1;
//
//	if ( disp_list < 1 )
//		throw (VException(("open gl display list can not be generated"),0));
//
//	glNewList(disp_list, GL_COMPILE);
//
//	glBegin ( GL_TRIANGLES );
//		long mysize = tri.Size();
//		for ( long i = 0; i < mysize; i++ )
//		{
//			VTriangleE curtri = tri.GetVTri(i);
//			const VPoint & normal = curtri.GetNormal();
//
//			glNormal3f ( static_cast <GLfloat> (normal.x())
//				,static_cast <GLfloat> (normal.y())
//				,static_cast <GLfloat> (normal.z()) );
//
//			glVertex3f ( (float)curtri.GetCorner1().x() , (float)curtri.GetCorner1().y() , (float)curtri.GetCorner1().z() );
//
//			glVertex3f ( (float)curtri.GetCorner2().x() , (float)curtri.GetCorner2().y() , (float)curtri.GetCorner2().z() );
//
//			glVertex3f ( (float)curtri.GetCorner3().x() , (float)curtri.GetCorner3().y() , (float)curtri.GetCorner3().z() );
//		}
//	glEnd ();
//
//	glEndList();
//}

void Display::MakeDisplayList ( const FMesh &tri )
{
	m_ftri = &tri;
	visualatt.SetDraw ( true );
	int mysize = (int)m_ftri->GetArraySize();
	
	if ((mysize % 12) != 0)
		throw VException (( IDS_FacMakDisLisExc),1);
}

void Display::MakeDisplayListByVertexArray ( const VRefTriangleMesh & tri )
{
	//if ( disp_list != 0 )
	//	return;
	if (m_dispListFacet != 0)
		DestroyDisplayListFacet ();


	visualatt.SetDraw (true);

/*	
	int refarraysize,vertexarraysize;
	int * references;
	GLfloat * normals , * modelpoints;


	refarraysize = tri.Size() * 3;
	references = new int [ refarraysize ];

	for ( long i= 0; i < tri.Size(); i++ )
	{
		RefTriangle * currreftri = tri.rdata[i];

		int istart = i * 3;
		
		if ( !IsReversed ( i , tri ) )
		{
			references [istart] = currreftri->ref0;
			references [istart+1] = currreftri->ref1;
			references [istart+2] = currreftri->ref2;
		}
		else
		{
			references [istart] = currreftri->ref2;
			references [istart+1] = currreftri->ref1;
			references [istart+2] = currreftri->ref0;
		}
	
	}

	vertexarraysize = tri.surfpack.modelpoints.Size() * 3;
	modelpoints = new GLfloat [vertexarraysize];
	normals = new GLfloat [vertexarraysize];
	
	for ( i = 0; i < tri.surfpack.modelpoints.Size(); i++ )
	{
		VPoint * currp = tri.surfpack.modelpoints[i];
		VPoint * currn = tri.surfpack.normals[i];
	
		int istart = i * 3;

		modelpoints[istart] = static_cast<GLfloat> (currp->x());
		modelpoints[istart+1] = static_cast<GLfloat> (currp->y());
		modelpoints[istart+2] = static_cast<GLfloat> (currp->z());
		normals[istart] = static_cast<GLfloat> (currn->x());
		normals[istart+1] = static_cast<GLfloat> (currn->y());
		normals[istart+2] = static_cast<GLfloat> (currn->z());
	}
*/	

	OglRefMesh * oglrefmesh = new OglRefMesh;
	oglrefmesh->FormMesh ( tri );
	MakeDisplayListByVertexArray ( *oglrefmesh );
	glFinish();
	// We should delete the oglrefmesh but opengl is parallel processing this may cause problem!	
	 delete oglrefmesh;

	
/*
	disp_list = glGenLists(1);
	(*usercount) = 1;

	if ( disp_list < 1 )
		throw (VException(( IDS_OpeGlDisLisCanN),0));

	glNewList(disp_list, GL_COMPILE);
	
	glEnableClientState ( GL_NORMAL_ARRAY );
	glEnableClientState ( GL_VERTEX_ARRAY );
		
	glNormalPointer ( GL_GLfloat , 0 , oglrefmesh->normals );
	glVertexPointer ( 3 , GL_GLfloat , 0 , oglrefmesh->modelpoints );	
	//glPolygonMode ( GL_FRONT_AND_BACK , GL_LINE );
	glDrawElements ( GL_TRIANGLES , oglrefmesh->refarraysize , GL_UNSIGNED_INT , oglrefmesh->references );
	//glPolygonMode ( GL_FRONT_AND_BACK , GL_FILL );
	glEndList ();
*/
	return;

	//delete [] references;
	//delete [] modelpoints;
	//delete [] normals;

}

void Display::MakeDisplayListByVertexArray ( OglRefMesh & oglrefmesh )
{
	//disp_list = glGenLists(1);
	m_dispListFacet = glGenLists (1);
	//(*usercount) = 1;

	//if ( disp_list < 1 )
	if (m_dispListFacet < 1)
		throw (VException(( IDS_OpeGlDisLisCanN),0));
	
	//glNewList(disp_list, GL_COMPILE);
	glNewList (m_dispListFacet , GL_COMPILE);
	glEnableClientState ( GL_NORMAL_ARRAY );
	glEnableClientState ( GL_VERTEX_ARRAY );
		
	glNormalPointer ( GL_FLOAT , 0 , oglrefmesh.normals );
	glVertexPointer ( 3 , GL_FLOAT , 0 , oglrefmesh.modelpoints );	
	//glPolygonMode ( GL_FRONT_AND_BACK , GL_LINE );
	glDrawElements ( GL_TRIANGLES , oglrefmesh.refarraysize , GL_UNSIGNED_INT , oglrefmesh.references );
	//glPolygonMode ( GL_FRONT_AND_BACK , GL_FILL );
	glEndList ();
}

FacetVisualAtt & Display::GetVisualAttFacet()
{
	return visualattFacet;
}

const FacetVisualAtt & Display::GetVisualAttFacet() const
{
	return visualattFacet;
}

bool Display::IsReversed ( int index , const VRefTriangleMesh & trimesh )
{
	VPoint c1 = trimesh.GetModelTriCor1 ( index );
	VPoint c2 = trimesh.GetModelTriCor2 ( index );
	VPoint c3 = trimesh.GetModelTriCor3 ( index );
	VPoint n = trimesh.GetCor1Normal ( index );

	VPoint e1 = c2 - c1;
	VPoint e2 = c3 - c2;

	VPoint crossp = e1 ^ e2;
	vreal dotp = n % crossp;
	
	if ( dotp >= 0 )
		return false;
	else
		return true;
	
}

bool Display::IsDefinedFacet() const
{
	//if ( disp_list )
	if (m_dispListFacet)	
		return true;
	else
		return false;
}

void Display::SetVisualAttFacet ( const FacetVisualAtt & visatt )
{
	visualattFacet = visatt;
}

void Display::OnlineDrawFacet ()const
{
	if (m_ftri == 0)
	{

	}
	else
	{
		int i = 0;
		int asize = m_ftri->GetArraySize();
		if ((asize % 12) != 0)
			throw VException (( IDS_FacOnlExcArrSiz),1);
		const float *a = m_ftri->GetArray();
		glBegin ( GL_TRIANGLES );
	
			while (i<asize)
			{
				glNormal3f (a[i],a[i+1],a[i+2]);

				glVertex3f (a[i+3],a[i+4],a[i+5]);

				glVertex3f (a[i+6],a[i+7],a[i+8]);

				glVertex3f (a[i+9],a[i+10],a[i+11]);
			
				i+=12;
			}
		glEnd ();
	}
}

void Display::MakeDisplayList (std::vector<FMesh *> &triVec )
{
	visualatt.SetDraw ( true );
	if (m_dispListFacet !=0)
		DestroyDisplayListFacet ();
	//disp_list = glGenLists(1);
	m_dispListFacet = glGenLists (1);
	//(*usercount) = 1;

	//if ( disp_list < 1 )
	if (m_dispListFacet < 1)
		throw (VException(( IDS_OpeGlDisLisCanN),0));
	
	//glNewList(disp_list, GL_COMPILE);
	glNewList (m_dispListFacet , GL_COMPILE);
	glBegin ( GL_TRIANGLES );
	size_t mysize = triVec.size();
	for (size_t k=0;k<mysize;++k)
	{
	
		FMesh *ftri = triVec.at(k);
		int i = 0;
		int asize = ftri->GetArraySize();
		if ((asize % 12) != 0)
			throw VException (( IDS_FacOnlExcArrSiz),1);
		const float *a = ftri->GetArray();
	
		while (i<asize)
		{
			glNormal3f (a[i],a[i+1],a[i+2]);

			glVertex3f (a[i+3],a[i+4],a[i+5]);

			glVertex3f (a[i+6],a[i+7],a[i+8]);

			glVertex3f (a[i+9],a[i+10],a[i+11]);
			
			i+=12;
		}
	}
	glEnd ();

	glEndList ();

}
//void Display::MakeTextureDisplayList(const VRefTriangleMesh *p_tri , const texture *p_texture)
//{
//	visualatt.SetDraw ( true );
//	if (m_dispListFacet != 0)
//		DestroyDisplayListFacet ();
//	//disp_list = glGenLists(1);
//	m_dispListFacet = glGenLists (1);
//	//(*usercount) = 1;
//
//	//if ( disp_list < 1 )
//	if (m_dispListFacet < 1)
//		throw (VException(( IDS_OpeGlDisLisCanN),0));
//	
//	//glNewList(disp_list, GL_COMPILE);
//	glNewList (m_dispListFacet , GL_COMPILE);
//	p_tri->DrawWithTexture (p_texture);
//
//	glEndList ();
//}
void Display::MakeTextureDisplayList(const VRefTriangleMesh *p_tri , const VTexture *p_texture)
{
	visualatt.SetDraw ( true );
	if (m_dispListFacet != 0)
		DestroyDisplayListFacet ();
	//disp_list = glGenLists(1);
	m_dispListFacet = glGenLists (1);
	//(*usercount) = 1;

	//if ( disp_list < 1 )
	if (m_dispListFacet < 1)
		throw (VException(( IDS_OpeGlDisLisCanN),0));
	
	//glNewList(disp_list, GL_COMPILE);
	glNewList (m_dispListFacet , GL_COMPILE);
	p_tri->DrawWithTexture (p_texture);

	glEndList ();
}
// PointsDisplay.cpp
//#include "stdafx.h"
#include "PointSet.h"
#include "VPoint.h"
//#include "PointsDisplay.h"
#include "DoubleVec.h"
//#include "gl/glu.h"
#include "PointSet.h"

//PointsDisplay::PointsDisplay()
//:Display()
//{
//	//Xmatrix = 0;
//}

//PointsDisplay::PointsDisplay (const PointsDisplay & Foo)
//:Display(Foo)
//{
//	//Xmatrix = Foo.Xmatrix;
//	visualatt = Foo.visualatt;
//}

Display::Display ( const PointSet & points )
//:Display()
{
	m_dispListLines = 0;
	m_dispListFacet = 0;
	m_dispListPoints = 0;
	m_seqLine = 0;
	m_ftri=0;
	MakeDisplayList ( points );
}

//PointsDisplay & PointsDisplay::operator=(const PointsDisplay & Foo)
//{
//	Display::operator =(Foo);
//	visualatt=Foo.visualatt;
//	return (*this);
//}

//PointsDisplay::~PointsDisplay ()
//{
//}
/*
void PointsDisplay::Draw () const 
{
	if ( disp_list )
	{
		if ( Xmatrix )
		{
			glMatrixMode(GL_MODELVIEW);
		
			glPushMatrix ();
		
			glMultMatrixd(Xmatrix); 
		}
	
			glCallList ( disp_list );

		if ( Xmatrix )
			glPopMatrix();
	
	}
}
*/
void Display::GetHitPointsPoints ( PointSet & selpoints , DoubleVec & depthvalues ,TransformData *dynamicTr) const
{
	//if ( disp_list == 0 )
	//	return;
	if (m_dispListPoints == 0)
		return;

	vreal projMatrix[16];
	vreal modelMatrix[16];
	int viewPort[4];
	glGetDoublev ( GL_MODELVIEW_MATRIX , modelMatrix );
	glGetDoublev ( GL_PROJECTION_MATRIX , projMatrix );
	glGetIntegerv ( GL_VIEWPORT , viewPort );

	glFeedbackBuffer ( SELECTION_BUFFER_SIZE , GL_3D , selectionbuff );
	glRenderMode ( GL_FEEDBACK );
	
	// Draw all triangles and record the ones that are hit
	
	DrawPoints();		

	// get the selected points from record
	int count = glRenderMode ( GL_RENDER );
	int currcount = 0;

 	while ( currcount < count )
	{		
		GLfloat token = selectionbuff[currcount];
		currcount++;

		if ( token == GL_POINT_TOKEN )
		{
			vreal xyz[3];
			xyz[0] = static_cast <vreal> (selectionbuff[currcount]);
			currcount++;
			xyz[1] = static_cast <vreal> (selectionbuff[currcount]);
			currcount++;
			xyz[2] = static_cast <vreal> (selectionbuff[currcount]);
			currcount++;
					
			vreal result[3];
			gluUnProject ( xyz[0] , xyz[1] , xyz[2] , modelMatrix , projMatrix , viewPort , 
							&(result[0]) , &(result[1]) , &(result[2]) );
			
			selpoints.InsertPoint  ( result );
			depthvalues.Addvreal ( xyz[2] );
		
		}
	}
	
}

bool Display::IsHitPoints ( vreal & depthvalue ) const
{
	//if ( disp_list == 0 )
	//	return false;
	if (m_dispListPoints == 0)
		return false;

	bool result = false;
	vreal tempdepthvalue = MAX_NUMBER_LIMIT;

	unsigned int sel_ind[500];	
	glSelectBuffer ( 500 , sel_ind );
	glRenderMode ( GL_SELECT );
	glInitNames();
	glPushName(0);

	DrawPoints();

	// get the selected points from record
	int hits = glRenderMode ( GL_RENDER );
	int offset = 0;

	for ( int i = 0; i < hits; i++ )
	{
		result = true;
		int nameno = sel_ind[offset++];
		
		vreal z1 =(static_cast<vreal> (sel_ind[offset++]))/OGL_SEL_ZCONST;
		vreal z2 =(static_cast<vreal> (sel_ind[offset++]))/OGL_SEL_ZCONST;
		
		if ( z1 < tempdepthvalue )
		{
			tempdepthvalue = z1;
		}
		if ( z2 < tempdepthvalue )
		{
			tempdepthvalue = z2;
		}

		for ( int j = 0; j < nameno; j++  )
		{
			int name = sel_ind[offset++];
		}
	}
	if ( result == true )
		depthvalue = tempdepthvalue;

	return result;
}

void Display::MakeDisplayList ( const PointSet &points )
{
	//if ( disp_list !=0 )
	//	return;
	if (m_dispListPoints != 0)
		DestroyDisplayListPoints ();

	visualatt.SetDraw ( true );
	//disp_list = glGenLists(1);
	m_dispListPoints = glGenLists (1);
	//(*usercount) = 1;

	//if ( disp_list < 1 )
	if (m_dispListPoints < 1)
		throw (VException(( IDS_OpeGlDisLisCanN),0));

	//glNewList(disp_list, GL_COMPILE);
	glNewList (m_dispListPoints , GL_COMPILE);
	glBegin ( GL_POINTS );
	for ( long i = 0; i < points.Size(); i++ )
	{
		glVertex3f ( static_cast <GLfloat> (points[i]->x())
					,static_cast <GLfloat> (points[i]->y())
					,static_cast <GLfloat> (points[i]->z()) );
	}
	glEnd ();

	glEndList();
}

PointVisualAtt & Display::GetVisualAttPoints ()
{
	return visualattPoints;
}

const PointVisualAtt & Display::GetVisualAttPoints () const
{
	return visualattPoints;
}
/*
bool PointsDisplay::IsDefined() const
{
	if ( disp_list )
		return true;
	else
		return false;
}
*/
void Display::SetVisualAttPoints ( const PointVisualAtt &newvisatt )
{
	visualattPoints = newvisatt;
}	

void Display::OnlineDrawPoints () const
{

}

void Display::MakeDisplayList (std::vector<const Display *> &facetDisplays 
			, std::vector<const Display *> &lineDisplays 
			, std::vector<const Display *> &pointDisplays
			, std::vector<OglMatrix> &curveTmVec)
{

	if (pointDisplays.size() != 0)
	{
		if (m_dispListPoints != 0)
			DestroyDisplayListPoints ();

		visualatt.SetDraw ( true );
		//disp_list = glGenLists(1);
		m_dispListPoints = glGenLists (1);

		if (m_dispListPoints < 1)
			throw (VException(( IDS_OpeGlDisLisCanN),0));

		glNewList (m_dispListPoints , GL_COMPILE);
		
		for (size_t i = 0; i < pointDisplays.size(); i++)
		{
			pointDisplays[i]->DrawPoints();
		}
		//glEnd ();

		glEndList();
	}

	if (lineDisplays.size() != 0)
	{
		if (m_dispListLines!= 0)
			DestroyDisplayListLines ();

		visualatt.SetDraw ( true );
		//disp_list = glGenLists(1);
		m_dispListLines = glGenLists (1);

		if (m_dispListLines < 1)
			throw (VException(( IDS_OpeGlDisLisCanN),0));
		
		glNewList (m_dispListLines , GL_COMPILE);
		glMatrixMode(GL_MODELVIEW);
		if (curveTmVec.size() != lineDisplays.size())
			throw VException (( IDS_DisMakByOthDisE),1);
		for (size_t i = 0; i < lineDisplays.size(); i++)
		{
			glPushMatrix();
			glMultMatrixd(curveTmVec.at(i).GetArray()); 	
			lineDisplays[i]->DrawLines(false);
			glPopMatrix();
		}
		//glEnd ();

		glEndList();
	}
	if (facetDisplays.size() != 0)
	{
		if (m_dispListFacet != 0)
			DestroyDisplayListFacet ();

		visualatt.SetDraw ( true );
		//disp_list = glGenLists(1);
		m_dispListFacet = glGenLists (1);

		if (m_dispListFacet < 1)
			throw (VException(( IDS_OpeGlDisLisCanN),0));

		glNewList (m_dispListFacet , GL_COMPILE);
		
		for (size_t i = 0; i < facetDisplays.size(); i++)
		{
			facetDisplays[i]->DrawFacet();
		}
		//glEnd ();

		glEndList();
	}
}

const std::vector<VPoint> * Display::GetSeqLine()const
{
	return m_seqLine;
}

unsigned int Display::GetDispListIdLines()const
{
	return m_dispListLines;
}
void Display::SetDispListIdLines (unsigned int dispListIdLines)
{
	m_dispListLines = dispListIdLines;
}
unsigned int Display::GetDispListIdFacet ()const
{
	return m_dispListFacet;
}
void Display::SetDispListIdFacet (unsigned int dispListIdFacet)
{
	m_dispListFacet = dispListIdFacet;
}
unsigned int Display::GetDispListIdPoints()const
{
	return m_dispListPoints;
}
void Display::SetDispListIdPoints (unsigned int dispListIdPoints)
{
	m_dispListPoints = dispListIdPoints;
}

void Display::MakeQuadDisplay (std::vector<QuadPointGenerator *> &quadPoints)
{
	visualatt.SetDraw ( true );
	if (m_dispListFacet !=0)
		DestroyDisplayListFacet ();
	//disp_list = glGenLists(1);
	m_dispListFacet = glGenLists (1);
	//(*usercount) = 1;

	//if ( disp_list < 1 )
	if (m_dispListFacet < 1)
		throw (VException(( IDS_OpeGlDisLisCanN),0));
	
	//glNewList(disp_list, GL_COMPILE);
	glNewList (m_dispListFacet , GL_COMPILE);
	size_t mysize = quadPoints.size();
	glBegin ( GL_QUADS );
	for (size_t k=0;k<mysize;++k)
	{
		QuadPointGenerator *curgen = quadPoints.at(k);
		int qsize = curgen->QuadSize();

		VPoint c[25] , n [25];

		for (int i=0;i<qsize;++i)
		{
			int csize = curgen->GetBoxCornersAndNormals(i,c,n);
			for (int j=0;j<csize;j+=4)
			{
				glNormal3dv (n[j].GetArray());

				glVertex3dv (c[j].GetArray());

				glVertex3dv (c[j+1].GetArray());

				glVertex3dv (c[j+2].GetArray());

				glVertex3dv (c[j+3].GetArray());
			}
		}
	}
	glEnd ();
	glEndList ();
}

std::vector<Display *> & Display::GetAttachedDisplay()
{
	return m_attachedDisplay;
}

std::vector<Display *> & Display::GetAttachedDisplayFacet()
{
	return m_attachedDisplayFacet;
}

std::vector<Display *> & Display::GetAttachedDisplayPoints()
{
	return m_attachedDisplayPoints;
}

void Display::DrawAttachedDisplay(bool isSelected)const
{
	int mysize = (int)m_attachedDisplay.size();
	for (int di = 0; di < mysize; ++di)
	{
		Display *curdisp = m_attachedDisplay.at(di);
		curdisp->DrawLines(isSelected);
		//curdisp->DrawFacet();
		//curdisp->DrawPoints();
	}
}

void Display::DrawAttachedDisplayFacet()const
{
	int mysize = (int)m_attachedDisplayFacet.size();
	for (int di = 0; di < mysize; ++di)
	{
		m_attachedDisplayFacet.at(di)->DrawFacet();
	}
}

void Display::DrawAttachedDisplayPoints()const
{
	int mysize = (int)m_attachedDisplayPoints.size();
	for (int di = 0; di < mysize; ++di)
	{
		m_attachedDisplayPoints.at(di)->DrawPoints();
	}
}