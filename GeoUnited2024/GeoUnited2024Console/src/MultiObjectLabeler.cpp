// MultiObjectLabeler.cpp

#include "stdafx.h"
#include "MultiObjectLabeler.h"
#include "TextBox.h"
#include "PointSet.h"

#define X 0
#define Y 1
#define Z 2
#define MTOL 1.0e-10
const int SHIFT =  10;

MultiObjectLabeler::MultiObjectLabeler ( const vreal &xwidth , const vreal &ywidth
										, const OglWindowData &wnddata , std::vector<Display *> *asciiDisp)
										:mwnddata(wnddata), m_asciiDisp(asciiDisp)
{
	mxwidth = xwidth;
	mywidth = ywidth;
	mxwidthE = mxwidth*1.2;
	mywidthE = mywidth*1.8;
	cellno = 0;
	cells = 0;
	CalcCellNo();
	MakeCells();
	FillCells();
	oglwindowupdated = false;
	m_pointsInView = 0;
	m_pointsInViewSize = 0;
}

//#############################################################################

MultiObjectLabeler::~MultiObjectLabeler ()
{
	if ( cellno > 0 )
		DeleteCells();

	if ( m_pointsInViewSize > 0 )
		delete [] m_pointsInView; 
}

//#############################################################################

void MultiObjectLabeler::MakeCells ()	
{
	cells = new GLfloat * [cellno];
	lineend = new GLfloat * [cellno];

	for ( int w = 0; w < cellno; w++ )
	{
		cells[w] = new GLfloat [3];
		cells[w][X] = 0.0;
		cells[w][Y] = 0.0;
		cells[w][Z] = 0.0;

		lineend[w] = new GLfloat [2];
		lineend[w][X] = 0.0;
		lineend[w][Y] = 0.0;
	}

}

//#############################################################################

void MultiObjectLabeler::CalcCellNo ()
{
	xcellno = (int)( mwnddata.mcornerx / mxwidthE );
	ycellno = (int)((mwnddata.mcornery - (mywidthE*2.1) ) / mywidthE );

	if ( ycellno < 0 )
		ycellno = 0;
	if ( xcellno < 0 )
		xcellno = 0;
	if ( ycellno > 90  )
		ycellno = 90;
	if ( xcellno > 90 )
		xcellno = 90;

	cellno = 2 * xcellno + 2 * ycellno;

}

//#############################################################################

void MultiObjectLabeler::FillCells ()
{
	//  A Cell
//			  
//           ----- 
//			|	  |lineend ( if left aligned )
//			|	  |----------
//			|_____|
//				   mybase

	int i;
	for ( i=0; i<xcellno; i++ )
	{
		// aligned top
		cells[i][X] = static_cast<GLfloat>((i+1)*mxwidthE);
		cells[i][Y] = static_cast<GLfloat>(mwnddata.mcornery - (SHIFT + mywidthE));
		cells[i][Z] = 0.0f;

		lineend[i][X] = static_cast<GLfloat>(cells[i][X] - mxwidthE / 2.0);
		lineend[i][Y] = static_cast<GLfloat>(cells[i][Y]);
	}
	
	for (  i=0; i<xcellno; i++ )
	{
		// aligned down
		cells[i+xcellno][X] = static_cast<GLfloat> ( (i+1)*mxwidthE );
		cells[i+xcellno][Y] = static_cast<GLfloat> (SHIFT);
		cells[i+xcellno][Z] = 0.0f;
		
		lineend[i+xcellno][X] = static_cast<GLfloat> (cells[i+xcellno][X] - mxwidthE / 2.0);
		lineend[i+xcellno][Y] = static_cast<GLfloat> (cells[i+xcellno][Y] + mywidth);	
	}

	for ( i=0; i<ycellno; i++ )
	{
		// aligned left
		cells[i+2*xcellno][X] = static_cast<GLfloat> (mxwidthE);
		cells[i+2*xcellno][Y] = static_cast<GLfloat> ((i+1)*mywidthE+SHIFT);
		cells[i+2*xcellno][Z] = 0.0f;

		lineend[i+2*xcellno][X] = static_cast<GLfloat> (mxwidthE);
		lineend[i+2*xcellno][Y] = static_cast<GLfloat> (cells[i+2*xcellno][Y] + mywidthE / 2.0);	
	}

	for ( i=0; i<ycellno; i++ )
	{
		// aligned right
		cells[i+2*xcellno+ycellno][X] = static_cast<GLfloat> ( mwnddata.mcornerx - SHIFT );
		cells[i+2*xcellno+ycellno][Y] = static_cast<GLfloat> ( (i+1)*mywidthE + SHIFT );
		cells[i+2*xcellno+ycellno][Z] = 0.0f;
	
		lineend[i+2*xcellno+ycellno][X] = static_cast<GLfloat> ( cells[i+2*xcellno+ycellno][X] - mxwidth );
		lineend[i+2*xcellno+ycellno][Y] = static_cast<GLfloat> ( cells[i+2*xcellno+ycellno][Y] + mywidthE / 2.0 );		
	}
	//cells created.
}

//#############################################################################

void MultiObjectLabeler::DeleteCells ()
{
	for ( int w = 0; w < cellno; w++ )
	{
		delete [] cells[w];
		delete [] lineend[w];
	}
}

//#############################################################################

int MultiObjectLabeler::FindNearestCellToPoint ( const GLfloat screenpnt[2] )
{
	//Go through all cells and find the nearest of all
	vreal min = 1/MTOL;
	vreal dx , dy;
	int set=-1;

	for ( int j=0; j<cellno; j++ )
	{
		if ( cells[j][Z] == 0.0 )// means cell is not selected untill now
		{
			dx = cells[j][X] - screenpnt[0];
			dy = cells[j][Y] - screenpnt[1];
			
			if ( min > fabs(dx) + fabs(dy) )
			{
				min = fabs(dx) + fabs(dy);
				set = j;
			}
		}
	}
	
	if ( set != -1 )
		cells[set][Z] = 1.0; // means cell selected

	return set;
}

//#############################################################################

int MultiObjectLabeler::FindNearestPointToCell ( const vreal &cellX , const vreal &cellY , vreal pointsInView[] , int pointsInViewNo )
{
	vreal mymin = MAX_NUMBER_LIMIT;
	int indexMin = -1;
	for ( int i=0; i < pointsInViewNo; i++ )
	{
		int startIndex = i*3;
		vreal isUsed = pointsInView[startIndex+2];
		
		if ( isUsed == 1 )
			continue;

		vreal currCoord[2];		
		currCoord[0] = pointsInView[startIndex];
		currCoord[1] = pointsInView[startIndex+1];
		vreal dx = cellX - currCoord[0];
		vreal dy = cellY - currCoord[1];
		vreal aproxDist = fabs(dx) + fabs(dy);
		if ( mymin >  aproxDist )
		{
			mymin = aproxDist;
			indexMin = i;
		}
	}
	if ( indexMin != -1 )
	{
		int startindex = indexMin*3;
		pointsInView[startindex+2] = 1;//set isused	
	}

	return indexMin;
}

//#############################################################################
/*
void MultiObjectLabeler::LabelCMMPoints ( const vreal pointsarray[] , const int &pointno 
							    , const CString &labeltxt , const GLfloat showcolor[3] )
{
	//InitCellsSelected();
	if ( oglwindowupdated == true )
		UpdateCells();

	for ( int i=0; i < pointno; i++ )
	{
		int inds = i*3; 
		GLfloat modelpoint [4] 
			= { static_cast <GLfloat> (pointsarray[inds]) 
			  , static_cast <GLfloat> (pointsarray[inds+1]) 
			  , static_cast <GLfloat> (pointsarray[inds+2]) , 0.0f };

		GLfloat screenpoint[4];
		bool ck = mwnddata.GetScreenCoordOfPoint ( modelpoint , screenpoint );
		
		if ( ck == false )
			continue; // point is out of view ( screen )
		
		CString showtext;
		showtext.Format ( labeltxt + ( IDS_D5),(i+1) );

		TextBox txtbox ( mxwidth , mywidth , showtext , showcolor );
		
		// Find nearest unselected cell
		int cellindex = this->FindNearestCellToPoint ( screenpoint );
		
		if ( cellindex == -1 )
			continue;

		TextBoxPosData posdata( cells[cellindex] , screenpoint , lineend[cellindex] );

		TextBoxDisplayer disptxt ( mwnddata );		
		disptxt.Display ( txtbox , posdata );
	}

}
*/
//#############################################################################
/*
void MultiObjectLabeler::LabelCMMPoints ( const PointSet & pointsarray 
										 , const std::vector<CString> & alltxt , const GLfloat showcolor[3] )
{
	//InitCellsSelected();
	if ( oglwindowupdated == true )
		UpdateCells();

	for ( int i=0; i < pointsarray.Size(); i++ )
	{
		VPoint *currpoint = pointsarray[i];
		GLfloat modelpoint [4] 
			= { static_cast <GLfloat> (currpoint->x()) 
			  , static_cast <GLfloat> (currpoint->y()) 
			  , static_cast <GLfloat> (currpoint->z()) , 0.0f };

		GLfloat screenpoint[4];
		bool ck = mwnddata.GetScreenCoordOfPoint ( modelpoint , screenpoint );
		
		if ( ck == false )
			continue; // point is out of view ( screen )
		
		CString showtext = alltxt[i];
		//showtext.Format ( labeltxt + (" %d"),(i+1) );

		TextBox txtbox ( mxwidth , mywidth , showtext , showcolor );
		
		// Find nearest unselected cell
		int cellindex = this->FindNearestCellToPoint ( screenpoint );
		
		if ( cellindex == -1 )
			continue;

		TextBoxPosData posdata( cells[cellindex] , screenpoint , lineend[cellindex] );

		TextBoxDisplayer disptxt ( mwnddata );		
		disptxt.Display ( txtbox , posdata );
	}

}
*/
//#############################################################################

void MultiObjectLabeler::LabelCMMPoints ( const PointSet & pointsarray 
										 , const std::vector<CString> & alltxt )
										 //, const GLfloat showcolor[3] )
{
	//InitCellsSelected();
	if ( oglwindowupdated == true )
		UpdateCells();

	const int arraysize = pointsarray.Size()*3;
	if ( arraysize > m_pointsInViewSize )
	{
		MakePointsInViewArray (arraysize);
	}

	PrepareVisiblePoints ( m_pointsInView , pointsarray );

	for ( int j=0; j<cellno; j++ )
	{
		vreal cellX = cells[j][X];
		vreal cellY = cells[j][Y];
		int pointIndex = FindNearestPointToCell (cellX,cellY,m_pointsInView,pointsarray.Size());
		if ( pointIndex != -1 )
		{
			int startindex = pointIndex*3;
			CString showtext = alltxt[pointIndex];
			TextBox txtbox ( mxwidth , mywidth , showtext , m_asciiDisp);//, showcolor );
			GLfloat screenpoint[2] = { (GLfloat)m_pointsInView[startindex]
									,(GLfloat)m_pointsInView[startindex+1] };
			TextBoxPosData posdata ( cells[j] , screenpoint , lineend[j] );
			TextBoxDisplayer disptxt ( mwnddata );		
			disptxt.Display ( txtbox , posdata );
		}
	}
}

void MultiObjectLabeler::InitCellsSelected ()
{
	for ( int i=0; i < cellno; i++ )
	{
		cells[i][Z] = 0.0;
	}
}

//#############################################################################

void MultiObjectLabeler::ResetOglWindowData ( const OglWindowData & newwindow )
{
	mwnddata = newwindow;	
	oglwindowupdated = true;
}

//#############################################################################

const vreal & MultiObjectLabeler::GetXwidth ()
{
	return mxwidth;
}

//#############################################################################

const vreal & MultiObjectLabeler::GetYwidth ()
{
	return mywidth;
}

//#############################################################################

void MultiObjectLabeler::UpdateCells()
{
	if ( cellno > 0 )
	{
		DeleteCells();
	}

	cellno = 0;
	cells = 0;
	CalcCellNo();
	MakeCells();
	FillCells();
	oglwindowupdated = false;
	InitCellsSelected();

}

//#############################################################################

void MultiObjectLabeler::PrepareVisiblePoints ( vreal pointsInView[] , const PointSet &pointsarray )
{
	for ( int i=0; i < pointsarray.Size(); i++)
	{
		const VPoint *currpoint = pointsarray[i];
		GLfloat modelpoint [4] 
			= { static_cast <GLfloat> (currpoint->x()) 
			  , static_cast <GLfloat> (currpoint->y()) 
			  , static_cast <GLfloat> (currpoint->z()) , 0.0f };

		GLfloat screenpoint[4];
		bool ck = mwnddata.GetScreenCoordOfPoint ( modelpoint , screenpoint );
			
		int startindex = i*3;
		
		if ( ck == false )
		{
			pointsInView[startindex] = screenpoint[0];
			pointsInView[startindex+1] = screenpoint[1];
			pointsInView[startindex+2] = 1;
		}
		else
		{	
			pointsInView[startindex] = screenpoint[0];
			pointsInView[startindex+1] = screenpoint[1];
			pointsInView[startindex+2] = 0;			
		}
	}
	
}

void MultiObjectLabeler::MakePointsInViewArray (const int arraysize)
{
	if ( m_pointsInViewSize > 0 )
	{
		delete [] m_pointsInView;
	}
	
	m_pointsInView = new vreal [arraysize];
	m_pointsInViewSize = arraysize;
}
