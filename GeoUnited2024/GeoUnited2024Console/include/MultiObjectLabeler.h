// MultiObjectLabeler.h

#ifndef __MultiObjectLabeler_h__
#define __MultiObjectLabeler_h__

#include "OglWindowData.h"
class PointSet;
class Display;

class MultiObjectLabeler
{
public:
	MultiObjectLabeler ( const vreal &xwidth , const vreal &ywidth 
						, const OglWindowData &wnddata , std::vector<Display *> *asciiDisp);
	~MultiObjectLabeler ();
	void CalcCellNo ();
	void MakeCells ();
	void FillCells ();
	void DeleteCells();
	int FindNearestCellToPoint ( const GLfloat screenpnt[2] );
	int FindNearestPointToCell ( const vreal &cellX 
								, const vreal &cellY 
								, vreal pointsInView[] 
								, int pointsInViewNo );
	//void LabelCMMPoints ( const vreal pointsarray[] , const int &pointno 
	//						    , const CString &labeltxt , const GLfloat showcolor[3] );
	void LabelCMMPoints ( const PointSet & pointsarray  
						, const std::vector<CString> &alltxt );//, const GLfloat showcolor[3] );	
	void InitCellsSelected ();
	void ResetOglWindowData ( const OglWindowData & newwindow );
	const vreal & GetXwidth();
	const vreal & GetYwidth();
	void PrepareVisiblePoints ( vreal pointsInView[] , const PointSet &pointsarray );

private:
	void MakePointsInViewArray (const int arraysize);

	// text box data
	vreal mxwidth, mywidth;
	vreal mxwidthE, mywidthE;
	GLfloat **cells;
	GLfloat **lineend;
	int xcellno , ycellno , cellno;
	OglWindowData mwnddata;
	bool oglwindowupdated;
	void UpdateCells();
	vreal *m_pointsInView;
	int m_pointsInViewSize;
	std::vector<Display *> *m_asciiDisp;
};

#endif //__MultiObjectLabeler_h__
